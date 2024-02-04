#pragma once

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

float vec2_length(const vec2_t v);
vec2_t vec2_add(const vec2_t a, const vec2_t b);
vec2_t vec2_sub(const vec2_t a, const vec2_t b);
vec2_t vec2_mul(const vec2_t v, const float factor);
vec2_t vec2_div(const vec2_t v, const float factor);
void vec2_normalize(vec2_t* v);
float vec2_dot(const vec2_t a, const vec2_t b);

float vec3_length(const vec3_t v);
vec3_t vec3_add(const vec3_t a, const vec3_t b);
vec3_t vec3_sub(const vec3_t a, const vec3_t b);
vec3_t vec3_mul(const vec3_t v, const float factor);
vec3_t vec3_div(const vec3_t v, const float factor);
vec3_t vec3_cross(const vec3_t a, const vec3_t b);
void vec3_normalize(vec3_t* v);
float vec3_dot(const vec3_t a, const vec3_t b);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);