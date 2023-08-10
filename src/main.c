#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "graphics.h"

int main()
{
    initSDL();

    CHIP8 *cpu = CHIP8_new();
    SDL_Event event;
    int quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
                break;
            }
        }

        testSDL();
        break;
    }

    // Clean up SDL
    destroySDL();

    CHIP8_destroy(cpu);

    return 0;
}