#include "chip8.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
#include <thread>

Chip8::Chip8()
{

    for (int i = 0; i < 80; i++)
    {
        memory[0x50 + i] = fontset[i];
    }

    for (int i = 0; i < 16; i++)
    {
        keys[i] = false;
    }
    for (int i = 0; i < 64, i < 32; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            display[j][i] = false;
        }
    }

    for (int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
    pc = 0x200;
}

bool Chip8::loadRom(const std::string &filename)
{

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Failed to open ROM: " << filename << std::endl;
        return false;
    }
    std::streamsize size = file.tellg();

    if (size > 3584)
    { // 4096 - 512
        std::cerr << "ROM size exceeds memory limit: " << size << " bytes" << std::endl;
        return false;
    }

    std::vector<char> buffer(size);

    file.seekg(0, std::ios::beg);
    if (!file.read(buffer.data(), size))
    {
        std::cerr << "Failed to read ROM: " << filename << std::endl;
        return false;
    }

    for (long i = 0; i < size; i++)
    {
        memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);
    }
    return true;
}

void Chip8::fetchInstruction()
{
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;
    std::cout << "Fetched opcode: " << opcode << std::endl;
    this->opcode = opcode;
}

void Chip8::updateTimers(){
    if(delayTimer > 0){
        delayTimer--;
    }
    if(soundTimer > 0){
        soundTimer--;
    }
}

void Chip8::executeInstruction()
{

    uint8_t family = (opcode) >> 12;
    switch (family)
    {
    case 0x0:
    {
        std::cout << "Entered family 0" << std::endl;
        if (opcode == 0x00E0)
        {
            opClearScreen();
        }
        else if (opcode == 0x00EE)
        {
            opReturnFromSubroutine();
        }

        break;
    }

    case 0x1:
        opJump();
        break;

    case 0x2:
        opCallSubroutine();
        break;

    case 0x3:
        opSkipIfEqual();
        break;

    case 0x4:
        opSkipIfNotEqual();
        break;

    case 0x5:
        opSkipIfRegistersEqual();
        break;

    case 0x6:
        opLoadImm();
        break;

    case 0x7:
        opAddImm();
        break;

    case 0x8:
    {
        uint8_t lastNibble = opcode & 0x000F;
        switch (lastNibble)
        {
        case 0x0:
            op8COPY();
            break;

        case 0x1:
            op8OR();
            break;

        case 0x2:
            op8AND();
            break;

        case 0x3:
            op8XOR();
            break;

        case 0x4:
            op8ADD();
            break;
 
        case 0x5:
            op8SUB();
            break;

        case 0x6:
            op8SHR();
            break;

        case 0x7:
            op8SUBN();
            break;

        case 0xE:
            op8SHL();
            break;
        default:
            opDefaultInstruction();
            break;
        }
        break;
    }

    case 0x9:
        opSkipIfRegistersNotEqual();
        break;

    case 0xA:
        opSetIndex();
        break;

    case 0xB:
        opJumpWithOffsetV0();
        break;

    case 0xC:
        opRandomAnd();
        break;

    case 0xD:
        opDrawSprite();
        break;

    case 0xE:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t lastByte = (opcode & 0x00FF);
        if (lastByte == 0x9E)
        {
            opSkipIfEqualKeyPressed();
        }
        else if (lastByte == 0xA1)
        {
            opSkipIfEqualKeyNotPressed();
        }
        else
        {
            opDefaultInstruction();
        }
        break;
    }

    case 0xF:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t lastByte = (opcode & 0x00FF);

        switch (lastByte)
        {

        case 0x07:
            opCopyDtToV();
            break;
        case 0x0A:
            opWaitForKeyPress();
            break;
        case 0x15:
            opSetDtToVx();
            break;
        case 0x18:
            opSetStToVx();
            break;
        case 0x1E:
            opAddVxToI();
            break;
        case 0x29:
            opSetItoSpriteStart();
            break;
        case 0x33:
            opVxToBCD();
            break;
        case 0x55:
            opCopyVxToI();
            break;
        case 0x65:
            opCopyItoVx();
            break;
        default:
            opDefaultInstruction();
        }
        break;
    }

    default:
        opDefaultInstruction();
        break;
    };
}

void Chip8::prtScr()
{
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            std::cout << (display[x][y] ? "#" : ".");
        }
        std::cout << std::endl;
    }
}

void Chip8::opLoadImm()
{

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    V[x] = nn;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::opSetIndex()
{
    uint16_t nnn = (opcode & 0x0FFF);
    I = nnn;
    std::cout << "I: " << std::hex << I << std::endl;
}

void Chip8::opRandomAnd()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    int randomValue = dis(gen);

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    std::cout << randomValue << std::endl;
    V[x] = randomValue & nn;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::opSkipIfEqual()
{

    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = (opcode & 0x00FF);
        if (V[x] == nn)
        {
            pc += 2;
            std::cout << "Skipping next instruction" << std::endl;
        }
        else
        {
            std::cout << "Not skipping next instruction" << std::endl;
        }
    }
}

void Chip8::opDrawSprite()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = (opcode & 0x000F);

    // 2. Fix: Initialize the collision flag to 0 before processing pixels
    V[15] = 0;

    for (int i = 0; i < n; i++)
    {
        uint8_t spriteRow = memory[I + i];
        for (int j = 0; j < 8; j++)
        {
            if ((spriteRow & (0x80 >> j)) != 0)
            {
                // Note: Modulo implements screen wrapping (standard for many CHIP-8 games)
                int xCoord = (V[x] + j) % 64;
                int yCoord = (V[y] + i) % 32;

                if (display[xCoord][yCoord])
                {
                    V[15] = 1; // Collision detected
                }

                display[xCoord][yCoord] ^= true;
            }
        }
    }
}

