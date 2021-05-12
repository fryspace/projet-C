//
// Created by bossu on 06/05/2021.
//

#include <stdio.h>
#include "ei_application.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_button.h"
#include "ei_event.h"
#include "bg_utils.h"

ei_surface_t main_window;
ei_surface_t surface_offscreen;
ei_widget_t* root;
ei_bool_t MAIN_LOOP_EXIT = EI_FALSE;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    main_window = hw_create_window(main_window_size, fullscreen);

    surface_offscreen = hw_surface_create(main_window, main_window_size, EI_FALSE);
    hw_surface_lock(surface_offscreen);
    frame_register_class();
    button_register_class();
    toplevel_register_class();

    root = ei_widget_create("frame", NULL, NULL, NULL);
    if (root!=NULL) {
        root->requested_size = main_window_size;
        root->screen_location.size.width = main_window_size.width;
        root->screen_location.size.height = main_window_size.height;
    }

}

void ei_app_run(){
    ei_event_t		event;
    while(MAIN_LOOP_EXIT != EI_TRUE){
        hw_surface_lock(main_window);

        ei_rect_t clipper = hw_surface_get_rect(main_window);



        draw_widgets(root, main_window, surface_offscreen, &clipper);

        hw_surface_unlock(main_window);
        hw_surface_update_rects(main_window, NULL);

        /* Wait for a character on command line. */
        hw_event_wait_next(&event);
        ei_widget_t *widget;

        ei_point_t mouse = event.param.mouse.where;

        switch(event.type){
            case ei_ev_exposed:
                break;
            case ei_ev_mouse_buttondown:
            case ei_ev_mouse_move:
            case ei_ev_mouse_buttonup:
                widget = ei_widget_pick(&mouse);

                if(widget != NULL) {

                    widget->wclass->handlefunc(widget, &event);
                }

                break;
            default:
                break;
        }


    }
}

void ei_app_free(){
    hw_quit();
}
void ei_app_quit_request(void){
    MAIN_LOOP_EXIT = EI_TRUE;
}

ei_widget_t*  ei_app_root_widget(){
    return root;
}

