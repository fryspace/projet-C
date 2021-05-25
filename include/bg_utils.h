//
// Created by Cassim on 09/05/2021.
//

#ifndef PROJETC_IG_BG_UTILS_H
#define PROJETC_IG_BG_UTILS_H

#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"

ei_color_t *modify_color(ei_color_t *color, ei_color_t *new_color ,float coef);

// Return TRUE if rect1 in rect2
ei_bool_t rect_in_rect(ei_rect_t rect1, ei_rect_t rect2);

void ei_intersection(ei_rect_t *first, ei_rect_t *second, ei_rect_t *result);

ei_bool_t point_in_rectangle(ei_point_t point, ei_rect_t rect);

void draw_widgets(ei_widget_t *widget, ei_surface_t main_windows, ei_surface_t pick_surface, ei_rect_t *clipper);

void ei_anchor(ei_anchor_t anchor, ei_size_t *size, ei_rect_t *rect, ei_point_t *position);

ei_widget_t* search_in_widget(ei_widget_t* widget, uint32_t pick_id);

#endif //PROJETC_IG_BG_UTILS_H
