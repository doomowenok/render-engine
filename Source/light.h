#ifndef RENDERENGINE_LIGHT_H
#define RENDERENGINE_LIGHT_H

#include "vector.h"
#include "stdint.h"

typedef struct
{
    vec3_t direction;
} light_t;

void init_light(vec3_t direction);
vec3_t get_light_direction(void);
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

#endif //RENDERENGINE_LIGHT_H