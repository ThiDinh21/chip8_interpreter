#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <SDL2/SDL.h>

#define SCALE 10

typedef uint8_t u8;
typedef uint16_t u16;

void initSDL(void);

void destroySDL(void);

void testSDL(void);

void cls(void);

u8 drawSprite(u8 *sprite, u8 n, u8 x, u8 y);

#endif