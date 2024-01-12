#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"

bool is_running = false;

void setup(void)
{
    color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
        default:
            break;
    }
}

void update(void) {}

void draw_rect(const int x, const int y, const int width, const int height, const uint32_t color)
{
    for(int i = 0; i < width; i++)
    {
        for (int k = 0; k < height; k++)
        {
            const int current_x = x + i;
            const int current_y = y + k;
            color_buffer[(window_width * current_y) + current_x] = color;
        }
    }
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw_grid();
    draw_dots();
    draw_rect(300, 300, 200,200, 0xFF00FF);
    render_color_buffer();
    clear_color_buffer(0xFFFFFFFF);

    SDL_RenderPresent(renderer);
}

#if WIN32
int SDL_main(int argc, char *argv[])
#else
int main(void)
#endif
{
    is_running = initialize_window();

    setup();

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}