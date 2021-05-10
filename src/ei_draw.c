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
 *
 * -y_max: defines until which y the side needs to be treat
 * -x_min: defines which is the x of the y_min of the side, after the initializations, it represents the x of the side
 * for the current y
 * -m_inverse: represents dx/dy
 * -next: pointer on another side
 */

struct side {
    int y_max;
    float x_min;
    float m_inverse;
    struct side *next;
};

/**
 * \brief represent the TC of a polygon.
 *
 * -state: defines at which y the TC corresponds
 * -side: pointer on the side too add at this side
 * -next: pointer on a structures representing the next state
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
 * @param first_point  The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points. The last point
 *				is implicitly connected to the first point, i.e. polygons are
 *				closed, it is not necessary to repeat the first point.
 */


struct Y limits (ei_linked_point_t *first_point){
    struct Y limits = {INT_MAX, 0};
    while(first_point->next != NULL){
        if (first_point->next->point.y < limits.y_min){
            limits.y_min = first_point->next->point.y;
        }
        if(first_point->next->point.y > limits.y_max){
            limits.y_max = first_point->next->point.y;
        }
        first_point=first_point->next;
    }
    return limits;
}

/**
 * \brief add a side at the end of a side chain
 *
 * @param   y   the value to add to then new side for y_max parameter
 *
 * @param   x   the value to add to the new side for x_min parameter
 *
 * @param   m_inverse   the value to add to the new side for m_inverse parameter
 *
 * @param   my_side     a pointer on a chained side which we want to add a new_side
 */

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

/**
 * \brief	Initialise the TC needed to draw a polygon
 *
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points. The last point
 *				is implicitly connected to the first point, i.e. polygons are
 *				closed, it is not necessary to repeat the first point.
 *
 * @param	limits      the limits of the points and side to draw the polygon
 */

struct TC *initialisation_TC(ei_linked_point_t *first_point, struct Y limits){
    //struct TC my_tc = {0, NULL, NULL};
    struct TC *real_tc = malloc(sizeof(struct TC));
    real_tc->state = 0;
    real_tc->side = NULL;
    real_tc->next = NULL;
    struct TC sent ={0,NULL, real_tc};
    ei_point_t point = {1,1};
    ei_linked_point_t sent2 = {point, first_point};
    for(int i=limits.y_min; i<=limits.y_max; i++){
        real_tc->state=i;
        ei_linked_point_t *queue = &sent2;
        while(queue->next->next != NULL){
            if ((MIN(queue->next->point.y , queue->next->next->point.y) == i) && (queue->next->point.y != queue->next->next->point.y)){
                float m= ((float)(queue->next->point.x - queue->next->next->point.x)/(float)(queue->next->point.y - queue->next->next->point.y));
                if(queue->next->point.y < queue->next->next->point.y){
                    float x = (float)queue->next->point.x;
                    real_tc->side = add_side(queue->next->next->point.y, x , m, real_tc->side);
                }else{
                    float x = (float)queue->next->next->point.x;
                    real_tc->side = add_side(queue->next->point.y, x, m, real_tc->side);
                }
            }queue = queue->next;
        }
        if ((queue->next->point.y || sent2.next->point.y == i) && (queue->next->point.y != sent2.next->point.y)) {
            float m = ((float) (queue->next->point.x - sent2.next->point.x) /
                       (float) (queue->next->point.y - sent2.next->point.y));
            if (queue->next->point.y > sent2.next->point.y) {
                float x = (float) sent2.next->point.x;
                real_tc->side = add_side(queue->next->point.y, x, m, real_tc->side);
            } else {
                float x = (float) queue->next->point.x;
                real_tc->side = add_side(sent2.next->point.y, x, m, real_tc->side);
            }
        }
        first_point = sent2.next;
        real_tc->next=malloc(sizeof (struct TC));
        real_tc->next->state = 0;
        real_tc->next->next = NULL;
        real_tc->next->side =NULL;
        real_tc=real_tc->next;
    }
    real_tc = sent.next;
    return real_tc;
}


/**
 * \brief	add a TC side to the TCA, respecting the increasing order of the TCA
 *
 * @param	tc_side     a side of a TC to be add
 *
 * @param	TCA     a side which will help to fill the pixels of the polygon
 */


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
                if (queue->next->x_min >= queue2->next->x_min) {
                    struct side *move = queue2->next;
                    queue2->next = queue2->next->next;
                    move->next = queue->next;
                    queue->next=move;
                    break;
                } else {
                    queue = queue->next;
                    if (queue->next == NULL) {
                        struct side *move = queue2->next;
                        queue2->next = queue2->next->next;
                        move->next = NULL;
                        queue->next = move;
                        break;
                    }
                }
            }
        }
        queue = &sent;
    }
    *tc_side = sent2.next;
    *TCA=sent.next;
}

/**
 * \brief	suppress sides which are outdated from the TCA
 *
 * @param	y     the number of the current scanline
 *
 * @param	TCA     a side which will help to fill the pixels of the polygon
 */

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
                break;
            }
        }else{
            queue=queue->next;
        }
    }
    *TCA=sent.next;
}

/**
 * \brief	sort the TCA following the increasing order
 *
 * @param	y     the number of the current scanline
 *
 * @param	TCA     a side which will help to fill the pixels of the polygon
 */

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


/**
 * \brief	fill the pixels on the scanline which needs to be filled
 *
 * @param	y     the number of the current scanline
 *
 * @param	TCA     a side which will help to fill the pixels of the polygon
 *
 * @param	color		The color used to draw the polygon. The alpha channel is managed.
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 *
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */

