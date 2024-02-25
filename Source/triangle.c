#include "display.h"
#include "swap.h"
#include "triangle.h"

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Find the two slopes (two triangle legs)
    float inv_slope_1 = (float) (x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float) (x2 - x0) / (y2 - y0);

    // Start x_start and x_end from the top vertex (x0,y0)
    float x_start = x0;
    float x_end = x0;

    // Loop all the scan-lines from top to bottom
    for (int y = y0; y <= y2; y++)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Find the two slopes (two triangle legs)
    float inv_slope_1 = (float) (x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float) (x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the bottom vertex (x2,y2)
    float x_start = x2;
    float x_end = x2;

    // Loop all the scanlines from bottom to top
    for (int y = y2; y >= y0; y--)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1)
    {
        // Draw flat-top triangle
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else
    {
        // Calculate the new vertex (Mx,My) using triangle similarity
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

        // Draw flat-top triangle
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a textured triangle based on a texture array of colors.
// We split the original triangle in two, half flat-bottom and half flat-top.
///////////////////////////////////////////////////////////////////////////////
//
//        v0
//        /\
//       /  \
//      /    \
//     /      \
//   v1--------\
//     \_       \
//        \_     \
//           \_   \ 
//              \_ \
//                 \\
//                   \
//                    v2
//
///////////////////////////////////////////////////////////////////////////////
void draw_textured_triangle(int x0, int y0, float u0, float v0,
                            int x1, int y1, float u1, float v1,
                            int x2, int y2, float u2, float v2,
                            uint32_t* texture)
{
    // We need to sort the vertices by the Y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }

    ///////////////////////////////////////////////////////
    // Render the upper pert of the triangle (flat-bottom)
    ///////////////////////////////////////////////////////
    float inverse_slope_1 = 0;
    float inverse_slope_2 = 0;

    if(y1 - y0 != 0)
    {
        inverse_slope_1 = (float)(x1 - x0) / (float)abs((y1 - y0));
    }

    if(y2 - y0 != 0)
    {
        inverse_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));
    }

    if(y1 - y0 != 0)
    {
        for(int y = y0; y <= y1; y++)
        {
            int x_start = x1 + (y - y1) * inverse_slope_1;
            int x_end = x0 + (y - y0) * inverse_slope_2;

            // Swap if x_start is to the right of x_end
            if(x_start > x_end)
            {
                int_swap(&x_start, &x_end);
            }

            for(int x = x_start; x < x_end; x++)
            {
                // Draw our pixel with the color that comes from the texture
                draw_pixel(x, y, 0xFFFF00FF);
            }
        }
    }

    ///////////////////////////////////////////////////////
    // Render the bottom pert of the triangle (flat-top)
    ///////////////////////////////////////////////////////

    inverse_slope_1 = 0;
    inverse_slope_2 = 0;

    if(y2 - y1 != 0)
    {
        inverse_slope_1 = (float)(x2 - x1) / (float)abs((y2 - y1));
    }

    if(y2 - y0 != 0)
    {
        inverse_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));
    }

    if(y2 - y1 != 0)
    {
        for(int y = y1; y <= y2; y++)
        {
            int x_start = x1 + (y - y1) * inverse_slope_1;
            int x_end = x0 + (y - y0) * inverse_slope_2;

            // Swap if x_start is to the right of x_end
            if(x_start > x_end)
            {
                int_swap(&x_start, &x_end);
            }

            for(int x = x_start; x < x_end; x++)
            {
                // Draw our pixel with the color that comes from the texture
                draw_pixel(x, y, 0xFFFF00FF);
            }
        }
    }
}
