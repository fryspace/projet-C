//
// Created by Cassim on 10/05/2021.
//
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"

ei_default_handle_func_t FUNC;

void ei_event_set_default_handle_func(ei_default_handle_func_t func){
    FUNC = func;
}

ei_default_handle_func_t ei_event_get_default_handle_func(void){
    return FUNC;
}