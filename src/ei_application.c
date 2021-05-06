//
// Created by bossu on 06/05/2021.
//

#include "ei_application.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();
    if (fullscreen== EI_FALSE){
        ei_widget_t root={"root", NULL, NULL, NULL, NULL, NULL, NULL,
                          NULL, NULL, NULL, main_window_size, 0, 0, main_window_size, NULL};
        ei_widgetclass_register("root")
        main_window = hw_create_window(main_window_size, fullscreen)
    }
}

void ei_app_run(){
    getchar()
}

void ei_app_free(){

}