void Chip8::opAddImm()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    V[x] += nn;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::opJump()
{
    uint16_t nnn = (opcode & 0x0FFF);
    pc = nnn;
    std::cout << "Jumping to: " << std::hex << pc << std::endl;
}

void Chip8::opCallSubroutine()
{
    uint16_t nnn = (opcode & 0x0FFF);
    stack[stackPointer] = pc;
    stackPointer++;
    pc = nnn;
    std::cout << "Calling subroutine at: " << std::hex << pc << std::endl;
}

void Chip8::opDefaultInstruction()
{
    std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
    exit(1);
}

void Chip8::opReturnFromSubroutine()
{
    stackPointer--;
    pc = stack[stackPointer];
    std::cout << "Returning from subroutine to: " << std::hex << pc << std::endl;
}

void Chip8::opClearScreen()
{
    std::cout << "Clearing screen" << std::endl;
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            display[i][j] = false;
        }
    }
}

void Chip8::opSkipIfNotEqual()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);
    if (V[x] != nn)
    {
        pc += 2;
        std::cout << "Skipping next instruction" << std::endl;
    }
    else
    {
        std::cout << "Not skipping next instruction" << std::endl;
    }
}

void Chip8::opSkipIfRegistersEqual()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] == V[y])
    {
        pc += 2;
        std::cout << "Skipping next instruction" << std::endl;
    }
    else
    {
        std::cout << "Not skipping next instruction" << std::endl;
    }
}

void Chip8::opSkipIfRegistersNotEqual()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] != V[y])
    {
        pc += 2;
        std::cout << "Skipping next instruction" << std::endl;
    }
    else
    {
        std::cout << "Not skipping next instruction" << std::endl;
    }
}

void Chip8::op8COPY()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] = V[y];
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8OR()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] |= V[y];
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8AND()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] &= V[y];
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8XOR()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] ^= V[y];
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8ADD()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sum = V[x] + V[y];
    V[15] = (sum > 255);
    V[x] = sum;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8SUB()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = (V[x] >= V[y]);
    V[x] -= V[y];
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8SHR()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 1; // Store least significant bit before shift
    V[x] >>= 1;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::op8SUBN()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = (V[y] >= V[x]);
    V[x] = V[y] - V[x];
}

void Chip8::op8SHL()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = (V[x] & 0x80) >> 7; // Store most significant bit before shift
    V[x] <<= 1;
    std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::opJumpWithOffsetV0()
{
    uint16_t nnn = (opcode & 0x0FFF);
    pc = nnn + V[0];
    std::cout << "Jumping to: " << std::hex << pc << std::endl;
}

void Chip8::opSkipIfEqualKeyPressed()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = V[x];
    if (key < 16 && keys[key])
    {
        pc += 2;
        std::cout << "Skipping next instruction because key " << static_cast<int>(key) << " is pressed." << std::endl;
    }
    else
    {
        std::cout << "Not skipping next instruction because key " << static_cast<int>(key) << " is not pressed." << std::endl;
    }
}

void Chip8::opSkipIfEqualKeyNotPressed()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = V[x];
    if (key < 16 && !keys[key])
    {
        pc += 2;
        std::cout << "Skipping next instruction because key " << static_cast<int>(key) << " is not pressed." << std::endl;
    }
    else
    {
        std::cout << "Not skipping next instruction because key " << static_cast<int>(key) << " is pressed." << std::endl;
    }
}

void Chip8::opCopyDtToV()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[x] = delayTimer;
    std::cout << "V[" << static_cast<int>(x) << "] set to delay timer value: " << static_cast<int>(V[x]) << std::endl;
}

void Chip8::opWaitForKeyPress()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int i = 0; i < 16; i++)
    {
        if (keys[i])
        {
            V[x] = i;
            std::cout << "Key " << i << " pressed. V[" << static_cast<int>(x) << "] set to " << i << std::endl;
            return;
        }
    }
    std::cout << "No key pressed. Waiting..." << std::endl;
    pc -= 2; // No key pressed, repeat this instruction
}

void Chip8::opSetDtToVx()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = V[x];
    std::cout << "delay timer value " << static_cast<int>(V[x]) << "set to V[" << static_cast<int>(x) << "] " << std::endl;
}

void Chip8::opSetStToVx()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = V[x];
    std::cout << "sound timer value " << static_cast<int>(V[x]) << "set to V[" << static_cast<int>(x) << "] " << std::endl;
}

void Chip8::opAddVxToI()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    I += V[x];
    std::cout << "Added V[" << static_cast<int>(x) << "] to I" << std::endl;
}

void Chip8::opSetItoSpriteStart()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    I = 0x50 + (V[x] * 5);
}

void Chip8::opVxToBCD()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t regValue = V[x];
    uint8_t hundreds = regValue / 100;
    uint8_t tens = (regValue / 10) % 10;
    uint8_t ones = regValue % 10;
    memory[I] = hundreds;
    memory[I + 1] = tens;
    memory[I + 2] = ones;
}

void Chip8::opCopyVxToI()
{

    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; i++)
    {
        memory[I + i] = V[i];
    }
}

void Chip8::opCopyItoVx()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; i++)
    {
        V[i] = memory[I + i];
    }
}
