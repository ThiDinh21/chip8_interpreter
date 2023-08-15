#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chip8.h"
#include "graphics.h"

#define Vx cpu->v[x]
#define Vy cpu->v[y]
#define VF cpu->v[0xF]
#define I cpu->i
#define PC cpu->pc
#define SP cpu->sp

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

CHIP8 *CHIP8_new(u8 *rom, long romSize)
{
    // For Cxkk
    srand(time(NULL));

    CHIP8 *cpu = calloc(61, sizeof(u8));
    cpu->ram = calloc(4096, sizeof(u8));
    memset(cpu->v, 0, sizeof(cpu->v));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    // TODO: init delay, sound

    SP = 0;
    cpu->pc = 0x0200;

    // Store built-in font in 0x50 - 0x9F
    for (size_t character = 0; character < 16; character++)
    {
        for (size_t byte = 0; byte < 5; byte++)
        {
            write_u8(cpu, (u16)(0x50 + character * 5 + byte), font[character][byte]);
        }
    }

    // Copy program ROM to RAM
    memcpy(cpu->ram + 0x200, rom, romSize);

    return cpu;
}

void CHIP8_destroy(CHIP8 *cpu)
{
    free(cpu->ram);
    free(cpu);
}

void eventLoop(CHIP8 *cpu)
{
    while (1)
    {
        u16 opcode = fetch_opcode(cpu);
        decode(cpu, opcode);
    }
}

static inline void stack_push(CHIP8 *cpu, u16 data)
{
    SP++;
    if (SP > 15)
    {
        fprintf(stderr, "Error: Stack overflow");
        exit(-1);
    }
    cpu->stack[SP] = data;
}

static inline u16 stack_pop(CHIP8 *cpu)
{
    u16 data = cpu->stack[SP];
    if (SP == 0)
    {
        fprintf(stderr, "Error: Stack underflow");
        exit(-1);
    }
    SP--;

    return data;
}

void op_null(u16 opcode)
{
    fprintf(stderr, "Opcode not recognized: %x\n", opcode);
    exit(-1);
}

void decode(CHIP8 *cpu, u16 opcode)
{
    u8 firstNibble = (opcode & 0xF000) >> 12;
    // nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    // n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    // x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    // y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    // kk or byte - An 8-bit value, the lowest 8 bits of the instruction
    u16 nnn = opcode & 0x0FFF;
    u8 n = opcode & 0x000F;
    u8 x = (opcode & 0x0F00) >> 8;
    u8 y = (opcode & 0x00F0) >> 4;
    u8 kk = opcode & 0x00FF;

    u8 sprite[n];
    // printf("nnn: %x; n: %x; x: %x; y: %x; kk: %x\n", nnn, n, x, y, kk);

    switch (firstNibble)
    {
    case 0:
        // 00E0 - CLS
        if (opcode == 0x00E0)
        {
            cls();
        }
        // 00EE - RET
        else if (opcode == 0x00EE)
        {
            PC = stack_pop(cpu);
        }
        // 0nnn - SYS addr
        else
        {
            // Jump to a machine code routine at nnn.
            // This instruction is only used on the old computers on
            // which Chip-8 was originally implemented.
            // It is ignored by modern interpreters.
            op_null(opcode);
        }
        break;
    case 1:
        // 1nnn - JUMP
        cpu->pc = nnn;
        break;
    case 2:
        // 2nnn - CALL addr
        stack_push(cpu, PC);
        PC = nnn;
        break;
    case 3:
        // 3xkk - SE Vx, byte
        if (Vx == kk)
        {
            PC += 2;
        }
        break;
    case 4:
        // 4xkk - SNE Vx, byte
        if (Vx != kk)
        {
            PC += 2;
        }
        break;
    case 5:
        // 5xy0 - SE Vx, Vy
        if (Vx == Vy)
        {
            PC += 2;
        }
        break;
    case 6:
        // 6xkk - LD Vx, kk
        Vx = kk;
        break;
    case 7:
        // 7xkk - ADD Vx, kk
        Vx += kk;
        break;
    case 8:
        switch (n)
        {
        case 0x0:
            // 8xy0 - LD Vx, Vy
            Vx = Vy;
            break;
        case 0x1:
            // 8xy1 - OR Vx, Vy
            Vx |= Vy;
            break;
        case 0x2:
            // 8xy2 - AND Vx, Vy
            Vx &= Vy;
            break;
        case 0x3:
            // 8xy3 - XOR Vx, Vy
            Vx ^= Vy;
            break;
        case 0x4:
            // 8xy4 - ADD Vx, Vy
            u8 sum = Vx + Vy;
            VF = sum < Vx || sum < Vy;
            Vx = sum;
            break;
        case 0x5:
            // 8xy5 - SUB Vx, Vy
            VF = Vx > Vy;
            Vx -= Vy;
            break;
        case 0x6:
            // 8xy6 - SHR Vx {, Vy}
            VF = (Vx & 0b1) > 0;
            Vx = Vx >> 1;
            break;
        case 0x7:
            // SUBN Vx, Vy
            VF = Vy > Vx;
            Vx = Vy - Vx;
            break;
        case 0xE:
            // SHL Vx {, Vy}
            VF = (Vx & 0b10000000) > 0;
            Vx = Vx << 1;
            break;
        default:
            op_null(opcode);
            break;
        }
        break;
    case 9:
        // 9xy0 - SNE Vx, Vy
        if (Vx != Vy)
        {
            PC += 2;
        }
        break;
    case 0xA:
        // Annn - LD I, addr
        I = nnn;
        break;
    case 0xB:
        // JP V0, addr
        PC = (u16)cpu->v[0] + nnn;
        break;
    case 0xC:
        // Cxkk - RND Vx, byte
        Vx = (u8)(rand() % 255) & kk;
        break;
    case 0xD:
        // Dxyn - DRW Vx, Vy, nibble
        for (size_t i = 0; i < n; i++)
        {
            sprite[i] = read_u8(cpu, I + (u16)i);
        }

        VF = drawSprite(sprite, n, Vx, Vy);

        break;
    case 0xE:
        /* code */
        break;
    case 0xF:
        /* code */
        break;
    default:
        op_null(opcode);
        break;
    }
}
