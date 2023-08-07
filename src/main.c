#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "chip8.h"

int main()
{
    CHIP8 *cpu = CHIP8_new();

    printf("%x", read_u8(cpu, 0x0055));

    CHIP8_destroy(cpu);

    return 0;
}