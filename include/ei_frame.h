//
// Created by bossu on 07/05/2021.
//

#ifndef PROJETC_IG_EI_FRAME_H
#define PROJETC_IG_EI_FRAME_H

#include "ei_widget.h"

typedef struct ei_frame_t {
    ei_widget_t           widget;
    ei_color_t*     bg_color;
    int*                  border;
    ei_relief_t*          relief;
    char**                text;
    ei_font_t*            text_font;
    ei_color_t*           text_color;
    ei_anchor_t*          text_anchor;
    ei_surface_t*         img;
    ei_rect_t**           img_rect;
    ei_anchor_t*          img_anchor;
} ei_frame_t;

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);


void* frame_allocfunc();



void frame_release_func();


void frame_setdefaultsfunc();

void frame_register_class ();

#endif //PROJETC_IG_EI_FRAME_H
