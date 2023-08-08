#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const u8 font[16][5] = {
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, //  0
    {0x20, 0x60, 0x20, 0x20, 0x70}, //  1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, //  2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, //  3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, //  4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, //  5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, //  6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, //  7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, //  8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, //  9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, //  A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, //  B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, //  C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, //  D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, //  E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}, //  F
};

void cls(SDL_Renderer *renderer);

CHIP8 *CHIP8_new()
{
    CHIP8 *cpu = calloc(61, sizeof(u8));
    cpu->ram = calloc(4096, sizeof(u8));
    memset(cpu->v, 0, sizeof(cpu->v));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    // TODO: init delay, sound, pc, sp

    // Store built-in font in 0x50 - 0x9F
    for (size_t letter = 0; letter < 16; letter++)
    {
        for (size_t byte = 0; byte < 5; byte++)
        {
            write_u8(cpu, (u16)(0x50 + letter * 5 + byte), font[letter][byte]);
        }
    }

    return cpu;
}

void CHIP8_destroy(CHIP8 *cpu)
{
    free(cpu->ram);
    free(cpu);
}

void decode(SDL_Renderer *renderer, CHIP8 *cpu, u16 opcode)
{
    u8 firstNibble = (opcode & 0xF000) >> 12;

    switch (firstNibble)
    {
    case 0:
        if (opcode == 0x00E0)
        {
            cls(renderer);
        }
        else if (opcode == 00E0)
        {
            // TODO: RET
        }
        else
        {
            // TODO: 0nnn
        }
        break;
    case 1:
        // TODO: 1nnn
        break;
    case 2:
        // TODO: 2nnn
        break;
    case 3:
        // TODO: 3xkk
        break;
    case 4:
        // TODO: 4xkk
        break;
    case 5:
        // TODO: 5xy0
        break;
    case 6:
        // TODO: 6xkk
        break;
    case 7:
        // TODO: 7xkk
        break;
    case 8:
        // TODO: 8 stuffs
        break;
    case 9:
        // TODO: 9xy0
        break;
    case 0xA:
        /* code */
        break;
    case 0xB:
        /* code */
        break;
    case 0xC:
        /* code */
        break;
    case 0xD:
        /* ode */
        break;
    case 0xE:
        /* code */
        break;
    case 0xF:
        /* code */
        break;

    default:
        break;
    }
}

void cls(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}