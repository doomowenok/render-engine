#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = { 0, 0, -5.0f };

float fov_factor = 640.0f;

bool is_running = false;

void setup(void)
{
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

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

vec2_t project(const vec3_t point)
{
    const vec2_t projected_point =
        {
            (fov_factor * point.x) / point.z,
            (fov_factor * point.y) / point.z
        };
    return projected_point;
}

void update(void)
{
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t cube_point = cube_points[i];

        cube_point.z -= camera_position.z;

        vec2_t projected_point = project(cube_point);
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    draw_dots();

    const int translate_x = window_width / 2;
    const int translate_y = window_height / 2;

    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        draw_rect(projected_point.x + translate_x, projected_point.y + translate_y, 4, 4, 0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

#if WIN32
int SDL_main(int argc, char* argv[])
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
