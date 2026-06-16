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
    std::cout << "Fetched opcode: " << std::hex << opcode << std::endl;
    this->opcode = opcode;
}

void Chip8::executeInstruction()
{

    uint8_t family = (opcode) >> 12;
    switch (family)
    {
    case 0x6:
        opLoadImm();
        break;

    case 0xA:
        opSetIndex();
        break;

    case 0xC:
        opRandomAnd();
        break;

    case 0x3:
        opSkipIfEqual();
        break;

    case 0xD:
        opDrawSprite();
        break;

    case 0x7:
        opAddImm();
        break;

    case 0x1:
        opJump();
        break;

    case 0x2:
        opCallSubroutine();
        break;
    
    case 0x0:{
        if(opcode == 0x00E0)
        {
            clearScreen();
        }
        else if(opcode == 0x00EE)
        {
            returnFromSubroutine();
        }
    }

    default:
        defaultInstruction();
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

void Chip8::defaultInstruction()
{
    std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
    exit(1);
}

void Chip8::returnFromSubroutine()
{ 
    stackPointer--;
    pc = stack[stackPointer -1];
    std::cout << "Returning from subroutine to: " << std::hex << pc << std::endl;

}

void Chip8::clearScreen()
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