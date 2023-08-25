#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "chip8.h"

#define SCALE 10

static const SDL_Scancode scancodeMap[16] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V,
};

typedef uint8_t u8;
typedef uint16_t u16;

void initSDL(void);

void destroySDL(void);

void cls(CHIP8 *cpu);

void drawSprite(CHIP8 *cpu, u8 n, u8 x, u8 y);

void renderToScreen(CHIP8 *cpu);

int getKeyboardState(u8 key);

int handleUserInterrupt(void);

u8 waitForUserInput(void);

#endif