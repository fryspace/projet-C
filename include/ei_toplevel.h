//
// Created by Cassim on 11/05/2021.
//

#ifndef PROJETC_IG_EI_TOPLEVEL_H
#define PROJETC_IG_EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_button.h"
#include "ei_event.h"

typedef struct ei_toplevel_t {
    ei_widget_t           widget;
    ei_color_t           bg_color;
    int                  border;
    char*                title;
    ei_button_t*		close_button;
    ei_bool_t		closable;
    ei_axis_set_t		resizable;
    ei_size_t*		size_min;

} ei_toplevel_t;

static const int k_top_size = 27;

void close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param);

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void* toplevel_allocfunc();

void toplevel_release_func();

void toplevel_setdefaultsfunc();

void toplevel_register_class ();

#endif //PROJETC_IG_EI_TOPLEVEL_H
