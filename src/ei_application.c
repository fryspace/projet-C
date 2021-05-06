//
// Created by bossu on 06/05/2021.
//

#include <stdio.h>
#include "ei_application.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <stdio.h>

ei_surface_t main_window;
ei_surface_t surface_offscreen;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    main_window = hw_create_window(main_window_size, fullscreen);

    surface_offscreen = hw_surface_create(main_window, main_window_size, EI_FALSE);

    ei_widget_t* root = ei_widget_create("frame", NULL, NULL, NULL);
    root->requested_size = main_window_size;
    root->screen_location.size.width = main_window_size.width;
    root->screen_location.size.height = main_window_size.height;

}

void ei_app_run(){
    getchar();
}

void ei_app_free(){

}

ei_widget_t*  ei_app_root_widget(){

}

