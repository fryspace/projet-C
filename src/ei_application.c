#include <malloc.h>
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
ei_linked_rect_t *rect_list = NULL;
ei_bool_t MAIN_LOOP_EXIT = EI_FALSE;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    /* Création de la fenêtre principale */
    main_window = hw_create_window(main_window_size, fullscreen);

    /* Création de la fenêtre offscreen */
    surface_offscreen = hw_surface_create(main_window, main_window_size, EI_FALSE);

    hw_surface_lock(surface_offscreen);

    /* On enregistre toutes nos classes de widget */
    frame_register_class();
    button_register_class();
    toplevel_register_class();

    /* Création du widget root */
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

    /* Dessin de tous les widgets */
    draw_widgets(root, main_window, surface_offscreen, &clipper);

    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    /* Tant qu'on a pas un signal d'arret */
    while(MAIN_LOOP_EXIT != EI_TRUE){

        /* Attente du prochain évenement */
        hw_event_wait_next(&event);

        /* Appel de la fonction par défauts */
        ei_default_handle_func_t default_handler = ei_event_get_default_handle_func();
        if(default_handler != NULL){
            default_handler(&event);
        }

        ei_widget_t *widget = NULL;
        ei_widget_t *active_widget = NULL;

        ei_point_t mouse = event.param.mouse.where;

        /* On test en fonction du type d'evenement */
        switch(event.type){
            case ei_ev_exposed:
                break;
            case ei_ev_keydown:
            case ei_ev_keyup:
                widget = root;
                ei_app_invalidate_rect(&root->screen_location);
                break;
            case ei_ev_mouse_buttondown:
            case ei_ev_mouse_move:
            case ei_ev_mouse_buttonup:
                widget = ei_widget_pick(&mouse);
                active_widget = ei_event_get_active_widget();

                if(active_widget != NULL){
                    active_widget->wclass->handlefunc(active_widget, &event);
                }else if(widget != NULL) {
                    widget->wclass->handlefunc(widget, &event);
                }

                break;
            default:
                break;
        }

        if (rect_list != NULL)
        {
            active_widget = ei_event_get_active_widget();
            ei_widget_t * current_widget = active_widget != NULL ? active_widget : widget;

            if(get_destroy() == EI_TRUE){
                hw_surface_lock(main_window);
                hw_surface_lock(surface_offscreen);

                draw_widgets(root, main_window, surface_offscreen, &clipper);

                hw_surface_unlock(surface_offscreen);
                hw_surface_unlock(main_window);
                hw_surface_update_rects(main_window, NULL);

                set_destroy(EI_FALSE);
            }else if(current_widget != NULL){
                hw_surface_lock(main_window);
                hw_surface_lock(surface_offscreen);

                if(current_widget->parent != NULL){
                    current_widget = current_widget->parent;
                }

                // On redessine le sous arbre du widget
                draw_widgets(current_widget, main_window, surface_offscreen, &(current_widget->screen_location));

                hw_surface_unlock(surface_offscreen);
                hw_surface_unlock(main_window);
                hw_surface_update_rects(main_window, rect_list);
            }else if(current_widget == NULL){
                hw_surface_lock(main_window);
                hw_surface_lock(surface_offscreen);

                // On redessine le sous arbre du widget
                draw_widgets(root, main_window, surface_offscreen, &clipper);

                hw_surface_unlock(surface_offscreen);
                hw_surface_unlock(main_window);
                hw_surface_update_rects(main_window, rect_list);
            }

            ei_linked_rect_t *to_delete = NULL;
            while (rect_list != NULL)
            {
                to_delete = rect_list;
                rect_list = rect_list->next;
                free(to_delete);
            }

        }
    }
}

void ei_app_invalidate_rect(ei_rect_t* rect){
    ei_rect_t new_rect = *rect;

    ei_rect_t null_rect = {NULL, NULL};
    ei_linked_rect_t sent = {null_rect, rect_list};
    ei_linked_rect_t *queue = &sent;

    while(queue->next != NULL){
        if(rect_in_rect(new_rect, queue->next->rect) == EI_TRUE){
            return;
        }else if(rect_in_rect(queue->next->rect, new_rect) == EI_TRUE){
            queue->next = queue->next->next;
        }else{
            queue = queue->next;
        }

    }
    queue->next = malloc(sizeof(ei_linked_rect_t));
    queue->next->rect = new_rect;
    queue->next->next = NULL;

    rect_list = sent.next;
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

ei_surface_t ei_app_root_surface(void){
    return main_window;
}

