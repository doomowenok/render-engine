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

#endif //RENDERENGINE_TEXTURE_H
