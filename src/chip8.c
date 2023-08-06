#include "chip8.h"

u8 mem_read_u8(CHIP8 *cpu, u16 addr)
{
    return cpu->ram[addr];
}

void mem_write_u8(CHIP8 *cpu, u16 addr, u8 data)
{
    cpu->ram[addr] = data;
}

u16 mem_read_u16(CHIP8 *cpu, u16 addr)
{
    u8 msb = mem_read_u8(cpu, addr);
    u8 lsb = mem_read_u8(cpu, addr + 1);

    return ((u16)msb << 8) | lsb;
}

void mem_write_u16(CHIP8 *cpu, u16 addr, u16 data)
{
    u8 msb = data >> 8;
    u8 lsb = data;

    mem_write_u8(cpu, addr, msb);
    mem_write_u8(cpu, addr + 1, lsb);
}
