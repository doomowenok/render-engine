//
// Created by zerof on 1/12/2024.
//
#include <stdio.h>
#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_height = display_mode.h;
    window_width = display_mode.w;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(const int x, const int y, const uint32_t color)
{
    if (x < window_width && y < window_height)
    {
        color_buffer[(window_width * y) + x] = color;
    }
}

void draw_grid(void)
{
    for(int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            if(y % 10 == 0 || x % 10 == 0)
            {
                draw_pixel(x, y, 0xFF333333);
            }
        }
    }
}

void draw_dots(void)
{
    for(int y = 0; y < window_height; y += 10)
    {
        for (int x = 0; x < window_width; x += 10)
        {
            draw_pixel(x, y, 0xFF333333);
        }
    }
}

void draw_rect(const int x, const int y, const int width, const int height, const uint32_t color)
{
    for(int i = 0; i < width; i++)
    {
        for (int k = 0; k < height; k++)
        {
            const int current_x = x + i;
            const int current_y = y + k;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void clear_color_buffer(const uint32_t color)
{
    for(int y = 0; y < window_height; y++)
    {
        for(int x = 0; x < window_width; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}