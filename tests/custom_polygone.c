//
// Created by Titouan on 15/05/2021.
//

/*
 * Fichier servant à tester le bon fonctionnement de polygone grâce à une génération aléatoire et possibles recouvrement
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "bg_button.h"

ei_linked_point_t *make_poly_random(int n, int max_y, int max_x){
    ei_linked_point_t *poly = NULL;
    ei_linked_point_t sent = {0,0,poly};
    ei_linked_point_t * queue = &sent;
    for(int i=0; i<n; i++){
        queue->next = malloc(sizeof(struct ei_linked_point_t));
        queue->next->point.x = rand() % max_x;
        queue->next->point.y = rand() % max_y;
        queue->next->next = NULL;
        queue = queue -> next;
    }
    poly = sent.next;
    return poly;
}

int main(int argc, char** argv){
    ei_event_t	event;
    srand(time(NULL));
    int N = rand() % 10;
    ei_size_t win_size = ei_size(800, 600);
    ei_color_t	white = { 0xff, 0xff, 0xff, 0xff };
    ei_rect_t*	clipper_ptr	= NULL;

    hw_init();

    ei_surface_t main_window = hw_create_window(win_size, EI_FALSE);
    hw_surface_lock	(main_window);
    ei_fill	(main_window, &white, clipper_ptr);

    for(int i=0; i < N; i++){
        int n = rand() % 10;
        int red = rand() % 256;
        int green =  rand() % 256;
        int blue = rand() % 256;
        int alpha = rand() % 256;
        ei_color_t color = {red, green, blue, alpha};
        ei_linked_point_t *poly = make_poly_random(n, win_size.height, win_size.width);
        ei_draw_polygon(main_window, poly, color, NULL);
    }

    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    hw_quit();

    return (EXIT_SUCCESS);
}