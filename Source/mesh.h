#pragma once

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern vec3_t cube_verices[N_CUBE_VERICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct
{
    vec3_t* vertices;
    face_t* faces;
    vec3_t rotation;
} mesh_t;


extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file_data(const char* file_path);