#ifndef RENDERENGINE_TEXTURE_H
#define RENDERENGINE_TEXTURE_H

#include "stdint.h"
#include "upng.h"

typedef struct
{
    float u;
    float v;
} tex2_t;

tex2_t tex2_clone(tex2_t* t);
void load_png_texture_data(char* filename);
uint32_t* get_mesh_texture(void);
int get_texture_width(void);
int get_texture_height(void);
void free_texture(void);

#endif //RENDERENGINE_TEXTURE_H
