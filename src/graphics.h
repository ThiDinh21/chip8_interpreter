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

#endif