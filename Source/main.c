#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "upng.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
#include "clipping.h"

#define MAX_TRIANGLES_PER_MESH 10000

triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0;

///////////////////////////////////////////////////////////////////////////////
// Declaration of our global transformation matrices
///////////////////////////////////////////////////////////////////////////////
mat4_t world_matrix;
mat4_t projection_matrix;
mat4_t view_matrix;

///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
void setup(void)
{
    // Initialize render mode and triangle culling method
    set_render_method(RENDER_WIRE);
    set_cull_method(CULL_BACKFACE);

    // Initialize perspective projection matrix
    float aspect_ratio_x = (float) get_window_width() / (float) get_window_height();
    float aspect_ratio_y = (float) get_window_height() / (float) get_window_width();
    float fov_y = M_PI / 3.0f; // 60.0 degrees in radians
    float fov_x = atan(tan(fov_y / 2.0f) * aspect_ratio_x) * 2.0f; // 60.0 degrees in radians
    float z_near = 0.1f;
    float z_far = 100.0f;
    projection_matrix = mat4_make_perspective(fov_y, aspect_ratio_y, z_near, z_far);

    // Initialize camera
    init_camera(vec3_new(0,0,0), vec3_new(0,0,1));

    // Initialize light
    init_light(vec3_new(0, 0, 1));

    // Initialize frustum planes with a point and a normal
    init_frustum_planes(fov_x, fov_y, z_near, z_far);

    //
    load_mesh("./Assets/Cube.obj",
              "./Assets/Cube.png",
              vec3_new(1, 1, 1),
              vec3_new(3, 0, 5),
              vec3_new(0, 0, 0));

    load_mesh("./Assets/Cube.obj",
              "./Assets/Cube.png",
              vec3_new(1, 1, 1),
              vec3_new(-3, 0, 5),
              vec3_new(0, 0, 0));
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_running = false;
                    break;
                }
                if (event.key.keysym.sym == SDLK_1)
                {
                    set_render_method(RENDER_WIRE_VERTEX);
                    break;
                }
                if (event.key.keysym.sym == SDLK_2)
                {
                    set_render_method(RENDER_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_3)
                {
                    set_render_method(RENDER_FILL_TRIANGLE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_4)
                {
                    set_render_method(RENDER_FILL_TRIANGLE_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_5)
                {
                    set_render_method(RENDER_TEXTURED);
                    break;
                }
                if (event.key.keysym.sym == SDLK_6)
                {
                    set_render_method(RENDER_TEXTURED_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_c)
                {
                    set_cull_method(CULL_BACKFACE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_x)
                {
                    set_cull_method(CULL_NONE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    translate_camera(vec3_new(0, 3.0f * delta_time, 0));
                    break;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    translate_camera(vec3_new(0, -3.0f * delta_time, 0));
                    break;
                }
                if (event.key.keysym.sym == SDLK_a)
                {
                    rotate_camera_y(-0.6f * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_d)
                {
                    rotate_camera_y(0.6f * delta_time);
                    break;
                }
                if(event.key.keysym.sym == SDLK_q)
                {
                    rotate_camera_x(0.6f * delta_time);
                    break;
                }
                if(event.key.keysym.sym == SDLK_e)
                {
                    rotate_camera_x(-0.6f * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_w)
                {
                    translate_camera_forward(5.0f * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_s)
                {
                    translate_camera_forward(-5.0f * delta_time);
                    break;
                }
                break;
            default:
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Process the graphics pipeline stages for all the mesh triangles
///////////////////////////////////////////////////////////////////////////////
// +-------------+
// | Model space |  <-- original mesh vertices
// +-------------+
// |   +-------------+
// `-> | World space |  <-- multiply by world matrix
//     +-------------+
//     |   +--------------+
//     `-> | Camera space |  <-- multiply by view matrix
//         +--------------+
//         |    +------------+
//         `--> |  Clipping  |  <-- clip against the six frustum planes
//              +------------+
//              |    +------------+
//              `--> | Projection |  <-- multiply by projection matrix
//                   +------------+
//                   |    +-------------+
//                   `--> | Image space |  <-- apply perspective divide
//                        +-------------+
//                        |    +--------------+
//                        `--> | Screen space |  <-- ready to render
//                             +--------------+
///////////////////////////////////////////////////////////////////////////////
void process_graphics_pipeline_stages(mesh_t* mesh)
{
    // Initialize the target looking at the positive z-axis
    vec3_t target = {0, 0, 1};
    update_camera_direction(target);

    // Offset the camera position in the direction where the camera is pointing at
    target = vec3_add(get_camera_position(), get_camera_direction());
    vec3_t up_direction = {0, 1, 0};

    // Create the view matrix
    view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

    // Create a scale, translation and rotation matrices that will be used to multiply the mesh vertices
    mat4_t scale_matrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh->translation.x, mesh->translation.y, mesh->translation.z);
    mat4_t rotation_x_matrix = mat4_make_rotation_x(mesh->rotation.x);
    mat4_t rotation_y_matrix = mat4_make_rotation_y(mesh->rotation.y);
    mat4_t rotation_z_matrix = mat4_make_rotation_z(mesh->rotation.z);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh->faces);
    for (int i = 0; i < num_faces; i++)
    {
        face_t mesh_face = mesh->faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh->vertices[mesh_face.a];
        face_vertices[1] = mesh->vertices[mesh_face.b];
        face_vertices[2] = mesh->vertices[mesh_face.c];

        vec4_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++)
        {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Create a World Matrix combining scale, rotation, and translation matrices
            world_matrix = mat4_identity();

            // Order matters: First scale, then rotate, then translate. [T] * [R] * [S] * v - read inverse!
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_z_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_y_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_x_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Multiply the world matrix by the original vector
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Multiply the view matrix by the vector to transform the scene to camera space
            transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[j] = transformed_vertex;
        }

        // Calculate the triangle face normal
        vec3_t face_normal = get_triangle_normal(transformed_vertices);

        // Backface culling test to see if the current face should be projected
        if (is_cull_backface())
        {
            // Find the vector between vertex A in the triangle and the camera origin
            vec3_t origin = {0, 0, 0};
            vec3_t camera_ray = vec3_sub(origin, vec3_from_vec4(transformed_vertices[0]));

            // Calculate how aligned the camera ray is with the face normal (using dot product)
            float dot_normal_camera = vec3_dot(face_normal, camera_ray);

            // Bypass the triangles that are looking away from the camera
            if (dot_normal_camera < 0)
            {
                continue;
            }
        }

        // Create a polygon from the original transformed triangle to be clipped
        polygon_t polygon = create_polygon_from_triangle
                (
                        vec3_from_vec4(transformed_vertices[0]),
                        vec3_from_vec4(transformed_vertices[1]),
                        vec3_from_vec4(transformed_vertices[2]),
                        mesh_face.a_uv,
                        mesh_face.b_uv,
                        mesh_face.c_uv
                );

        // Clip the polygon and returns a new polygon with potential new vertices
        clip_polygon(&polygon);

        // Break the clipped polygon apart back into individual triangle
        triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
        int num_triangles_after_clipping = 0;

        triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

        // Loops all the assembled triangles after clipping
        for (int t = 0; t < num_triangles_after_clipping; t++)
        {
            triangle_t triangle_after_clipping = triangles_after_clipping[t];

            vec4_t projected_points[3];

            // Loop all three vertices to perform projection
            for (int j = 0; j < 3; j++)
            {
                // Project the current vertex
                projected_points[j] = mat4_mul_vec4_project(projection_matrix, triangle_after_clipping.points[j]);

                // Scale into the view
                projected_points[j].x *= (float) get_window_width() / 2.0f;
                projected_points[j].y *= (float) get_window_height() / 2.0f;

                // Invert the y values to account for flipped screen y coordinate
                projected_points[j].y *= -1.0f;

                // Translate the projected points to the middle of the screen
                projected_points[j].x += ((float)get_window_width() / 2.0f);
                projected_points[j].y += ((float)get_window_height() / 2.0f);
            }

            // Calculate the shade intensity based on how aligned the face normal and light direction
            float light_intensity_factor = -vec3_dot(face_normal, get_light_direction());

            // Calculate the triangle color based in light angle
            uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

            triangle_t triangle_to_render =
                    {
                            .points =
                                    {
                                            {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
                                            {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
                                            {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w},
                                    },
                            .tex_coords =
                                    {
                                            {triangle_after_clipping.tex_coords[0].u, triangle_after_clipping.tex_coords[0].v},
                                            {triangle_after_clipping.tex_coords[1].u, triangle_after_clipping.tex_coords[1].v},
                                            {triangle_after_clipping.tex_coords[2].u, triangle_after_clipping.tex_coords[2].v},
                                    },
                            .color = triangle_color,
                            .texture = mesh->texture
                    };

            // Save the projected triangle in the array of triangles to render
            if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
            {
                triangles_to_render[num_triangles_to_render] = triangle_to_render;
                num_triangles_to_render++;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void)
{
    // Wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    // Get a delta time factor converted to seconds to be used to update our game objects
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;

    previous_frame_time = SDL_GetTicks();

    // Initialize the counter of triangles to render for the current frame
    num_triangles_to_render = 0;

    // Loop all the meshes of our scene
    for(int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++)
    {
        mesh_t* mesh = get_mesh(mesh_index);
        // Change the mesh scale/rotation/translation values per animation frame
        // mesh.rotation.x += 1.0f * delta_time;
        // mesh.rotation.y += 1.0f * delta_time;
        // mesh.rotation.z += 1.0f * delta_time;
        // mesh.translation.z = 5.0f;

        process_graphics_pipeline_stages(mesh);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void)
{
    // Clear all the array to get ready for the next frame
    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    draw_grid();

    // Loop all projected triangles and render them
    for (int i = 0; i < num_triangles_to_render; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        // Draw filled triangle
        if (should_render_filled_triangles())
        {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, // vertex C
                triangle.color
            );
        }

        // Draw textured triangle
        if (should_render_textured_triangles())
        {
            draw_textured_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
                triangle.tex_coords[0].u, triangle.tex_coords[0].v, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
                triangle.tex_coords[1].u, triangle.tex_coords[1].v, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
                triangle.tex_coords[2].u, triangle.tex_coords[2].v, // vertex C
                triangle.texture
            );
        }

        // Draw triangle wireframe
        if (should_render_wireframe())
        {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFFFFFFFF
            );
        }

        // Draw triangle vertex points
        if (should_render_wire_vertex())
        {
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000); // vertex A
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000); // vertex B
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000); // vertex C
        }
    }

    render_color_buffer();
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void)
{
    free_meshes();
    destroy_window();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
#if WIN32

int SDL_main(int argc, char* argv[])
#else

int main(void)
#endif
{
    is_running = initialize_window();

    setup();

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
