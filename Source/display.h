#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)
#define SCREEN_FACTOR 1

enum cull_method
{
    CULL_NONE,
    CULL_BACKFACE
};

enum render_method
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
};

bool initialize_window(void);

int get_window_width(void);

int get_window_height(void);

void set_render_method(int method);

int get_render_method(void);

void set_cull_method(int method);

int get_cull_method(void);

bool is_cull_backface(void);

bool should_render_filled_triangles(void);

bool should_render_textured_triangles(void);

bool should_render_wireframe(void);

bool should_render_wire_vertex(void);

void draw_grid(void);

void draw_pixel(int x, int y, uint32_t color);

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void draw_rect(int x, int y, int width, int height, uint32_t color);

void render_color_buffer(void);

void clear_color_buffer(uint32_t color);

void clear_z_buffer();

float get_z_buffer_at(int x, int y);
void update_z_buffer_at(int x, int y, float value);

void destroy_window(void);

#endif
