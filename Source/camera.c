#include "camera.h"

static camera_t camera;

void init_camera(vec3_t position, vec3_t direction)
{
    camera.position = position;
    camera.direction = direction;
    camera.forward_velocity = vec3_new(0, 0, 0);
    camera.yaw = 0.0f;
    camera.pitch = 0.0f;
}

void translate_camera(vec3_t v)
{
    camera.position = vec3_add(camera.position, v);
}

void translate_camera_forward(float value)
{
    vec3_t direction = vec3_mul(camera.direction, value);
    translate_camera(direction);
}

void rotate_camera_y(float value)
{
    camera.yaw += value;
}

void rotate_camera_x(float value)
{
    camera.pitch += value;
}

void update_camera_direction(vec3_t target)
{
    mat4_t camera_rotation = mat4_mul_mat4(
            mat4_make_rotation_y(camera.yaw),
            mat4_make_rotation_x(camera.pitch));
    camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_rotation, vec4_from_vec3(target)));
}

vec3_t get_camera_position(void)
{
    return camera.position;
}

vec3_t get_camera_direction(void)
{
    return camera.direction;
}
