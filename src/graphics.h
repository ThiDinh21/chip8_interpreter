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

/// @brief Initiate SDL Window, Renderer and Texture
void initSDL(void);

/// @brief Clean up SDL components
void destroySDL(void);

/// @brief Render the CPU's display to SDL Window
/// @param cpu a pointer of type CHIP8 *
void renderToScreen(CHIP8 *cpu);

/// @brief Check a key to see if it is pressed
/// @param key the key, represented by a number from 0x0 to 0xF
/// @return 0 or 1 (not pressed or pressed)
int getKeyboardState(u8 key);

/// @brief Handle the user input, update the key pad accordingly
/// @return 1 if the user pressed Esc, 0 otherwise
int handleUserInterrupt(void);

/// @brief Wait till user press something, loop otherwise
/// @return the number represent the pressed key
u8 waitForUserInput(void);

#endif