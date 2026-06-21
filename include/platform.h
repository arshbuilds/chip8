#pragma once


#include <SDL2/SDL.h>

class Platform
{
private:

    SDL_Window *window;
    SDL_Renderer *renderer;

public:
    int width, height, scale;
    Platform(int w, int h, int s);

    void render(bool display[64][32]);
    
};