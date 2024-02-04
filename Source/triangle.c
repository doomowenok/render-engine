#include "triangle.h"

void sort_by_increase_y(triangle_t* triangle)
{
    if(triangle->points[0].y > triangle->points[1].y)
    {
        float local_y0 = triangle->points[0].y;
        float local_y1 = triangle->points[1].y;

        float local_x0 = triangle->points[0].x;
        float local_x1 = triangle->points[1].x;

        triangle->points[0].y = local_y1;
        triangle->points[0].x = local_x1;

        triangle->points[1].y = local_y0;
        triangle->points[1].x = local_x0;
    }

    if(triangle->points[1].y > triangle->points[2].y)
    {
        float local_y0 = triangle->points[1].y;
        float local_y1 = triangle->points[2].y;

        float local_x0 = triangle->points[1].x;
        float local_x1 = triangle->points[2].x;

        triangle->points[1].y = local_y1;
        triangle->points[1].x = local_x1;

        triangle->points[2].y = local_y0;
        triangle->points[2].x = local_x0;
    }

    if(triangle->points[0].y > triangle->points[1].y)
    {
        float local_y0 = triangle->points[0].y;
        float local_y1 = triangle->points[1].y;

        float local_x0 = triangle->points[0].x;
        float local_x1 = triangle->points[1].x;

        triangle->points[0].y = local_y1;
        triangle->points[0].x = local_x1;

        triangle->points[1].y = local_y0;
        triangle->points[1].x = local_x0;
    }
}

void draw_filled_triangle(triangle_t* triangle, uint32_t color)
{
    triangle_t sort_triangle;

    for(int i = 0; i < 3; i++)
    {
        sort_triangle.points[i] = triangle->points[i];
    }

    sort_by_increase_y(&sort_triangle);

    float division_y = sort_triangle.points[1].y;
    float division_x =
            (((sort_triangle.points[2].x - sort_triangle.points[0].x) *
            (sort_triangle.points[1].y - sort_triangle.points[0].y)) /
            (sort_triangle.points[2].y - sort_triangle.points[0].y)) + triangle->points[0].x;

    triangle_t top_triangle;

    top_triangle.points[0].x = sort_triangle.points[0].x;
    top_triangle.points[0].y = sort_triangle.points[0].y;

    top_triangle.points[1].x = sort_triangle.points[1].x;
    top_triangle.points[1].y = sort_triangle.points[1].y;

    top_triangle.points[2].x = division_x;
    top_triangle.points[2].y = division_y;

    triangle_t bottom_triangle;

    bottom_triangle.points[0].x = sort_triangle.points[1].x;
    bottom_triangle.points[0].y = sort_triangle.points[1].y;

    bottom_triangle.points[1].x = division_x;
    bottom_triangle.points[1].y = division_y;

    bottom_triangle.points[2].x = sort_triangle.points[2].x;
    bottom_triangle.points[2].y = sort_triangle.points[2].y;

    fill_flat_bottom_triangle();
    fill_flat_top_triangle();
}
