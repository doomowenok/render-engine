#include "math.h"
#include "matrix.h"

mat4_t mat4_identity(void)
{
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
    mat4_t m =
            {
                    {
                            {1, 0, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 1}
                    }
            };

    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    // | sx  0  0 0 |
    // |  0 sy  0 0 |
    // |  0  0 sz 0 |
    // |  0  0  0 1 |
    mat4_t m = mat4_identity();

    m.m[0][0]= sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;

    return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
    vec4_t result;

    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

    return result;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    mat4_t result = mat4_identity();

    result.m[0][3] = tx;
    result.m[1][3] = ty;
    result.m[2][3] = tz;

    return result;
}

mat4_t mat4_make_rotation_z(float angle)
{
    // | c -s  0  0 |
    // | s  c  0  0 |
    // | 0  0  1  0 |
    // | 0  0  0  1 |
    float c = cos(angle);
    float s = sin(angle);
    mat4_t result = mat4_identity();

    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;

    return result;
}

mat4_t mat4_make_rotation_x(float angle)
{
    // | 1  0  0  0 |
    // | 0  c -s  0 |
    // | 0  s  c  0 |
    // | 0  0  0  1 |
    float c = cos(angle);
    float s = sin(angle);
    mat4_t result = mat4_identity();

    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;

    return result;
}

mat4_t mat4_make_rotation_y(float angle)
{
    // |  c  0  s  0 |
    // |  0  1  0  0 |
    // | -s  0  c  0 |
    // |  0  0  0  1 |
    float c = cos(angle);
    float s = sin(angle);
    mat4_t result = mat4_identity();

    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;

    return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
    mat4_t m;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }

    return m;
}

mat4_t mat4_make_perspective(float field_of_view, float aspect_ratio, float z_near, float z_far)
{
    // | (aspect_ration) * (1 / tan(field_of_view / 2))                           0                         0                                     0 |
    // |                                              0  1 / tan(field_of_view / 2)                         0                                     0 |
    // |                                              0                           0  z_far / (z_far - z_near)  (-z_far * z_near) / (z_far - z_near) |
    // |                                              0                           0                         1                                     0 |
    mat4_t m = mat4_identity();

    m.m[0][0] = aspect_ratio * (1 / tan(field_of_view / 2));
    m.m[1][1] = 1 / tan(field_of_view / 2);
    m.m[2][2] = z_far / (z_far - z_near);
    m.m[2][3] = (-z_far * z_near) / (z_far - z_near);
    m.m[3][2] = 1;
    m.m[3][3] = 0;

    return m;
}

vec4_t mat4_mul_vec4_project(mat4_t projection_matrix, vec4_t v)
{
    // multiply the projection matrix by our original vector
    vec4_t result = mat4_mul_vec4(projection_matrix, v);

    // perform perspective divide with original z-value that is now stored in w
    if(result.w != 0.0f)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up)
{
    // Compute the forward (z), right (x), and up (y) vectors
    vec3_t z = vec3_sub(target, eye);
    vec3_normalize(&z);
    vec3_t x = vec3_cross(up, z);
    vec3_normalize(&x);
    vec3_t y = vec3_cross(z, x);

    // | x.x   x.y   x.z  -dot(x,eye) |
    // | y.x   y.y   y.z  -dot(y,eye) |
    // | z.x   z.y   z.z  -dot(z,eye) |
    // |   0     0     0            1 |
    mat4_t view_matrix = {{
        { x.x, x.y, x.z, -vec3_dot(x, eye) },
        { y.x, y.y, y.z, -vec3_dot(y, eye) },
        { z.x, z.y, z.z, -vec3_dot(z, eye) },
        {   0,   0,   0,                     1 }
    }};
    return view_matrix;
}
