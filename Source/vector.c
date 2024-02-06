#include <math.h>
#include "vector.h"

float vec2_length(vec2_t v)
{
    float length = sqrt(v.x * v.x + v.y * v.y);
    return length;
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
    const vec2_t result = {a.x + b.x, a.y + b.y};
    return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    const vec2_t result = {a.x - b.x, a.y - b.y};
    return result;
}

vec2_t vec2_mul(vec2_t v, float factor)
{
    const vec2_t result = {v.x * factor, v.y * factor};
    return result;
}

vec2_t vec2_div(vec2_t v, float factor)
{
    const vec2_t result = {v.x / factor, v.y / factor};
    return result;
}

void vec2_normalize(vec2_t* v)
{
    const float length = vec2_length(*v);

    v->x = v->x / length;
    v->y = v->y / length;
}

float vec2_dot(vec2_t a, vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

float vec3_length(vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    const vec3_t result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    const vec3_t result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3_t vec3_mul(vec3_t v, float factor)
{
    const vec3_t result = {v.x * factor, v.y * factor, v.z * factor};
    return result;
}

vec3_t vec3_div(vec3_t v, float factor)
{
    const vec3_t result = {v.x / factor, v.y / factor, v.z / factor};
    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    const vec3_t result =
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

void vec3_normalize(vec3_t* v)
{
    const float length = vec3_length(*v);

    v->x = v->x / length;
    v->y = v->y / length;
    v->z = v->z / length;
}

float vec3_dot(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;;
}

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
    const vec3_t rotated_vector =
    {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
    const vec3_t rotated_vector =
    {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
    const vec3_t rotated_vector =
    {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };

    return rotated_vector;
}