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
    chip8.loadRom("D:\\Projects\\chip8\\corax.ch8");
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();

    while (true)
    {
        std::this_thread::sleep_for(chrono::milliseconds(50));
        chip8.fetchInstruction();
        chip8.executeInstruction();
        auto now = std::chrono::high_resolution_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastTimerUpdate)
                .count() >= 16)
        {
            chip8.updateTimers();
            lastTimerUpdate = now;
        }
    }

    return 0;
}