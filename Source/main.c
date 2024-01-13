#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];

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

    int point_count = 0;
    for (float x = -1.0f; x <= 1.0f; x += 0.25f)
    {
        for (float y = -1.0f; y <= 1.0f; y += 0.25f)
        {
            for (float z = -1.0f; z <= 1.0f; z += 0.25f)
            {
                const vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count] = new_point;
                point_count++;
            }
        }
    }
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

void update(void)
{
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw_grid();
    draw_dots();
    draw_rect(300, 300, 200, 200, 0xFF00FF);
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
