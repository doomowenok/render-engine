#include <stdio.h>
#include <stdint.h>
#include "texture.h"
#include "upng.h"

static int texture_width = 0;
static int texture_height = 0;

static upng_t* png_texture = NULL;
static uint32_t* mesh_texture = NULL;

void load_png_texture_data(char* filename)
{
    png_texture = upng_new_from_file(filename);

    if (png_texture != NULL)
    {
        upng_decode(png_texture);
        if(upng_get_error(png_texture) == UPNG_EOK)
        {
            mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
            texture_width = upng_get_width(png_texture);
            texture_height = upng_get_height(png_texture);
        }
    }
}

tex2_t tex2_clone(tex2_t* t)
{
    tex2_t result = { .u = t->u, .v = t->v};
    return result;
}

uint32_t* get_mesh_texture(void)
{
    return mesh_texture;
}

void free_texture(void)
{
    upng_free(png_texture);
}

int get_texture_width(void)
{
    return texture_width;
}

int get_texture_height(void)
{
    return texture_height;
}
