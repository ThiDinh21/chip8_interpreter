#include "graphics.h"

void initSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8 Interpreter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
}

void destroySDL(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void cls(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

u8 getPixel(SDL_Surface *surface, u8 x, u8 y)
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

// u8 drawSprite(SDL_Renderer *renderer, SDL_Surface *surface, u8 *sprite, u8 n, u8 x, u8 y)
// {
//     u8 isCollided = 0;
//     for (size_t row = 0; row < n; row++)
//     {
//         u8 spriteRow = sprite[row];
//         for (size_t bit = 0; bit < 8; bit++)
//         {
//             const u8 spritePixel = spriteRow & 0b00000001;
//             const u8 pixel = getPixel(surface, x + bit, y + row);
//             const u8 color = pixel ^ spritePixel;

//             if (pixel == 1)
//                 isCollided = 1;

//             SDL_SetRenderDrawColor(renderer, color, color, color, 255);
//             SDL_RenderDrawPoint(renderer, (x + bit) % 64, (y + row) % 32);

//             spriteRow = spriteRow >> 1;
//         }
//     }

//     return isCollided;
// }