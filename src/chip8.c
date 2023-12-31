#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "chip8.h"
#include "graphics.h"

#define Vx cpu->v[x]
#define Vy cpu->v[y]
#define VF cpu->v[0xF]
#define PC cpu->pc
#define SP cpu->sp

CHIP8 *CHIP8_new(u8 *rom, long romSize)
{
    // For Cxkk
    srand(time(NULL));
    CHIP8 *cpu = calloc(sizeof(*cpu), sizeof(u8));
    cpu->ram = calloc(4096, sizeof(u8));
    memset(cpu->v, 0, sizeof(cpu->v));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    memset(cpu->display, 0, sizeof(cpu->display));
    cpu->drawFlag = 0;

    // TODO: init sound
    cpu->delayTimer = 0;

    SP = 0;
    PC = 0x0200;

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

void cycle(CHIP8 *cpu)
{
    while (1)
    {
        if (handleUserInterrupt() == 1)
        {
            break;
        }
        u16 opcode = fetch_opcode(cpu);
        decode(cpu, opcode);

        if (cpu->drawFlag == 1)
        {
            renderToScreen(cpu);
        }

        if (cpu->delayTimer > 0)
        {
            cpu->delayTimer--;
        }
        usleep(1000);
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

    switch (firstNibble)
    {
    case 0:
        // 00E0 - CLS
        if (opcode == 0x00E0)
        {
            cls(cpu);
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
        PC = nnn;
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
        cpu->i = nnn;
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
        drawSprite(cpu, n, Vx, Vy);
        break;
    case 0xE:
        // Ex9E - SKP Vx
        if (kk == 0x9E)
        {
            if (getKeyboardState(Vx) == 1)
            {
                PC += 2;
            }
        }
        // ExA1 - SKNP Vx
        else if (kk == 0xA1)
        {
            if (getKeyboardState(Vx) == 0)
            {
                PC += 2;
            }
        }
        else
        {
            op_null(opcode);
        }
        break;
    case 0xF:
        switch (kk)
        {
        // Fx07 - LD Vx, DT
        case 0x07:
            Vx = cpu->delayTimer;
            break;
        // LD Vx, K
        case 0x0A:
            Vx = waitForUserInput();
            break;
        // Fx15 - LD DT, Vx
        case 0x15:
            cpu->delayTimer = Vx;
            break;
        case 0x18:
            // TODO: sound
            break;
        // Fx1E - ADD I, Vx
        case 0x1E:
            cpu->i += Vx;
            break;
        // Fx29 - LD F, Vx
        case 0x29:
            cpu->i = 0x50 + Vx * 5;
            break;
        // Fx33 - LD B, Vx
        case 0x33:
            write_u8(cpu, cpu->i, (Vx / 100) % 10);
            write_u8(cpu, cpu->i + 1, (Vx / 10) % 10);
            write_u8(cpu, cpu->i + 2, Vx % 10);
            break;
        // Fx55 - LD [I], Vx
        case 0x55:
            for (int i = 0; i < x + 1; i++)
            {
                write_u8(cpu, cpu->i + i, cpu->v[i]);
            }
            break;
        // Fx65 - LD Vx, [I]
        case 0x65:
            for (int i = 0; i < x + 1; i++)
            {
                cpu->v[i] = read_u8(cpu, cpu->i + i);
            }
            break;
        default:
            op_null(opcode);
            break;
        }
        break;
    default:
        op_null(opcode);
        break;
    }
}

void cls(CHIP8 *cpu)
{
    memset(cpu->display, 0, sizeof(cpu->display));
    cpu->drawFlag = 1;
}

void drawSprite(CHIP8 *cpu, u8 n, u8 x, u8 y)
{
    u8 sprite[n];
    u8 isCollided = 0;

    for (size_t i = 0; i < n; i++)
    {
        sprite[i] = read_u8(cpu, cpu->i + (u16)i);
    }

    for (size_t row = 0; row < n; row++)
    {
        if (y + row >= 32)
        {
            break;
        }
        u8 spriteRow = sprite[row];
        for (size_t bit = 0; bit < 8; bit++)
        {
            if (x + bit >= 64)
            {
                break;
            }
            const u8 spritePixel = spriteRow & (0b10000000 >> bit);

            if (spritePixel != 0)
            {
                if (cpu->display[x + bit][y + row] == 1)
                {
                    isCollided = 1;
                }
                cpu->display[x + bit][y + row] ^= 1;
            }
        }
    }

    cpu->v[0xF] = isCollided;
    cpu->drawFlag = 1;
}
