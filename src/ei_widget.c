//
// Created by bossu on 06/05/2021.
//
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <malloc.h>
#include "ei_draw.h"
#include "ei_application.h"




void ei_frame_configure	(ei_widget_t* widget, ei_size_t*	requested_size, const ei_color_t* color,int* border_width, ei_relief_t* relief,
                            char** text, ei_font_t* text_font, ei_color_t*	text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t*	img_anchor){


}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, ei_widget_t* parent, void* user_data, ei_widget_destructor_t destructor){
    ei_widgetclass_t* wclasse= ei_widgetclass_from_name(class_name);
    if (wclasse) {
        ei_widget_t *widget = wclasse->allocfunc();
        widget->wclass = wclasse;

        widget->parent = parent;
        if (parent) {
            widget->screen_location.top_left.y = parent->content_rect->top_left.y;
            widget->screen_location.top_left.x = parent->content_rect->top_left.x;
            if (parent->children_head != NULL) {
                parent->children_tail->next_sibling = widget; //on rajoute widget à la chaine
                parent->children_tail = widget; //nouveau dernier enfant
            } else { //1er enfant de parent
                parent->children_head = widget;
                parent->children_tail = widget;
            }
        } else {   //pas de parent: widget est root
            widget->screen_location.top_left.x = 0;
            widget->screen_location.top_left.y = 0;
        }
        widget->destructor = destructor;
        widget->user_data = user_data;
        widget->children_head = NULL;
        widget->children_tail = NULL;
        widget->next_sibling = NULL;
        widget->pick_color = NULL;
        widget->pick_id = 0;
        widget->placer_params = NULL;
        widget->requested_size.width = 0;
        widget->requested_size.height = 0;
        ei_rect_t location = {0, 0};
        widget->content_rect = malloc(sizeof(ei_rect_t));
        widget->screen_location = location;
        widget->content_rect->top_left.x = widget->screen_location.top_left.x;
        widget->content_rect->top_left.y = widget->screen_location.top_left.y;
        widget->content_rect->size.width = 0;
        widget->content_rect->size.height = 0;
        wclasse->setdefaultsfunc(widget);
        return widget;
    }
    return NULL;
}




