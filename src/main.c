#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "chip8.h"

int main()
{
    CHIP8 *cpu = CHIP8_new();
    // cpu->pc = 0x55;

    // printf("%x, %x\n", fetch_opcode(cpu), cpu->pc);
    // printf("%x, %x\n", fetch_opcode(cpu), cpu->pc);
    // printf("%x, %x\n", fetch_opcode(cpu), cpu->pc);
    // printf("%x, %x\n", fetch_opcode(cpu), cpu->pc);
    u16 opcode = 0x9230;
    u8 firstNibble = (opcode & 0xF000) >> 12;
    printf("%x", firstNibble);

    CHIP8_destroy(cpu);

    // printf("Makefile is shit!");

    return 0;
}