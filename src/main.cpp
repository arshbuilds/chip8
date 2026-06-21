#define SDL_MAIN_HANDLED
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "chip8.h"
#include "platform.h"

using namespace std;

int main()
{
    Chip8 chip8;
    Platform platform(640, 320, 10);
    chip8.loadRom("D:\\Projects\\chip8\\corax.ch8");
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        for (int i = 0; i < 10; i++)
        {
            chip8.fetchInstruction();
            chip8.executeInstruction();
        }

        auto now = std::chrono::high_resolution_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastTimerUpdate)
                .count() >= 16)
        {
            chip8.updateTimers();
            lastTimerUpdate = now;
        }

        platform.render(chip8.display);

    }

    return 0;
}