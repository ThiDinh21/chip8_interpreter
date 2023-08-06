#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct Chip8
{
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#memmap
    // Memory Map:
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
    u16 pc; // program counter
    u8 sp;  // stack pointer
} CHIP8;

/// @brief CHIP8's constructor, will allocate memory for the CPU's RAM
/// @return a pointer to the newly created struct
CHIP8 *CHIP8_new();

/// @brief CHIP8's destructor, will free allocated memory for the RAM
/// @param cpu - a pointer to the CHIP8 that need to be freed
void CHIP8_destroy(CHIP8 *cpu);

/// @brief Read the byte at the address in the RAM of the CPU
/// @param cpu - a pointer to the CPU
/// @param addr - address of the data
/// @return a byte read from RAM
u8 mem_read_u8(CHIP8 *cpu, u16 addr);

/// @brief Write a byte to the CPU's RAM at the adress
/// @param cpu - a pointer to the CPU
/// @param addr - address of the data
/// @param data - data to be writen
void mem_write_u8(CHIP8 *cpu, u16 addr, u8 data);

/// @brief Read the byte and the one after that (big-edian) at the address in the RAM of the CPU
/// @param cpu - a pointer to the CPU
/// @param addr - address of the data
/// @return 2 bytes read from RAM in big-edian
u16 mem_read_u16(CHIP8 *cpu, u16 addr);

/// @brief Write 2 bytes (big-edian) to the CPU's RAM at the adress
/// @param cpu - a pointer to the CPU
/// @param addr - address of the data
/// @param data - data to be writen
void mem_write_u16(CHIP8 *cpu, u16 addr, u16 data);

u16 fetchOpcode(CHIP8 *cpu);

/// @brief CHIP-8's common built-in font. Stored in 0x50 - 0x9F
const u8 font[16][16] = {
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

#endif