/*
 * C file allowing to draw each primitive we want.
 */


#include "ei_draw.h"
#include "ei_types.h"
#include "hw_interface.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))


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

void ei_draw_polyline	(ei_surface_t surface, const ei_linked_point_t*	first_point, ei_color_t	color,
                          const ei_rect_t*	clipper){
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


/**
 * \brief represent the side of a polygon.
 */
struct side {
    int y_max;
    float x_min;
    float m_inverse;
    struct side *next;
};

/**
 * \brief represent the TC of a polygon.
 */
struct TC {
    int state;
    struct side *side;
    struct TC *next;
};

/**
 * \brief represent the highest and lowest y in the polygon.
 */

struct Y{
    int y_min;
    int y_max;
};


/**
 * \brief Search the limits of the scanline to build the TC of a Polygon
 *
 * @param first_point first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points. The last point
 *				is implicitly connected to the first point, i.e. polygons are
 *				closed, it is not necessary to repeat the first point.
 */


struct Y limits (ei_linked_point_t **first_point){
    ei_point_t point = {1,1};
    ei_linked_point_t sent = {point, *first_point};
    ei_linked_point_t *queue = &sent;
    struct Y limits={INT_MAX, 0};
    while(queue->next != NULL){
        if (queue->next->point.y < limits.y_min){
            limits.y_min = queue->next->point.y;
        }
        if(queue->next->point.y > limits.y_max){
            limits.y_max = queue->next->point.y;
        }
        queue=queue->next;
    }
    *first_point=sent.next;
    return limits;
}

 struct side *add_side(int y, float x, float m_inverse, struct side *my_side){
    struct side sent={0,0,0,my_side};
    struct side *queue = &sent;
    while(queue->next != NULL){
        queue=queue->next;
    }
    queue->next=malloc(sizeof(struct side));
    assert(queue->next != NULL);
    queue->next->y_max = y;
    queue->next->x_min = x;
    queue->next->m_inverse = m_inverse;
    queue->next->next=NULL;
    my_side = sent.next;
    return my_side;
}

struct TC *initialisation_TC(ei_linked_point_t *first_point, struct Y limits){
    struct TC my_tc = {0, NULL, NULL};
    struct TC *real_tc = &my_tc;
    struct TC sent ={0,NULL, real_tc};
    ei_point_t point = {1,1};
    ei_linked_point_t sent2 = {point, first_point};
    for(int i=limits.y_min; i<=limits.y_max; i++){
        real_tc->state=i;
        ei_linked_point_t *queue = &sent2;
        while(queue->next->next != NULL){
            if ((MIN(queue->next->point.y , queue->next->next->point.y) == i) && (queue->next->point.y != queue->next->next->point.y)){
                float m= ((float)(queue->next->point.x - queue->next->next->point.x)/(float)(queue->next->point.y - queue->next->next->point.y));
                if(queue->next->point.y > queue->next->next->point.y){
                    float x = (float)queue->next->point.x;
                    real_tc->side=add_side(queue->next->point.y, x , m, real_tc->side);
                }else{
                    float x = (float)queue->next->point.x;
                    real_tc->side=add_side(queue->next->next->point.y, x, m, real_tc->side);
                }
            }queue = queue->next;
        }
        if ((queue->next->point.y || sent2.next->point.y == i) && (queue->next->point.y != sent2.next->point.y)) {
            float m = ((float) (queue->next->point.x - sent2.next->point.x) /
                       (float) (queue->next->point.y - sent2.next->point.y));
            if (queue->next->point.y > sent2.next->point.y) {
                float x = (float) queue->next->point.x;
                real_tc->side=add_side(queue->next->point.y, x, m, real_tc->side);
            } else {
                float x = (float) queue->next->point.x;
                real_tc->side=add_side(sent2.next->point.y, x, m, real_tc->side);
            }
        }
        first_point = sent2.next;
        real_tc->next=malloc(sizeof (struct TC));
        real_tc->next->next = NULL;
        real_tc->next->side =NULL;
        real_tc=real_tc->next;
    }
    real_tc = sent.next;
    return real_tc;
}

void add_TC(struct side **tc_side, struct side **TCA){
    struct side sent = {0,0,0, *TCA};
    struct side *queue = &sent;
    struct side sent2 = {0,0,0, *tc_side};
    struct side *queue2 = &sent2;
    while(queue2->next != NULL){
        if (queue->next == NULL) {
            struct side *move=queue2->next;
            queue2->next = queue2->next->next;
            move->next=NULL;
            queue->next=move;
        }else {
            while (queue->next != NULL) {
                if (queue->next->x_min > queue2->next->x_min) {
                    struct side *move = queue2->next;
                    queue2->next = queue2->next->next;
                    move->next = queue->next;
                    queue->next=move;
                    break;
                } else {
                    queue = queue->next;
                }
            }
        }
        queue = &sent;
    }
    *tc_side = sent2.next;
    *TCA=sent.next;
}

void suppression_TCA(int y, struct side **TCA){
    struct side sent = {0,0,0, *TCA};
    struct side *queue = &sent;
    while(queue->next != NULL){
        if (queue->next->y_max == y){
            if(queue->next->next != NULL){
                struct side *rm= queue->next;
                queue->next = queue->next->next;
                free(rm);
            }else{
                free(queue->next);
                queue->next = NULL;
            }
        }
        queue=queue->next;
    }
    *TCA=sent.next;
}

void trier_TCA(struct side **TCA){
   struct side sent = {0,0,0, *TCA};
   struct side *queue = &sent;
   while (queue->next->next != NULL){
       if (queue->next->x_min > queue->next->next->x_min){
           struct  side *queue2 = &sent;
           while(queue2->next->x_min < queue->next->next->x_min){
               queue2=queue2->next;
           }
           struct side *in = queue->next->next;
           in->next=queue2->next;
           queue2->next = in;
           if(queue->next->next->next == NULL){
               queue->next->next = NULL;
           }else{
               queue->next->next =queue ->next -> next -> next;
           }
       }
       queue=queue->next;
   }
   *TCA=sent.next;
}


void filled(int y,struct side **TCA,ei_color_t  color, ei_surface_t  surface, const ei_rect_t*    clipper){
    struct side sent = {0, 0, 0, *TCA};
    struct side *queue = &sent;
    bool colo = true;
    while(queue->next->next!=NULL){
        if (colo == true){
            ei_linked_point_t pts[2];
            if ((int)queue->next->x_min==queue->next->x_min){
                ei_point_t point1={(int)queue->next->x_min,y};
                pts[0].point=point1;
            }else{
                ei_point_t point1= {(int) ceilf(queue->next->x_min), y};
                pts[0].point=point1;
            }
            pts[0].next=&pts[1];
            if ((int)queue->next->next->x_min == queue->next->next->x_min){
                ei_point_t point2= {(int)queue->next->next->x_min - 1, y};
                pts[1].point=point2;
            }else{
                ei_point_t point2= {(int)floorf(queue->next->next->x_min), y};
                pts[1].point=point2;
            }
            pts[1].next=NULL;
            ei_draw_polyline(surface,pts,color,clipper);
            colo=false;
        }else{
            colo=true;
        }
        queue=queue->next;
    }
    *TCA=sent.next;
}

void increment(struct side **TCA){
    struct side sent = {0, 0, 0, *TCA};
    struct side *queue = &sent;
    while(queue->next != NULL){
        queue->next->x_min += queue->next->m_inverse;
        queue=queue->next;
    }
    *TCA=sent.next;
}

/**
 * \brief	Draws a filled polygon.
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points. The last point
 *				is implicitly connected to the first point, i.e. polygons are
 *				closed, it is not necessary to repeat the first point.
 * @param	color		The color used to draw the polygon. The alpha channel is managed.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */

void ei_draw_polygon (ei_surface_t  surface, const ei_linked_point_t*   first_point, ei_color_t  color
                      , const ei_rect_t*    clipper){
    if (first_point != NULL){
        // limits of the scanline
        struct Y limit= limits(&first_point);
        // TC construction
        struct TC *my_tc = initialisation_TC(first_point, limit);
        //TCA construction
        struct side *real_TCA = NULL;
        while ((my_tc != NULL) || (real_TCA != NULL)){
            add_TC(&my_tc->side, &real_TCA);
            suppression_TCA(my_tc->state,&real_TCA);
            trier_TCA(&real_TCA);
            int my_state = my_tc->state;
            filled(my_state, &real_TCA, color, surface, clipper);
            my_tc=my_tc->next;
            increment(&real_TCA);
        }

    }


}

