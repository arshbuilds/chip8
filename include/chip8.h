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
    public:
        uint8_t memory[4096];
        bool display[64][32];

        uint16_t pc; // program counter
        Chip8();

        //Loading ROM
        bool loadRom(const std::string &filename);

        //Instruction handling
        void fetchInstruction();
        void executeInstruction();

        //Test screen
        void prtScr();


        //Instruction implementations
        void opLoadImm();
        void opSetIndex();
        void opRandomAnd();
        void opSkipIfEqual();
        void opDrawSprite();
        void opAddImm();
        void opJump();
        void opCallSubroutine();
        void defaultInstruction();
        void returnFromSubroutine();
        void clearScreen();

};
