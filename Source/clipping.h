#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"

enum
{
    LEFT_FRUSTRUM_PLANE,
    RIGHT_FRUSTRUM_PLANE,
    TOP_FRUSTRUM_PLANE,
    BOTTOM_FRUSTRUM_PLANE,
    NEAR_FRUSTRUM_PLANE,
    FAR_FRUSTRUM_PLANE
};

typedef struct
{
    vec3_t point;
    vec3_t normal;
} plane_t;

#endif //CLIPPING_H
