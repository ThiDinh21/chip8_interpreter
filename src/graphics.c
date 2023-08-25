#include "chip8.h"
#include "graphics.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;
static u8 keyboard[16] = {0};

static inline u8 getKeyValue(SDL_Scancode);

void initSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("CHIP-8 Interpreter",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              64 * SCALE, 32 * SCALE,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 64, 32);

    surface = SDL_GetWindowSurface(window);
}

void destroySDL(void)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

void renderToScreen(CHIP8 *cpu)
{
    SDL_SetRenderTarget(renderer, texture);

    for (size_t x = 0; x < 64; x++)
    {
        for (size_t y = 0; y < 32; y++)
        {
            u8 color = cpu->display[x][y] == 0 ? 0 : 255;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

static inline u8 getKeyValue(SDL_Scancode code)
{
    for (u8 i = 0; i < 16; i++)
    {
        if (scancodeMap[i] == code)
        {
            return i;
        }
    }
    return 16;
}

int getKeyboardState(u8 key)
{
    return keyboard[key];
}

int handleUserInterrupt(void)
{
    SDL_Event event;
    SDL_Scancode scancode;
    u8 key = 16;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return 1;
        case SDL_KEYDOWN:
            scancode = event.key.keysym.scancode;
            key = getKeyValue(scancode);
            if (key < 16)
            {
                keyboard[key] = 1;
            }
            break;
        case SDL_KEYUP:
            scancode = event.key.keysym.scancode;
            key = getKeyValue(scancode);
            if (key < 16)
            {
                keyboard[key] = 0;
            }
            break;
        default:
            break;
        }
    }

    return 0;
}

u8 waitForUserInput(void)
{
    SDL_Event event;
    SDL_Scancode scancode;
    u8 key = 16;

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit(1);
            case SDL_KEYDOWN:
                scancode = event.key.keysym.scancode;
                key = getKeyValue(scancode);
                if (key < 16)
                {
                    keyboard[key] = 1;
                    return key;
                }
                break;
            default:
                break;
            }
        }
    }
}