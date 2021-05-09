//
// Created by Cassim on 09/05/2021.
//

#include "bg_utils.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"


ei_color_t *modify_color(ei_color_t *color, ei_color_t *new_color ,float coef){
    int red = (int)color->red*coef;
    int green = (int)color->green*coef;
    int blue = (int)color->blue*coef;

    new_color->red = red <= 255 ? red : 255;
    new_color->green = green <= 255 ? green : 255;
    new_color->blue = blue <= 255 ? blue : 255;
    new_color->alpha = color->alpha;

}

void draw_widgets(ei_widget_t *widget, ei_surface_t main_windows, ei_surface_t pick_surface, ei_rect_t *clipper){
    if(widget != NULL){
        widget->wclass->drawfunc(widget, main_windows, pick_surface, clipper);
        draw_widgets(widget->children_head, main_windows, pick_surface, widget->parent->content_rect);
        if(widget->next_sibling != NULL){
            draw_widgets(widget->next_sibling, main_windows, pick_surface, widget->parent->content_rect);
        }
    }
}