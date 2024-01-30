#include <math.h>
#include "vector.h"

float vec2_length(const vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(const vec2_t a, const vec2_t b)
{
    const vec2_t result = {a.x + b.x, a.y + b.y};
    return result;
}

vec2_t vec2_sub(const vec2_t a, const vec2_t b)
{
    const vec2_t result = {a.x - b.x, a.y - b.y};
    return result;
}

vec2_t vec2_mul(const vec2_t v, const float factor)
{
    const vec2_t result = {v.x * factor, v.y * factor};
    return result;
}

vec2_t vec2_div(const vec2_t v, const float factor)
{
    const vec2_t result = {v.x / factor, v.y / factor};
    return result;
}

float vec2_dot(const vec2_t a, const vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

float vec3_length(const vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(const vec3_t a, const vec3_t b)
{
    const vec3_t result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3_t vec3_sub(const vec3_t a, const vec3_t b)
{
    const vec3_t result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3_t vec3_mul(const vec3_t v, const float factor)
{
    const vec3_t result = {v.x * factor, v.y * factor, v.z * factor};
    return result;
}

vec3_t vec3_div(const vec3_t v, const float factor)
{
    const vec3_t result = {v.x / factor, v.y / factor, v.z / factor};
    return result;
}

vec3_t vec3_cross(const vec3_t a, const vec3_t b)
{
    const vec3_t result =
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

float vec3_dot(const vec3_t a, const vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;;
}

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
