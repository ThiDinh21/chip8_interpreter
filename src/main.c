#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "graphics.h"

int getROMSize(FILE *fptr);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing argument: Path to ROM");
        return -1;
    }

    char *pathToROM = argv[1];
    FILE *fptr;
    fptr = fopen(pathToROM, "rb");

    if (fptr == NULL)
    {
        fprintf(stderr, "File does not exist");
        return -1;
    }

    int romSize = getROMSize(fptr);
    u8 *rom = (u8 *)malloc(romSize);
    fread(rom, romSize, 1, fptr);
    CHIP8 *cpu = CHIP8_new(rom, romSize);

    fclose(fptr);
    free(rom);

    // Create SDL Window
    initSDL();

    eventLoop(cpu);
    // testSDL();

    // Clean up
    destroySDL();
    CHIP8_destroy(cpu);

    return 0;
}

int getROMSize(FILE *fptr)
{
    int size;
    fseek(fptr, 0, SEEK_END); // Jump to EOF
    size = ftell(fptr);       // Get the cuurent byte offset
    rewind(fptr);             // Jump back to the beginning of file
    return size;
}
