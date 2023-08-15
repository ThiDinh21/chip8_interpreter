#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <SDL2/SDL.h>

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
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
};

typedef uint8_t u8;
typedef uint16_t u16;

void initSDL(void);

void destroySDL(void);

void testSDL(void);

void cls(void);

u8 drawSprite(u8 *sprite, u8 n, u8 x, u8 y);

int getUserInterrupt(void);

#endif