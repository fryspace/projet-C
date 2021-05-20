#ifndef PROJETC_IG_EI_BUTTON_H
#define PROJETC_IG_EI_BUTTON_H

#include "ei_widget.h"

typedef struct ei_button_t {
    ei_widget_t           widget;
    ei_color_t           bg_color;
    int                  border;
    int                   corner_radius;
    ei_relief_t          relief;
    char*                text;
    ei_font_t            text_font;
    ei_color_t           text_color;
    ei_anchor_t          text_anchor;
    ei_surface_t        img;
    ei_rect_t*           img_rect;
    ei_anchor_t         img_anchor;
    ei_callback_t        callback;
    void*                user_param;
} ei_button_t;

void button_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void* button_allocfunc();

void button_release_func();

void button_setdefaultsfunc();

void button_register_class ();

#endif //PROJETC_IG_EI_BUTTON_H
