#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
#include <thread>

class Chip8
{
private:
    uint8_t V[16];   // registers
    uint16_t opcode; // opcode
    uint16_t I;      // index register
    uint16_t stack[16];
    int stackPointer = 0;
    uint8_t delayTimer;
    uint8_t soundTimer;
    static constexpr uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

public:
    uint8_t memory[4096];
    bool display[64][32];
    bool keys[16];

    uint16_t pc; // program counter
    Chip8();

    // Loading ROM
    bool loadRom(const std::string &filename);

    // Instruction handling
    void fetchInstruction();
    void executeInstruction();

    // Test screen
    void prtScr();

    //Update timers
    void updateTimers();

    // Instruction implementations
    void opLoadImm();
    void opSetIndex();
    void opRandomAnd();
    void opSkipIfEqual();
    void opDrawSprite();
    void opAddImm();
    void opJump();
    void opCallSubroutine();
    void opDefaultInstruction();
    void opReturnFromSubroutine();
    void opClearScreen();
    void opSkipIfNotEqual();
    void opSkipIfRegistersEqual();
    void opSkipIfRegistersNotEqual();
    // 8 Family instructions
    void op8COPY();
    void op8OR();
    void op8AND();
    void op8XOR();
    void op8ADD();
    void op8SUB();
    void op8SHR();
    void op8SUBN();
    void op8SHL();
    void opJumpWithOffsetV0();
    // E family Instructions
    void opSkipIfEqualKeyPressed();
    void opSkipIfEqualKeyNotPressed();
    // F family Instructions
    void opCopyDtToV();
    void opWaitForKeyPress();
    void opSetDtToVx();
    void opSetStToVx();
    void opAddVxToI();
    void opSetItoSpriteStart();
    void opVxToBCD();
    void opCopyVxToI();
    void opCopyItoVx();
    
    void cycle();
};
