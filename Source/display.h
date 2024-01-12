//
// Created by zerof on 1/12/2024.
//

#pragma once

#include <SDL.h>
#include <stdbool.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

bool initialize_window(void);
void render_color_buffer(void);
void draw_grid(void);
void draw_dots(void);
void clear_color_buffer(const uint32_t color);
void destroy_window(void);