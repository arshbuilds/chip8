#include "platform.h"

#include <SDL2/SDL.h>

Platform::Platform(int w, int h, int s)
{
    this->width = w;
    this->height = h;
    this->scale = s;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window =
        SDL_CreateWindow(
            "CHIP-8",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            320,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);


    }
    
    void Platform::render(bool display[64][32])
    {
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                if (display[y][x])
                {
                    SDL_Rect pixel =
                    {
                        x * scale,
                        y * scale,
                        scale,
                        scale};
                        
                        SDL_RenderFillRect(renderer, &pixel);
                    }
                }
            }
            SDL_RenderPresent(renderer);
}
