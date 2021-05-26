//
// Created by Cassim on 09/05/2021.
//

#ifndef PROJETC_IG_BG_UTILS_H
#define PROJETC_IG_BG_UTILS_H

#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"

void set_destroy(ei_bool_t destroy);

ei_bool_t get_destroy(void);

/**
 * \brief	Change a color with a coef to get a lower or higher color
 *
 * @param	color		The current color
 * @param   new_color   Where to store the new color
 * @param   coef        The coefficient
 */
ei_color_t *modify_color(ei_color_t *color, ei_color_t *new_color ,float coef);

/**
 * \brief	Check if a rectangle is include in another
 *          Here is if the rect1 is in the rect 2
 *
 * @param	rect1		The first rectangle
 * @param   rect2       The another rectangle
 *
 * @return  EI_TRUE if rect1 in rect2
 */
ei_bool_t rect_in_rect(ei_rect_t rect1, ei_rect_t rect2);

/**
 * \brief	Free all the polygons
 *
 * @param	polygon		The polygon to free
 */
void free_polygon(ei_linked_point_t **polygon);

/**
 * \brief	Return the intersection of two rectangles
 *
 * @param	first   The first rectangle
 * @param   second  The second rectangle
 * @param   result  Where to store the result
 */
void ei_intersection(ei_rect_t *first, ei_rect_t *second, ei_rect_t *result);

/**
 * \brief	Check if a point is in a rectangle
 *
 * @param	point   The point
 * @param   rect    The rectangle
 *
 * @return Return EI_TRUE if the point is in the rectangle
 */
ei_bool_t point_in_rectangle(ei_point_t point, ei_rect_t rect);

/**
 * \brief	Draw all the widgets
 *
 * @param	widget		The widget to be drawn
 * @param   main_windows    The main surface
 * @param pick_surface      The picking surface
 * @param   clipper         The clipper
 */
void draw_widgets(ei_widget_t *widget, ei_surface_t main_windows, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	Get the position of a rect based on the anchor
 *
 * @param	anchor  The anchor of the rectangle
 * @param   size    The size of the rectangle
 * @param   rect    The rectangle
 * @param   position   The result position
 */
void ei_anchor(ei_anchor_t anchor, ei_size_t *size, ei_rect_t *rect, ei_point_t *position);

/**
 * \brief	Search in the widget tree based on the pick_id
 *
 * @param	widget		The widget tree
 * @param   pick_id     The pick_id
 *
 * @return  The widget with the pick_id or NULL
 */
ei_widget_t* search_in_widget(ei_widget_t* widget, uint32_t pick_id);

#endif //PROJETC_IG_BG_UTILS_H
