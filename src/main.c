#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "chip8.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8 Interpreter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return 1;
    }

    CHIP8 *cpu = CHIP8_new();

    SDL_Event event;
    int quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    CHIP8_destroy(cpu);

    return 0;
}