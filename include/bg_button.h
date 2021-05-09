//
// Created by Cassim on 06/05/2021.
//

#ifndef BG_BUTTON_H
#define BG_BUTTON_H

#include "ei_types.h"
#include "hw_interface.h"

/**
 * \brief	Create an arc
 *
 * @param	center		The center point
 * @param	radius		The radius of the rectangle
 * @param   start       The start angle in degree
 * @param   end         The end angle in degree
 * @param   n           Number of point to build the arc
 *
 * @return 			A chained list of points
 */
ei_linked_point_t *ei_arc(ei_point_t center, int radius, float start, float end, int n);

/**
 * \brief	Return a rounded frame, based on a rectangle and a radius.
 *
 * @param	rect		The base rectangle
 * @param	radius		The radius of the rectangle
 * @param   part        The part of the rectangle we want ( 0 for whole, 1 for top part, 2 for bottom part )
 * @param   n           Number of point to build the arc
 *
 * @return 			A chained list of points
 */
ei_linked_point_t* ei_rounded_frame(ei_rect_t rect, int radius, int part, int n);

void ei_draw_button(ei_rect_t rect, ei_surface_t surface, int radius, ei_color_t color, int button_type, ei_rect_t* clipper);

#endif //BG_BUTTON_H
