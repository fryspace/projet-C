//
// Created by bossu on 07/05/2021.
//
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include <malloc.h>

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

}

void* frame_allocfunc(){
    return calloc(1, sizeof(ei_frame_t));
}

void frame_release_func(){

}

void frame_setdefaultsfunc(){

}

void frame_register_class (){
    strcpy(frame.name, "frame");
    frame.drawfunc = &frame_drawfunc;
    frame.allocfunc = &frame_allocfunc;
    frame.releasefunc = &frame_release_func;
    frame.setdefaultsfunc = &frame_setdefaultsfunc;
    ei_widgetclass_register(&frame);
}