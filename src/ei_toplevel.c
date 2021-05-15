//
// Created by Cassim on 11/05/2021.
//

#include "ei_toplevel.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "bg_utils.h"
#include "ei_button.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

ei_widgetclass_t toplevel;
static int delta_x;
static int delta_y;

void close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param){
    ei_widget_destroy(search_in_widget(widget, widget->pick_id - 1));
    ei_widget_destroy(widget);
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


    int border = *toplevel->border;
    int top_size = 27;

    int xmin = rect.top_left.x + border;
    int xmax = rect.top_left.x - border + rect.size.width;
    int ymin = rect.top_left.y + border;
    int ymax = rect.top_left.y + border + top_size;

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
    int top_size = 27;

    int button_border = 1;
    ei_color_t button_color = {158, 13, 13, 255};
    ei_relief_t relief = ei_relief_raised;
    ei_size_t requested_size = {button_diameter, button_diameter};
    int button_x = rect.top_left.x + 3 * button_border;
    int button_y = rect.top_left.y + (top_size - button_diameter)/2 ;
    int button_width = button_diameter;
    int button_height = button_diameter;
    ei_callback_t button_callback = close_toplevel;

    ei_widget_t *button = ei_widget_create("button", toplevel->widget.parent, NULL, NULL);
    ei_button_configure(button, &requested_size, &button_color,
                        &button_border, &button_radius, &relief, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, &button_callback, NULL);
    ei_place(button, NULL, &button_x, &button_y, &button_width, &button_height, NULL, NULL, NULL, NULL);

    return (ei_button_t *)button;
}

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_toplevel_t  *toplevel = (ei_toplevel_t*) widget;

    ei_rect_t rect = widget->screen_location;

    ei_color_t top_color;
    modify_color(toplevel->bg_color, &top_color, 0.6);

    int border = *toplevel->border;
    int top_size = 27;
    int resizable_button_size = 15;

    ei_point_t little_rect_point = {rect.top_left.x + border, rect.top_left.y + top_size};
    ei_size_t little_rect_size = {rect.size.width - 2*border, rect.size.height - border - top_size};
    ei_rect_t little_rect = {little_rect_point, little_rect_size};

    ei_point_t resizable_rect_point = {rect.top_left.x + rect.size.width - resizable_button_size, rect.top_left.y + rect.size.height - resizable_button_size};
    ei_size_t resizable_rect_size = {resizable_button_size, resizable_button_size};
    ei_rect_t resizable_rect = {resizable_rect_point, resizable_rect_size};

    ei_fill(surface, &top_color, &rect);
    ei_fill(surface, toplevel->bg_color, &little_rect);
    ei_fill(surface, &top_color, &resizable_rect);
    ei_fill(pick_surface, widget->pick_color, &resizable_rect);
    ei_fill(pick_surface, widget->pick_color, &little_rect);
    ei_fill(pick_surface, widget->pick_color, &rect);

    // Creating the close button

    toplevel->close_button->widget.screen_location.top_left.x = widget->screen_location.top_left.x + toplevel->border + \
                                                                                *toplevel->close_button->border * 5;
    toplevel->close_button->widget.screen_location.top_left.y = widget->screen_location.top_left.y + *toplevel->border * 2 + \
                                                                                *toplevel->close_button->border * 5;
    toplevel->close_button->widget.screen_location.size.width = ((ei_widget_t *)toplevel->close_button)->requested_size.width;
    toplevel->close_button->widget.screen_location.size.height = ((ei_widget_t *)toplevel->close_button)->requested_size.height;
    ei_widget_t *button_widget = (ei_widget_t *)toplevel->close_button;
    button_drawfunc(button_widget, surface, pick_surface, clipper);

    // Drawing text

    if (toplevel->title != NULL){

        ei_point_t text_position;
        ei_size_t text_size;

        hw_text_compute_size(*(toplevel->title), ei_default_font, &(text_size.width), &(text_size.height));

        ei_anchor(ei_anc_northwest, &text_size, &widget->screen_location, &text_position);
        text_position.x += 25;

        ei_draw_text(surface, &text_position, *(toplevel->title), ei_default_font, ei_font_default_color, clipper);
    }
}

void* toplevel_allocfunc(){
    ei_toplevel_t *toplevel = calloc(1, sizeof(ei_toplevel_t));
    toplevel->border = calloc(1, sizeof(int));
    toplevel->bg_color = calloc(1, sizeof (ei_color_t));

    toplevel->close_button = calloc(1, sizeof(ei_button_t));
    toplevel->closable = calloc(1, sizeof(ei_bool_t));
    toplevel->resizable = calloc(1, sizeof(ei_axis_set_t));

    toplevel->size_min = calloc(1, sizeof(ei_size_t));

    return toplevel;
}

void toplevel_release_func(struct ei_widget_t* widget){
    free(((ei_toplevel_t*)widget)->bg_color);
    free(((ei_toplevel_t*)widget)->border);
    free(((ei_toplevel_t*)widget)->closable);
    free(((ei_toplevel_t*)widget)->close_button);
    free(((ei_toplevel_t*)widget)->resizable);
    free(((ei_toplevel_t*)widget)->size_min);

}

void toplevel_setdefaultsfunc(struct ei_widget_t* widget){

    *(((ei_toplevel_t*)widget)->bg_color) = ei_default_background_color;
    *(((ei_toplevel_t*)widget)->border) = 0;
    *(((ei_toplevel_t*)widget)->closable) =  EI_FALSE;
    *(((ei_toplevel_t*)widget)->resizable) = ei_axis_none;

    ((ei_toplevel_t*)widget)->title = NULL;

    ((ei_toplevel_t*)widget)->close_button = create_close_button((ei_toplevel_t*)widget);

}

void toplevel_handlefunc(ei_widget_t* widget, ei_event_t* event){

    if(event->type == ei_ev_mouse_buttondown){
        if(point_in_resizable(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(widget);
        }

        if(point_in_topbar(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(widget);
            delta_x = event->param.mouse.where.x - widget->screen_location.top_left.x;
            delta_y = event->param.mouse.where.y - widget->screen_location.top_left.y;
        }
    }

    if(event->type == ei_ev_mouse_move){
        if(point_in_resizable(event->param.mouse.where, widget) == EI_TRUE){
            ei_widget_t *active = ei_event_get_active_widget();

            if(active != NULL){
                active->screen_location.size.width = event->param.mouse.where.x - active->screen_location.top_left.x + 10;
                active->screen_location.size.height = event->param.mouse.where.y - active->screen_location.top_left.y + 10;
            }
        }

        if(point_in_topbar(event->param.mouse.where, widget) == EI_TRUE){
            ei_widget_t *active = ei_event_get_active_widget();

            if(active != NULL){
                active->screen_location.top_left.x = event->param.mouse.where.x - delta_x;
                active->screen_location.top_left.y = event->param.mouse.where.y - delta_y;
            }
        }
    }

    if(event->type == ei_ev_mouse_buttonup){
        if(point_in_resizable(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(NULL);
        }

        if(point_in_topbar(event->param.mouse.where, widget) == EI_TRUE){
            ei_event_set_active_widget(NULL);
        }
    }

}

void toplevel_register_class (){
    strcpy(toplevel.name, "toplevel");
    toplevel.drawfunc = &toplevel_drawfunc;
    toplevel.allocfunc = &toplevel_allocfunc;
    toplevel.handlefunc = &toplevel_handlefunc;
    toplevel.releasefunc = &toplevel_release_func;
    toplevel.setdefaultsfunc = &toplevel_setdefaultsfunc;
    ei_widgetclass_register(&toplevel);
}