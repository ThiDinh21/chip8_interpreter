#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "graphics.h"

u8 *readROM(char *path);

int main(int argc, char **argv)
{
    initSDL();

    CHIP8 *cpu = CHIP8_new();

    if (argc < 2)
    {
        fprintf(stderr, "Missing argument: Path to ROM");
        return -1;
    }

    char *pathToROM = argv[1];
    u8 *rom = readROM(pathToROM);

    if (rom == NULL)
    {
        fprintf(stderr, "File does not exist");
        return -1;
    }

    // Clean up SDL
    destroySDL();

    CHIP8_destroy(cpu);
    free(rom);

    return 0;
}

u8 *readROM(char *path)
{
    FILE *fptr;
    fptr = fopen(path, "rb");
    u8 *buffer;
    long len;

    if (fptr == NULL)
        return NULL;

    fseek(fptr, 0, SEEK_END); // Jump to EOF
    len = ftell(fptr);        // Get the cuurent byte offset
    rewind(fptr);             // Jump back to the beginning of file
    buffer = (u8 *)malloc(len);

    fread(buffer, len, 1, fptr);
    fclose(fptr);

    return buffer;
}