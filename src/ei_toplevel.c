//
// Created by Cassim on 11/05/2021.
//

#include "ei_toplevel.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_application.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "bg_utils.h"
#include "ei_button.h"
#include <string.h>
#include <malloc.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

ei_widgetclass_t toplevel;
static int delta_x;
static int delta_y;

// 0 -> none | 1 -> resize | 2 -> move
static int event_type;

void close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param){
    ei_widget_destroy(widget->parent);
}

ei_bool_t point_in_resizable(ei_point_t point, ei_widget_t *widget){
    ei_rect_t rect = widget->screen_location;

    int resizable_button_size = 15;

    int xmin = rect.top_left.x + rect.size.width - resizable_button_size;
    int xmax = rect.top_left.x + rect.size.width;
    int ymin = rect.top_left.y + rect.size.height - resizable_button_size;
    int ymax = rect.top_left.y + rect.size.height;

    if (xmin <= point.x && point.x <= xmax && ymin <= point.y && point.y <= ymax){
        return EI_TRUE;
    }else{
        return EI_FALSE;
    }

}

ei_bool_t point_in_topbar(ei_point_t point, ei_widget_t *widget){
    ei_toplevel_t  *toplevel = (ei_toplevel_t*) widget;

    ei_rect_t rect = widget->screen_location;

    int border = toplevel->border;

    int xmin = rect.top_left.x;
    int xmax = rect.top_left.x + rect.size.width;
    int ymin = rect.top_left.y;
    int ymax = rect.top_left.y + border + k_top_size;

    if (xmin <= point.x && point.x <= xmax && ymin <= point.y && point.y <= ymax){
        return EI_TRUE;
    }else{
        return EI_FALSE;
    }

}

static ei_button_t* create_close_button(ei_toplevel_t * toplevel){
    ei_rect_t rect = ((ei_widget_t *)toplevel)->screen_location;
    int button_radius = 9;
    int button_diameter = 2 * button_radius;

    int button_border = 1;
    ei_color_t button_color = {158, 13, 13, 255};
    ei_relief_t relief = ei_relief_raised;
    ei_size_t requested_size = {button_diameter, button_diameter};
    int button_x = rect.top_left.x + 3 * button_border;
    int button_y = rect.top_left.y + (k_top_size - button_diameter)/2 ;
    int button_width = button_diameter;
    int button_height = button_diameter;
    ei_callback_t button_callback = close_toplevel;

    ei_widget_t *button = ei_widget_create("button", (ei_widget_t *)toplevel, NULL, NULL);
    ei_button_configure(button, &requested_size, &button_color,
                        &button_border, &button_radius, &relief, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
    ei_place(button, NULL, &button_x, &button_y, &button_width, &button_height, NULL, NULL, NULL, NULL);

    return (ei_button_t *)button;
}

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_toplevel_t  *toplevel = (ei_toplevel_t*) widget;

    ei_color_t top_color;
    modify_color(&toplevel->bg_color, &top_color, 0.6);

    int border = toplevel->border;
    int resizable_button_size = 15;

    ei_rect_t rect, content;

    toplevel->widget.screen_location.size.width = toplevel->widget.content_rect->size.width + 2 * border;
    toplevel->widget.screen_location.size.height = toplevel->widget.content_rect->size.height + k_top_size + 2 * border;

    ei_intersection(clipper, &(toplevel->widget.screen_location), &rect);

    toplevel->widget.content_rect->top_left.x = toplevel->widget.screen_location.top_left.x + border;
    toplevel->widget.content_rect->top_left.y = toplevel->widget.screen_location.top_left.y + k_top_size + border;

    ei_intersection(clipper, toplevel->widget.content_rect, &content);

    ei_fill(surface, &top_color, &rect);
    ei_fill(surface, &toplevel->bg_color, &content);
    ei_fill(pick_surface, widget->pick_color, &rect);
    ei_fill(pick_surface, widget->pick_color, &content);


    if(toplevel->resizable != ei_axis_none){
        ei_point_t resizable_rect_point = {widget->screen_location.top_left.x + widget->screen_location.size.width - resizable_button_size, widget->screen_location.top_left.y + widget->screen_location.size.height - resizable_button_size};
        ei_size_t resizable_rect_size = {resizable_button_size, resizable_button_size};
        ei_rect_t resizable_rect = {resizable_rect_point, resizable_rect_size};

        ei_fill(surface, &top_color, &resizable_rect);
        ei_fill(pick_surface, widget->pick_color, &resizable_rect);
    }

    // Creating the close button

    toplevel->close_button->widget.screen_location.top_left.x = widget->screen_location.top_left.x + 3;
    toplevel->close_button->widget.screen_location.top_left.y = widget->screen_location.top_left.y + 5;
    toplevel->close_button->widget.screen_location.size.width = ((ei_widget_t *)toplevel->close_button)->requested_size.width;
    toplevel->close_button->widget.screen_location.size.height = ((ei_widget_t *)toplevel->close_button)->requested_size.height;
    ei_widget_t *button_widget = (ei_widget_t *)toplevel->close_button;
    button_drawfunc(button_widget, surface, pick_surface, clipper);

    if(toplevel->closable == EI_FALSE){
        toplevel->close_button->callback = NULL;
    }

    // Drawing text
    if (toplevel->title != NULL){

        ei_point_t text_position;
        ei_size_t text_size;

        hw_text_compute_size(toplevel->title, ei_default_font, &(text_size.width), &(text_size.height));

        ei_anchor(ei_anc_northwest, &text_size, &widget->screen_location, &text_position);
        text_position.x += 25;

        ei_color_t white = {255, 255, 255, 255};

        ei_draw_text(surface, &text_position, toplevel->title, ei_default_font, white, clipper);
    }

}

