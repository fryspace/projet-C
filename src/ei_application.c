//
// Created by bossu on 06/05/2021.
//

#include <stdio.h>
#include "ei_application.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <stdio.h>
#include "ei_frame.h"
#include <stdbool.h>

ei_surface_t main_window;
ei_surface_t surface_offscreen;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    main_window = hw_create_window(main_window_size, fullscreen);

    surface_offscreen = hw_surface_create(main_window, main_window_size, EI_FALSE);
    frame_register_class();

    ei_widget_t* root = ei_widget_create("frame", NULL, NULL, NULL);
    if (root!=NULL) {
        root->requested_size = main_window_size;
        root->screen_location.size.width = main_window_size.width;
        root->screen_location.size.height = main_window_size.height;
    }

}

ei_bool_t fin=EI_FALSE;

void ei_app_run(){
    while (fin==EI_FALSE){

    }
    getchar();
}

void ei_app_free(){

}
void ei_app_quit_request(void){
    fin=EI_TRUE;
}

ei_widget_t*  ei_app_root_widget(){

}

