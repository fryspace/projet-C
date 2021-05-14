//
// Created by Cassim on 10/05/2021.
//
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"

ei_default_handle_func_t FUNC;
ei_widget_t* active_widget;

void ei_event_set_default_handle_func(ei_default_handle_func_t func){
    FUNC = func;
}

ei_default_handle_func_t ei_event_get_default_handle_func(void){
    return FUNC;
}

void ei_event_set_active_widget(ei_widget_t* widget){
    active_widget = widget;
}

ei_widget_t* ei_event_get_active_widget(void){
    return active_widget;
}