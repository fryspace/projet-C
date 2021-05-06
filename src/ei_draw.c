//
// Created by bossu on 05/05/2021.
//
#include "ei_draw.h"
#include "ei_types.h"
#include "hw_interface.h"
#include <math.h>

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))


uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color){
    int ib=0;
    int ia=0;
    int ir=0;
    int ig=0;

    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t result =0;
    result+=(color.alpha)<<((ia)*8);
    result+=(color.blue)<<((ib)*8);
    result+=(color.green)<<((ig)*8);
    result+=(color.red)<<((ir)*8);
    return result;
}

void ei_draw_pixel(ei_surface_t surface, ei_color_t color,int x, int y, int width){
    uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface) + y*width + x;
    *pixel_ptr= ei_map_rgba(surface, color);
}

void ei_draw_polyline	(ei_surface_t surface, const ei_linked_point_t*	first_point, ei_color_t	color, const ei_rect_t*	clipper){
    hw_surface_lock(surface);
    while(first_point->next != NULL){
        int x0 = first_point->point.x;
        int y0 = first_point->point.y;
        int x1 = first_point->next->point.x;
        int y1 = first_point->next->point.y;

        int dx = x1 - x0;
        int dy = y1 - y0;
        int error = 0;
        ei_size_t size = hw_surface_get_size(surface);

        if(abs(dx) > abs(dy)  && dx >= 0 && dy >= 0){
            while (x0<x1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                x0+=1;
                error +=dy;
                if (2*error > dx){
                    y0+=1;
                    error-=dx;
                }
            }
        } else if(abs(dx) > abs(dy)  && dx > 0 && dy < 0){
            while (x0<x1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                x0+=1;
                error -=dy;
                if (2*error > dx){
                    y0-=1;
                    error-=dx;
                }
            }
        }else if(abs(dx) > abs(dy)  && dx < 0 && dy > 0){
            while (x0>x1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                x0-=1;
                error +=dy;
                if (2*error > dx){
                    y0+=1;
                    error+=dx;
                }
            }
        } else if(abs(dx) > abs(dy)  && dx <= 0 && dy <= 0){
            while (x0>x1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                x0-=1;
                error +=dy;
                if (2*error < dx){
                    y0-=1;
                    error-=dx;
                }
            }
        } else if (abs(dy) > abs(dx) && dx >= 0 && dy >= 0) {
            while (y0<y1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                y0+=1;
                error +=dx;
                if (2*error > dy){
                    x0+=1;
                    error-=dy;
                }
            }
        } else if (abs(dy) > abs(dx) && dx <= 0 && dy <= 0) {
            while (y0>y1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                y0-=1;
                error +=dx;
                if (2*error < dy){
                    x0-=1;
                    error-=dy;
                }
            }
        } else if (abs(dy) > abs(dx) && dx > 0 && dy < 0) {
            while (y0>y1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                y0-=1;
                error +=dx;
                if (2*error > dy){
                    x0+=1;
                    error+=dy;
                }
            }
        } else if (abs(dy) > abs(dx) && dx < 0 && dy > 0) {
            while (y0<y1){
                ei_draw_pixel(surface, color, x0, y0, size.width);
                y0+=1;
                error -=dx;
                if (2*error > dy){
                    x0-=1;
                    error-=dy;
                }
            }
        } else if (dx == 0 && dy == 0){
            ei_draw_pixel(surface, color, x0, y0, size.width);
        }


        first_point = first_point->next;
    }

    hw_surface_unlock(surface);


}

void ei_fill(ei_surface_t surface, const ei_color_t* color, const ei_rect_t* clipper){
    hw_surface_lock(surface);

    ei_size_t size = hw_surface_get_size(surface);

    uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    for (int i = 0; i < (size.width * size.height); i++)
        *pixel_ptr++ = ei_map_rgba(surface, *color);

    hw_surface_unlock(surface);
}



