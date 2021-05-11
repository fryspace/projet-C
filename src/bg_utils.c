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
        if(widget->parent != NULL){
            if(clipper != NULL){
                widget->wclass->drawfunc(widget, main_windows, pick_surface, clipper);
            }else{
                widget->wclass->drawfunc(widget, main_windows, pick_surface, widget->parent->content_rect);
            }
        }else{
            widget->wclass->drawfunc(widget, main_windows, pick_surface, clipper);
        }

        if(widget->children_head != NULL){
            draw_widgets(widget->children_head, main_windows, pick_surface, clipper);
            ei_widget_t  *queue = widget->children_head->next_sibling;
            while(queue != NULL){
                draw_widgets(queue, main_windows, pick_surface, clipper);
                queue = queue->next_sibling;
            }
        }
    }
}

void ei_anchor(ei_anchor_t anchor, ei_size_t *size, ei_rect_t *rect, ei_point_t *position){
    
    switch (anchor)
    {
        case ei_anc_center:
            (*position).x = rect->top_left.x + rect->size.width / 2 - size->width / 2;
            (*position).y = rect->top_left.y + rect->size.height / 2 - size->height / 2;
            break;
        case ei_anc_north:
            (*position).x = rect->top_left.x + rect->size.width / 2 - size->width / 2;
            (*position).y = rect->top_left.y;
            break;
        case ei_anc_northeast:
            (*position).x = rect->top_left.x + rect->size.width - size->width;
            (*position).y = rect->top_left.y;
            break;
        case ei_anc_east:
            (*position).x = rect->top_left.x + rect->size.width - size->width;
            (*position).y = rect->top_left.y + rect->size.height / 2 - size->height / 2;
            break;
        case ei_anc_southeast:
            (*position).x = rect->top_left.x + rect->size.width - size->width;
            (*position).y = rect->top_left.y + rect->size.height - size->height;
            break;
        case ei_anc_south:
            (*position).y = rect->top_left.y + rect->size.height - size->height;
            (*position).x = rect->top_left.x + rect->size.width / 2 - size->width / 2;
            break;
        case ei_anc_southwest:
            (*position).y = rect->top_left.y + rect->size.height - size->height;
            (*position).x = rect->top_left.x;
            break;
        case ei_anc_west:
            (*position).x = rect->top_left.x;
            (*position).y = rect->top_left.y + rect->size.height / 2 - size->height;
            break;
        default:
            (*position).x = rect->top_left.x;
            (*position).y = rect->top_left.y;
    }

}

ei_widget_t* search_in_widget(ei_widget_t* widget, uint32_t pick_id)
{
    if (widget->pick_id == pick_id) {
        return widget;
    }
    if (widget->children_head != NULL){
        ei_widget_t* current = widget->children_head->next_sibling;
        while (current != NULL){
            return search_in_widget(current, pick_id);
        }
        return search_in_widget(widget->children_head, pick_id);
    }
    return NULL;
}