void filled(int y,struct side **TCA,ei_color_t  color, ei_surface_t  surface, const ei_rect_t*    clipper){
    struct side sent = {0, 0, 0, *TCA};
    struct side *queue = &sent;
    bool colo = true;
    while(queue->next->next!=NULL){
        if (colo == true){
            ei_linked_point_t pts[2];
            if ((int)queue->next->x_min==queue->next->x_min){
                pts[0].point.x = (int)queue->next->x_min;
                pts[0].point.y = y;
            }else{
                pts[0].point.x = (int) ceilf(queue->next->x_min);
                pts[0].point.y = y;
            }
            pts[0].next=&pts[1];
            if ((int)queue->next->next->x_min == queue->next->next->x_min){
                pts[1].point.x = (int)queue->next->next->x_min - 1;
                pts[1].point.y = y;
            }else{
                pts[1].point.x = (int)floorf(queue->next->next->x_min);
                pts[1].point.y = y;
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

/**
 * \brief	refresh the TCA to prepare the next scanline scan
 *
 * @param	TCA     a side which will help to fill the pixels of the polygon
 */


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
        struct Y limit = limits(first_point);
        // TC construction
        struct TC *my_tc = initialisation_TC(first_point, limit);
        //TCA construction
        struct side *real_TCA = NULL;
        for (int i = limit.y_min; i<= limit.y_max; i++){
            add_TC(&my_tc->side, &real_TCA);
            suppression_TCA(my_tc->state,&real_TCA);
            if (real_TCA != NULL){
                trier_TCA(&real_TCA);
                filled(i, &real_TCA, color, surface, clipper);
            }
            my_tc=my_tc->next;
            if (real_TCA != NULL){
                increment(&real_TCA);
            }
        }
    }
}

void ei_draw_text(ei_surface_t surface, const ei_point_t* where, const char* text, ei_font_t font, ei_color_t color, const ei_rect_t* clipper){
    ei_rect_t rect = hw_surface_get_rect(surface);
    ei_font_t text_font = font == NULL ? ei_default_font : font;

    ei_surface_t text_surface = hw_text_create_surface(text, text_font, color);

    hw_surface_lock(text_surface);
    hw_surface_set_origin(text_surface, *where);

    hw_surface_unlock(text_surface);
    hw_surface_free(text_surface);
}

/**
 * \brief	Copies pixels from a source surface to a destination surface.
 *		The source and destination areas of the copy (either the entire surfaces, or
 *		subparts) must have the same size before considering clipping.
 *		Both surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param	destination	The surface on which to copy pixels.
 * @param	dst_rect	If NULL, the entire destination surface is used. If not NULL,
 *				defines the rectangle on the destination surface where to copy
 *				the pixels.
 * @param	source		The surface from which to copy pixels.
 * @param	src_rect	If NULL, the entire source surface is used. If not NULL, defines the
 *				rectangle on the source surface from which to copy the pixels.
 * @param	alpha		If true, the final pixels are a combination of source and
 *				destination pixels weighted by the source alpha channel and
 *				the transparency of the final pixels is set to opaque.
 *				If false, the final pixels are an exact copy of the source pixels,
 				including the alpha channel.
 *
 * @return			Returns 0 on success, 1 on failure (different sizes between source and destination).
 */
int	ei_copy_surface	(ei_surface_t	destination,    const ei_rect_t*	dst_rect,
                                    ei_surface_t	source, const ei_rect_t*	src_rect,
                                    ei_bool_t	alpha){
    // initialisation and exit cases
    ei_point_t * dst_buffer;
    ei_point_t * src_buffer;
    int width;
    int height;
    if (dst_rect != NULL){
        if(src_rect != NULL){
            if (dst_rect->size.width != src_rect->size.width || dst_rect->size.height != src_rect->size.height){
                return 1;
            }else{
                dst_buffer->x = dst_rect->top_left.x;
                dst_buffer->y = dst_rect->top_left.y;
                src_buffer->x = src_rect->top_left.x;
                src_buffer->y = src_rect->top_left.y;
                width = dst_rect->size.width;
                height = dst_rect->size.height;
            }
        }else{
            if (dst_rect->size.width != hw_surface_get_size(source).width || dst_rect->size.height != hw_surface_get_size(source).height){
                return 1;
            }else{
                dst_buffer->x = dst_rect->top_left.x;
                dst_buffer->y = dst_rect->top_left.y;
                src_buffer->x = 0;
                src_buffer->y = 0;
                width = dst_rect->size.width;
                height = dst_rect->size.height;
            }
        }
    }else{
        if(src_rect != NULL){
            if (hw_surface_get_size(destination).width != src_rect->size.width || hw_surface_get_size(destination).height != src_rect->size.height){
                return 1;
            }else{
                dst_buffer->x = 0;
                dst_buffer->y = 0;
                src_buffer->x = src_rect->top_left.x;
                src_buffer->y = src_rect->top_left.y;
                width = hw_surface_get_size(destination).width;
                height = hw_surface_get_size(destination).height;
            }
        }else{
            if (hw_surface_get_size(destination).width != hw_surface_get_size(source).width || hw_surface_get_size(destination).height != hw_surface_get_size(source).height){
                return 1;
            }else{
                dst_buffer->x = 0;
                dst_buffer->y = 0;
                src_buffer->x = 0;
                src_buffer->y = 0;
                width = hw_surface_get_size(destination).width;
                height = hw_surface_get_size(destination).height;
            }
        }
    }
    //treatment
    for (int i = 0; i < height; i++){
        for(int j =0; j< width; j++){
            uint32_t* pixel_src = (uint32_t *)hw_surface_get_buffer(source) + (src_buffer->x + i)*width + j + src_buffer->y;
            uint32_t * pixel_dst = (uint32_t *)hw_surface_get_buffer(destination) + (dst_buffer->x + i)*width + j + dst_buffer->y;
            *pixel_dst = *pixel_src;
        }
    }
}