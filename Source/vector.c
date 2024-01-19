//
// Created by zerof on 1/13/2024.
//

#include <math.h>
#include "vector.h"

vec3_t vec3_rotate_x(const vec3_t v, const float angle)
{
    const vec3_t rotated_vector =
        {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
        };

    return rotated_vector;
}

vec3_t vec3_rotate_y(const vec3_t v, const float angle)
{
    const vec3_t rotated_vector =
        {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
        };

    return rotated_vector;
}

vec3_t vec3_rotate_z(const vec3_t v, const float angle)
{
    const vec3_t rotated_vector =
        {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
        };

    return rotated_vector;
}
