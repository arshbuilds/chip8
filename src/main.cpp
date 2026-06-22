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
    chip8.loadRom("D:\\Projects\\chip8\\Pong.ch8");
    constexpr auto CPU_DELAY =
        std::chrono::microseconds(1000);
    auto lastTimerUpdate = std::chrono::high_resolution_clock::now();

    bool running = true;

    while (running)
    {

        // Event handling
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.keys[0x1] = true;
                    break;
                case SDLK_2:
                    chip8.keys[0x2] = true;
                    break;
                case SDLK_3:
                    chip8.keys[0x3] = true;
                    break;
                case SDLK_4:
                    chip8.keys[0xC] = true;
                    break;

                case SDLK_q:
                    chip8.keys[0x4] = true;
                    break;
                case SDLK_w:
                    chip8.keys[0x5] = true;
                    break;
                case SDLK_e:
                    chip8.keys[0x6] = true;
                    break;
                case SDLK_r:
                    chip8.keys[0xD] = true;
                    break;

                case SDLK_a:
                    chip8.keys[0x7] = true;
                    break;
                case SDLK_s:
                    chip8.keys[0x8] = true;
                    break;
                case SDLK_d:
                    chip8.keys[0x9] = true;
                    break;
                case SDLK_f:
                    chip8.keys[0xE] = true;
                    break;

                case SDLK_z:
                    chip8.keys[0xA] = true;
                    break;
                case SDLK_x:
                    chip8.keys[0x0] = true;
                    break;
                case SDLK_c:
                    chip8.keys[0xB] = true;
                    break;
                case SDLK_v:
                    chip8.keys[0xF] = true;
                    break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.keys[0x1] = false;
                    break;
                case SDLK_2:
                    chip8.keys[0x2] = false;
                    break;
                case SDLK_3:
                    chip8.keys[0x3] = false;
                    break;
                case SDLK_4:
                    chip8.keys[0xC] = false;
                    break;

                case SDLK_q:
                    chip8.keys[0x4] = false;
                    break;
                case SDLK_w:
                    chip8.keys[0x5] = false;
                    break;
                case SDLK_e:
                    chip8.keys[0x6] = false;
                    break;
                case SDLK_r:
                    chip8.keys[0xD] = false;
                    break;

                case SDLK_a:
                    chip8.keys[0x7] = false;
                    break;
                case SDLK_s:
                    chip8.keys[0x8] = false;
                    break;
                case SDLK_d:
                    chip8.keys[0x9] = false;
                    break;
                case SDLK_f:
                    chip8.keys[0xE] = false;
                    break;

                case SDLK_z:
                    chip8.keys[0xA] = false;
                    break;
                case SDLK_x:
                    chip8.keys[0x0] = false;
                    break;
                case SDLK_c:
                    chip8.keys[0xB] = false;
                    break;
                case SDLK_v:
                    chip8.keys[0xF] = false;
                    break;
                }
            }

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Cycling logic
        chip8.cycle();

        // Sound and delay timer update logic

        auto nowTimer = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                nowTimer - lastTimerUpdate)
                .count() >= 16)
        {
            chip8.updateTimers();
            lastTimerUpdate = nowTimer;
        }

        platform.render(chip8.display);
        std::this_thread::sleep_for(
            std::chrono::microseconds(CPU_DELAY));
    }

    return 0;
}