#ifndef RENDERENGINE_LIGHT_H
#define RENDERENGINE_LIGHT_H

#include "vector.h"
#include "stdint.h"

typedef struct
{
    vec3_t direction;
} light_t;

extern light_t light;

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

#endif //RENDERENGINE_LIGHT_H