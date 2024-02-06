#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t* triangles_to_render = NULL;;

vec3_t camera_position = {0.0f, 0.0f, 0.0f};
float fov_factor = 640.0f;

bool is_running = false;
int previous_frame_time = 0;

void setup(void)
{
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height);

    load_cube_mesh_data();
    // load_obj_file_data("../Assets/Plane.obj");
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
            if (event.key.keysym.sym == SDLK_ESCAPE) is_running = false;
            if (event.key.keysym.sym == SDLK_1) render_method = RENDER_WIRE_VERTEX;
            if (event.key.keysym.sym == SDLK_2) render_method = RENDER_WIRE;
            if (event.key.keysym.sym == SDLK_3) render_method = RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4) render_method = RENDER_FILL_TRIANGLE_WIRE;
            if (event.key.keysym.sym == SDLK_c) cull_method = CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_d) cull_method = CULL_NONE;
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
    const int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    triangles_to_render = NULL;

    mesh.rotation.x += 0.02f;
    mesh.rotation.y += 0.00f;
    mesh.rotation.z += 0.00f;

    for (int i = 0; i < array_length(mesh.faces); i++)
    {
        const face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            transformed_vertex.z += 5.0f;

            transformed_vertices[j] = transformed_vertex;
        }

        if (cull_method == CULL_BACKFACE)
        {
            const vec3_t vector_a = transformed_vertices[0];
            const vec3_t vector_b = transformed_vertices[1];
            const vec3_t vector_c = transformed_vertices[2];

            vec3_t vector_ab = vec3_sub(vector_b, vector_a);
            vec3_t vector_ac = vec3_sub(vector_c, vector_a);
            vec3_normalize(&vector_ab);
            vec3_normalize(&vector_ac);

            vec3_t normal = vec3_cross(vector_ab, vector_ac);
            vec3_normalize(&normal);

            const vec3_t camera_ray = vec3_sub(camera_position, vector_a);

            const float dot_normal_camera = vec3_dot(normal, camera_ray);

            if (dot_normal_camera < 0.0f) continue;
        }

        vec2_t projected_points[3];

        for (int k = 0; k < 3; k++)
        {
            projected_points[k] = project(transformed_vertices[k]);

            projected_points[k].x += (window_width / 2);
            projected_points[k].y += (window_height / 2);
        }

        triangle_t projected_triangle =
                {
                        .points =
                                {
                                        {projected_points[0].x, projected_points[0].y},
                                        {projected_points[1].x, projected_points[1].y},
                                        {projected_points[2].x, projected_points[2].y}
                                },
                        .color = mesh_face.color
                };

        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void)
{
    draw_grid();

    // Loop all projected triangles and render them
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            // Draw filled triangle
            draw_filled_triangle(
                    triangle.points[0].x, triangle.points[0].y, // vertex A
                    triangle.points[1].x, triangle.points[1].y, // vertex B
                    triangle.points[2].x, triangle.points[2].y, // vertex C
                    triangle.color
            );
        }

        if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX ||
            render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            // Draw unfilled triangle
            draw_triangle(
                    triangle.points[0].x, triangle.points[0].y, // vertex A
                    triangle.points[1].x, triangle.points[1].y, // vertex B
                    triangle.points[2].x, triangle.points[2].y, // vertex C
                    0xFFFFFFFF
            );
        }

        if (render_method == RENDER_WIRE_VERTEX)
        {
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFF00FF00);
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFF00FF00);
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFF00FF00);
        }
    }

    // Clear the array of triangles to render every frame loop
    array_free(triangles_to_render);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void free_resources(void)
{
    array_free(mesh.faces);
    array_free(mesh.vertices);
    free(color_buffer);
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

    free_resources();

    return 0;
}
