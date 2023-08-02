#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

struct CPU
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
};
