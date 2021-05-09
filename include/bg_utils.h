//
// Created by Cassim on 09/05/2021.
//

#ifndef PROJETC_IG_BG_UTILS_H
#define PROJETC_IG_BG_UTILS_H

#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"

ei_color_t *modify_color(ei_color_t *color, ei_color_t *new_color ,float coef);

void draw_widgets(ei_widget_t *widget, ei_surface_t main_windows, ei_surface_t pick_surface, ei_rect_t *clipper);

#endif //PROJETC_IG_BG_UTILS_H
