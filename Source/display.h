#pragma once

#include <SDL.h>
#include <stdbool.h>

#include "triangle.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

bool initialize_window(void);
void render_color_buffer(void);
void draw_pixel(const int x, const int y, const uint32_t color);
void draw_grid(void);
void draw_dots(void);
void draw_line(const int x0, const int y0, const int x1, const int y1, const uint32_t color);
void draw_triangle(const triangle_t* triangle, const uint32_t color);
void draw_rect(const int x, const int y, const int width, const int height, const uint32_t color);
void clear_color_buffer(const uint32_t color);
void destroy_window(void);