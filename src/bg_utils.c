#include "bg_utils.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"

ei_bool_t DESTROY = EI_FALSE;

void set_destroy(ei_bool_t destroy){
    DESTROY = destroy;
}

ei_bool_t get_destroy(void){
    return DESTROY;
}

ei_color_t *modify_color(ei_color_t *color, ei_color_t *new_color ,float coef){
    int red = (int)color->red*coef;
    int green = (int)color->green*coef;
    int blue = (int)color->blue*coef;

    new_color->red = red <= 255 ? red : 255;
    new_color->green = green <= 255 ? green : 255;
    new_color->blue = blue <= 255 ? blue : 255;
    new_color->alpha = color->alpha;

}

void free_polygon(ei_linked_point_t **polygon){
    if(polygon != NULL){
        ei_linked_point_t* to_delete = *polygon;
        while(*polygon != NULL){
            *polygon = (*polygon)->next;
            free(to_delete);
            to_delete = *polygon;
        }
    }
}

ei_bool_t rect_in_rect(ei_rect_t rect1, ei_rect_t rect2){
    if(rect1.top_left.x >= rect2.top_left.x && rect1.top_left.y >= rect2.top_left.y && rect1.top_left.x + rect1.size.width <= rect2.top_left.x + rect2.size.width && rect1.top_left.y + rect1.size.height <= rect2.top_left.y + rect2.size.height){
        return EI_TRUE;
    }else{
        return EI_FALSE;
    }
}

ei_bool_t point_in_rectangle(ei_point_t point, ei_rect_t rect){
    if(point.x >= rect.top_left.x && point.x <= rect.top_left.x + rect.size.width && point.y >= rect.top_left.y && point.y <= rect.top_left.y + rect.size.height){
        return EI_TRUE;
    }else{
        return EI_FALSE;
    }
}

void ei_intersection(ei_rect_t *first, ei_rect_t *second, ei_rect_t *result){
    ei_rect_t *left_rect, *right_rect, *top_rect, *bottom_rect;

    if (first->top_left.x <= second->top_left.x)
    {
        left_rect = first;
        right_rect = second;
    }
    else
    {
        left_rect = second;
        right_rect = first;
    }

    if (first->top_left.y <= second->top_left.y)
    {
        top_rect = first;
        bottom_rect = second;
    }
    else
    {
        top_rect = second;
        bottom_rect = first;
    }


    // Creating the new rectangle
    ei_point_t dest_point;
    ei_size_t dest_size;

    if ((left_rect->top_left.x + left_rect->size.width < right_rect->top_left.x) || top_rect->top_left.y + top_rect->size.height < bottom_rect->top_left.y)
    {
        dest_point.x = 0;
        dest_point.y = 0;
        dest_size.width = 0;
        dest_size.height = 0;
    }
    else
    {
        int x_margin, y_margin;

        if (right_rect->top_left.x + right_rect->size.width <= left_rect->top_left.x + left_rect->size.width)
        {
            x_margin = left_rect->top_left.x + left_rect->size.width - (right_rect->top_left.x + right_rect->size.width);
        }
        else
        {
            x_margin = 0;
        }
        if (bottom_rect->top_left.y + bottom_rect->size.height <= top_rect->top_left.y + top_rect->size.height)
        {
            y_margin = top_rect->top_left.y + top_rect->size.height - (bottom_rect->top_left.y + bottom_rect->size.height);
        }
        else
        {
            y_margin = 0;
        }
        dest_point.x = right_rect->top_left.x;
        dest_point.y = bottom_rect->top_left.y;
        dest_size.width = left_rect->top_left.x + left_rect->size.width - dest_point.x - x_margin;
        dest_size.height = top_rect->top_left.y + top_rect->size.height - dest_point.y - y_margin;
    }
    result->top_left = dest_point;
    result->size = dest_size;
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
        }

        if(widget->next_sibling != NULL){
            draw_widgets(widget->next_sibling, main_windows, pick_surface, clipper);
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
    ei_widget_t *current = widget->children_head;
    while(current){
        ei_widget_t *picking_widget = search_in_widget(current, pick_id);
        if(picking_widget){
            return picking_widget;
        }
        current = current->next_sibling;
    }

    return NULL;
}