void* toplevel_allocfunc(){
    ei_toplevel_t *toplevel = calloc(1, sizeof(ei_toplevel_t));

    return toplevel;
}

void toplevel_release_func(struct ei_widget_t* widget){
    free(((ei_toplevel_t*)widget));
}

void toplevel_setdefaultsfunc(struct ei_widget_t* widget){

    (((ei_toplevel_t*)widget)->bg_color) = ei_default_background_color;
    (((ei_toplevel_t*)widget)->border) = 0;
    (((ei_toplevel_t*)widget)->closable) =  EI_TRUE;
    (((ei_toplevel_t*)widget)->resizable) = ei_axis_none;

    ((ei_toplevel_t*)widget)->title = NULL;

    ((ei_toplevel_t*)widget)->close_button = create_close_button((ei_toplevel_t*)widget);

}

void toplevel_handlefunc(ei_widget_t* widget, ei_event_t* event){

    if(event->type == ei_ev_mouse_buttondown){
        if(point_in_resizable(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(widget);
            event_type = 1;
        }

        if(point_in_topbar(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(widget);
            delta_x = event->param.mouse.where.x - widget->screen_location.top_left.x;
            delta_y = event->param.mouse.where.y - widget->screen_location.top_left.y;
            event_type = 2;
        }
    }

    if(event->type == ei_ev_mouse_move){

        if(event_type == 1){
            // Resize

            int new_width = event->param.mouse.where.x - widget->screen_location.top_left.x - 2* ((ei_toplevel_t*)widget)->border;
            int new_height = event->param.mouse.where.y - widget->screen_location.top_left.y - 2* ((ei_toplevel_t*)widget)->border - k_top_size;

            switch(((ei_toplevel_t *)widget)->resizable){
                case ei_axis_y:
                    ei_place(widget, NULL, NULL, NULL, NULL, &new_height, NULL, NULL, NULL, NULL);
                    break;
                case ei_axis_x:
                    ei_place(widget, NULL, NULL, NULL, &new_width, NULL, NULL, NULL, NULL, NULL);
                    break;
                case ei_axis_both:
                    ei_place(widget, NULL, NULL, NULL, &new_width, &new_height, NULL, NULL, NULL, NULL);
                    break;
            }


        }else if(event_type == 2){
            // Move

            int new_x = event->param.mouse.where.x - delta_x;
            int new_y = event->param.mouse.where.y - delta_y;

            ei_place(widget, NULL, &new_x, &new_y, NULL, NULL, NULL, NULL, NULL, NULL);

        }

    }

    if(event->type == ei_ev_mouse_buttonup){
        ei_event_set_active_widget(NULL);
        event_type = 0;
    }

}

void toplevel_geomnotifyfunc(ei_widget_t*	widget, ei_rect_t rect){
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    if (widget->screen_location.size.width - 2 * toplevel->border >= toplevel->size_min->width)
    {
        widget->screen_location.size.width = rect.size.width + 2 * toplevel->border;
        widget->content_rect->size.width = widget->screen_location.size.width - 2 * toplevel->border;
    }
    if (widget->screen_location.size.height - 2 * toplevel->border - k_top_size >= toplevel->size_min->height)
    {
        widget->screen_location.size.height = rect.size.height + k_top_size + 2 * toplevel->border;
        widget->content_rect->size.height = widget->screen_location.size.height - k_top_size - 2 * toplevel->border;
    }

    widget->content_rect->top_left.x = widget->screen_location.top_left.x + toplevel->border;
    widget->content_rect->top_left.y = widget->screen_location.top_left.y + k_top_size + toplevel->border;
}

void toplevel_register_class (){
    strcpy(toplevel.name, "toplevel");
    toplevel.drawfunc = &toplevel_drawfunc;
    toplevel.allocfunc = &toplevel_allocfunc;
    toplevel.handlefunc = &toplevel_handlefunc;
    toplevel.releasefunc = &toplevel_release_func;
    toplevel.geomnotifyfunc = &toplevel_geomnotifyfunc;
    toplevel.setdefaultsfunc = &toplevel_setdefaultsfunc;
    ei_widgetclass_register(&toplevel);
}