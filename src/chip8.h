#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct Chip8
{
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#memmap
    // Memory Map (4kB = 4096 bytes):
    // +---------------+= 0xFFF (4095) End of Chip-8 RAM
    // |               |
    // |               |
    // |               |
    // |               |
    // |               |
    // | 0x200 to 0xFFF|
    // |     Chip-8    |
    // | Program / Data|
    // |     Space     |
    // |               |
    // |               |
    // |               |
    // +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
    // |               |
    // |               |
    // |               |
    // +---------------+= 0x200 (512) Start of most Chip-8 programs
    // | 0x000 to 0x1FF|
    // | Reserved for  |
    // |  interpreter  |
    // +---------------+= 0x000 (0) Start of Chip-8 RAM
    u8 *ram;
    u16 stack[16];
    u8 v[16]; // 16 general purpose 8-bit registers
    u16 i;    // 16-bit registers general used to store adresses so only the lowest (rightmost) 12 bits are usually used
    // TODO: delay + sound registers
    u8 delay_timer;
    u16 pc; // program counter
    u8 sp;  // stack pointer
} CHIP8;

/// @brief CHIP-8's common built-in font. Stored in 0x50 - 0x9F
static const u8 font[16][5] = {
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

/// @brief CHIP8's constructor, will allocate memory for the CPU's RAM
/// @param rom a byte array represent the chip8 program
/// @param romSize length of the rom array
/// @return a pointer to the newly created struct
CHIP8 *CHIP8_new(u8 *rom, long romSize);

/// @brief CHIP8's destructor, will free allocated memory for the RAM
/// @param cpu a pointer to the CHIP8 that need to be freed
void CHIP8_destroy(CHIP8 *cpu);

/// @brief Read the byte at the address in the RAM of the CPU
/// @param cpu a pointer of type CHIP8 *
/// @param addr address of the data
/// @return a byte read from RAM
static inline u8 read_u8(CHIP8 *cpu, u16 addr)
{
    return cpu->ram[addr];
}

/// @brief Write a byte to the CPU's RAM at the adress
/// @param cpu a pointer of type CHIP8 *
/// @param addr address of the data
/// @param data data to be writen
static inline void write_u8(CHIP8 *cpu, u16 addr, u8 data)
{
    cpu->ram[addr] = data;
}

/// @brief Read the byte and the one after that (big-edian) at the address in the RAM of the CPU
/// @param cpu a pointer of type CHIP8 *
/// @param addr address of the data
/// @return 2 bytes read from RAM in big-edian
static inline u16 read_u16(CHIP8 *cpu, u16 addr)
{
    u8 msb = read_u8(cpu, addr);
    u8 lsb = read_u8(cpu, addr + 1);

    return ((u16)msb << 8) | lsb;
}

/// @brief Write 2 bytes (big-edian) to the CPU's RAM at the adress
/// @param cpu a pointer of type CHIP8 *
/// @param addr address of the data
/// @param data data to be writen
static inline void write_u16(CHIP8 *cpu, u16 addr, u16 data)
{
    u8 msb = data >> 8;
    u8 lsb = data;

    write_u8(cpu, addr, msb);
    write_u8(cpu, addr + 1, lsb);
}

/// @brief Read the next opcode from the library and increment the PC by 2
/// @param cpu a pointer of type CHIP8 *
/// @return the opcode as an u16
static inline u16 fetch_opcode(CHIP8 *cpu)
{
    u16 opcode = read_u16(cpu, cpu->pc);
    cpu->pc += 2;
    return opcode;
}

void eventLoop(CHIP8 *cpu);

void decode(CHIP8 *cpu, u16 opcode);

#endif