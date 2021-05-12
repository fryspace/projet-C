//
// Created by Cassim on 11/05/2021.
//

#ifndef PROJETC_IG_EI_TOPLEVEL_H
#define PROJETC_IG_EI_TOPLEVEL_H

#include "ei_widget.h"

typedef struct ei_toplevel_t {
    ei_widget_t           widget;
    ei_color_t*           bg_color;
    int*                  border;
    char**                title;
    ei_bool_t*		closable;
    ei_axis_set_t*		resizable;
    ei_size_t**		size_min;

} ei_toplevel_t;

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);


void* toplevel_allocfunc();



void toplevel_release_func();


void toplevel_setdefaultsfunc();

void toplevel_register_class ();

#endif //PROJETC_IG_EI_TOPLEVEL_H
