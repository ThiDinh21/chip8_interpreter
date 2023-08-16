#include "graphics.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;
static u8 keyboard[16] = {0};

u8 getPixel(u8 x, u8 y);

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

    // Clear color texture
    cls();
}

void destroySDL(void)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void cls(void)
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

u8 getPixel(u8 x, u8 y)
{
    const u8 bpp = surface->format->BytesPerPixel;
    /*
    Retrieve the address to a specific pixel
    surface->pixels	    = an array containing the SDL_Surface' pixels
    surface->pitch		= the length of a row of pixels (in bytes)
    X and Y				= the offset on where on the image to retrieve the pixel, (0, 0) is in the upper left corner of the image
    */
    u8 *pixel = (u8 *)surface->pixels + y * surface->pitch + x * bpp;
    Uint32 pixelData = *(Uint32 *)pixel;
    u8 r, g, b;
    SDL_GetRGB(pixelData, surface->format, &r, &g, &b);

    return (r == 0 && g == 0 && b == 0) ? 0 : 1;
}

u8 drawSprite(u8 *sprite, u8 n, u8 x, u8 y)
{
    SDL_SetRenderTarget(renderer, texture);

    u8 isCollided = 0;
    for (size_t row = 0; row < n; row++)
    {
        u8 spriteRow = sprite[row];
        for (size_t bit = 0; bit < 8; bit++)
        {
            const u8 spritePixel = (spriteRow & 0b10000000) >> 7;
            const u8 pixel = getPixel((x + bit) * SCALE, (y + row) * SCALE);
            const u8 color = pixel ^ spritePixel ? 255 : 0;

            if (pixel == 1)
            {
                isCollided = 1;
            }

            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, (x + bit) % 64, (y + row) % 32);

            spriteRow = spriteRow << 1;
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    return isCollided;
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