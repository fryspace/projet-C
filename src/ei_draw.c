/*
 * C file allowing to draw each primitive we want.
 */

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"



/**
 * \brief represent the side of a polygon.
 */
struct side {
    int ymax;
    int xmin;
    float 1_m;
    struct side next;
};

/**
 * \brief represent the TC of a polygon.
 */
struct TC{
    int state;
    struct side first_cote;
    struct TC next;
};

/**
 * \brief represent the highest and lowest y in the polygones.
 */

struct Y{
    int* ymin;
    int* ymax;
};


/**
 * \brief Search the limits of the scanline to build the TC of a Polygon
 *
 * @param first_point first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points. The last point
 *				is implicitly connected to the first point, i.e. polygons are
 *				closed, it is not necessary to repeat the first point.
 */


int *limits (const ei_linked_point_t* first_point){
    ei_point_t point = {1,1};
    ei_linked_point_t sent = {point, first_point};
    struct Y limits={NULL,NULL};
    while(first_point->next != NULL){
        if (first_point->point->y < limits.ymin){
            limits.ymin = first_point->point->y;
        }else if(first_point->point->y > limits.ymax){
            limits.ymax = first_point->point->y;
        }
        first_point=first_point->suivant;
    }
    *first_point=sent->suivant;
    return limits;
}

struct side *add_side(int *y, int *x, int* 1_m, struct side *my_side){
    struct side sent={0,0,0,*my_side};
    struct side *queue = &sent;
    while(sent->next != NULL){
        queue=queue->next;
    }
    queue->next=malloc(sizeof(struct side));
    assert(queue->next != NULL);
    queue->next->y = y;
    queue->next->x = x;
    queue->next->1_m = 1_m;
    queue->next->next=NULL;
    *my_side = sent.suiv;
}

struct *TC(struct ei_linked_point_t first_point, struct Y limits){
    struct TC *my_tc = {1,NULL,NULL};
    struct TC sent ={0,NULL,*my_tc};
    for(i=limits->ymin, i<=limits->ymax, i++){
        ei_point_t point = {1,1};
        ei_linked_point_t sent = {point, first_point};
        while(first_point->next != NULL){
            if (first_point->point->y || first_point->next->point->y == limits->ymin){
                if(first_point->point->y > first_point->next->point->y){
                    int *1_m= (first_point->point->x - first_point->point->next->x)/((first_point->point->y - first_point->point->next->y))
                    my_tc->side=add_side(first_point->point->y, first_point->next->point->x, 1_m, struct side *my_tc->side);
                }
            }
        }

    }
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
    if (first_point == NULL){
        return 0
    }
    hw_surface_lock(surface);
    // limits of the scanline
    struct Y limit= limits(first_point);
    // TC construction

}

