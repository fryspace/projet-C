//
// Created by bossu on 06/05/2021.
//
#include "ei_widget.h"
#include "ei_widgetclass.h"


void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

}

void frame_allocfunc(){

}

void release_func(){

}

void setdefaultsfunc(){

}

void ei_frame_configure	(ei_widget_t* widget, ei_size_t*	requested_size, const ei_color_t* color,int* border_width, ei_relief_t* relief,
                            char** text, ei_font_t* text_font, ei_color_t*	text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t*	img_anchor){


}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, ei_widget_t* parent, void* user_data, ei_widget_destructor_t destructor){
    ei_widget_t * widget;
    widget->parent= parent;
    widget->destructor= destructor;
    ei_widgetclass_t* wclasse = ei_widgetclass_from_name(class_name);
    widget->wclass= wclasse;
    widget->user_data=user_data;
    return widget;
}




