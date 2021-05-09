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
#include "ei_event.h"
#include "bg_utils.h"
#include <stdbool.h>

ei_surface_t main_window;
ei_surface_t surface_offscreen;
ei_widget_t* root;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    main_window = hw_create_window(main_window_size, fullscreen);

    surface_offscreen = hw_surface_create(main_window, main_window_size, EI_FALSE);
    frame_register_class();

    root = ei_widget_create("frame", NULL, NULL, NULL);
    if (root!=NULL) {
        root->requested_size = main_window_size;
        root->screen_location.size.width = main_window_size.width;
        root->screen_location.size.height = main_window_size.height;
    }

}

void ei_app_run(){
    ei_event_t		event;
    hw_surface_lock(main_window);

    ei_rect_t clipper = hw_surface_get_rect(main_window);

    hw_surface_lock(surface_offscreen);

    draw_widgets(root, main_window, surface_offscreen, &clipper);

    hw_surface_unlock(surface_offscreen);
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    /* Wait for a character on command line. */
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

void ei_app_free(){
    hw_quit();
}
void ei_app_quit_request(void){

}

ei_widget_t*  ei_app_root_widget(){

}

