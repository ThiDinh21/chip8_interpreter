#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;

void initSDL(void);

void destroySDL(void);

void cls(SDL_Renderer *renderer);

u8 getPixel(SDL_Surface *surface, u8 x, u8 y);

u8 drawSprite(SDL_Renderer *renderer, SDL_Surface *surface, u8 *sprite, u8 n, u8 x, u8 y);

#endif