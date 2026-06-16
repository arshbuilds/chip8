#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "chip8.h"

using namespace std;


int main()
{
    Chip8 chip8;
    chip8.loadRom("D:\\Projects\\chip8\\maze.ch8");
    while (true)
    {
        std::this_thread::sleep_for(chrono::milliseconds(50));
        chip8.fetchInstruction();
        chip8.executeInstruction();
    }

    return 0;
}