#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

typedef struct
{
    vec3_t position;
    vec3_t direction;
    vec3_t forward_velocity;
    float yaw;
    float pitch;
} camera_t;

void init_camera(vec3_t position, vec3_t direction);
void translate_camera(vec3_t v);
void translate_camera_forward(float value);
void rotate_camera_y(float value);
void rotate_camera_x(float value);
void update_camera_direction(vec3_t target);
vec3_t get_camera_position(void);
vec3_t get_camera_direction(void);

#endif //CAMERA_H
