#include "graphics.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;

void cls(void);

u8 getPixel(u8 x, u8 y);

u8 drawSprite(u8 *sprite, u8 n, u8 x, u8 y);

void testSDL(void)
{
    // u8 sprite[5] = {0x11, 0x00, 0xF0, 0xFF};

    // 0xff, 0x0, 0xff, 0x0, 0x3c, 0x0, 0x3c, 0x0, 0x3c, 0x0, 0x3c, 0x0, 0xff, 0x0, 0xff,
    // 0xff, 0x0, 0xff, 0x0, 0x38, 0x0, 0x3f, 0x0, 0x3f, 0x0, 0x38, 0x0, 0xff, 0x0, 0xff,
    // 0x80, 0x0, 0xe0, 0x0, 0xe0, 0x0, 0x80, 0x0, 0x80, 0x0, 0xe0, 0x0, 0xe0, 0x0, 0x80,
    // 0xf8, 0x0, 0xfc, 0x0, 0x3e, 0x0, 0x3f, 0x0, 0x3b, 0x0, 0x39, 0x0, 0xf8, 0x0, 0xf8,
    // 0x3, 0x0, 0x7, 0x0, 0xf, 0x0, 0xbf, 0x0, 0xfb, 0x0, 0xf3, 0x0, 0xe3, 0x0, 0x43,
    // 0xe0, 0x0, 0xe0, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x0, 0xe0, 0x0, 0xe0,

    u8 sprite[4] = {
        // 0xff,
        // 0x0,
        // 0xff,
        // 0x0,
        // 0x38,
        // 0x0,
        0x3f,
        0x0,
        0x3f,
        0x0,
        // 0x38,
        // 0x0,
        // 0xff,
        // 0x0,
        // 0xff,
    };

    drawSprite(sprite, sizeof(sprite) / sizeof(sprite[0]), 0, 0);

    SDL_Delay(10000);
}

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

    // printf("%d %d %d\n", r, g, b);

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