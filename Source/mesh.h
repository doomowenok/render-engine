#pragma once

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERICES 8
extern vec3_t mesh_verices[N_MESH_VERICES];

#define N_MESH_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t mesh_faces[N_MESH_FACES];