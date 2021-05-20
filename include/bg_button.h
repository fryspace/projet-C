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

/**
 * \brief	Draw a button
 *
 * @param	rect		The base rectangle
 * @param	border		The button border size
 * @param   surface     The surface where the button will be drawn
 * @param   pick_surface     The picking surface to use events
 * @param   radius      The radius of the corner
 * @param   color       The button color
 * @param   pick_color  The picking color
 * @param   button_type The type of putton ( pressed ( 1 ) or unpress ( 0 ) )
 * @param   clipper     Button clipper
 *
 */
void ei_draw_button(ei_rect_t rect, int border, ei_surface_t surface, ei_surface_t pick_surface, int radius, ei_color_t color, ei_color_t pick_color, int button_type, ei_rect_t* clipper);

#endif //BG_BUTTON_H
