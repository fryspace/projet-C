//
// Created by Cassim on 06/05/2021.
//
#include <stdlib.h>
#include <math.h>
#include "ei_types.h"
#include "bg_button.h"
#include "ei_draw.h"
#include "hw_interface.h"
#include "bg_utils.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

ei_linked_point_t *ei_arc(ei_point_t center, int radius, float start, float end, int n){

    ei_linked_point_t *result = NULL;

    // Utilisation des radians pour utiliser les fonctions cos et sin
    float start_rad = start * (M_PI / 180.0);
    float end_rad = end * (M_PI / 180.0);
    float h = (end_rad - start_rad)/n;

    ei_linked_point_t* current = malloc(sizeof(ei_linked_point_t));
    current->point.x = center.x + radius*cos(start_rad);
    current->point.y = center.y - radius*sin(start_rad);

    result = current;
    for(int i = 1; i < n; i++){

        ei_linked_point_t* next = malloc(sizeof(ei_linked_point_t));

        next->point.x = center.x + radius*cos(start_rad + i*h);
        next->point.y = center.y - radius*sin(start_rad + i*h);
        next->next = NULL;

        current->next = next;
        current = next;
    }

    return result;
}


ei_linked_point_t* ei_rounded_frame(ei_rect_t rect, int radius, int part, int n){
    ei_linked_point_t *result;

    int width = rect.size.width;
    int height = rect.size.height;

    int right_coord = rect.top_left.x + width - radius;
    int left_coord = rect.top_left.x + radius;

    int top_coord = rect.top_left.y + radius;
    int bottom_coord = rect.top_left.y + height - radius;

    ei_linked_point_t *top_left_arc, *top_right_arc, *bottom_left_arc, *bottom_right_arc;

    ei_point_t top_left_center = {left_coord, top_coord};
    ei_point_t top_right_center = {right_coord, top_coord};
    ei_point_t bottom_left_center = {left_coord, bottom_coord};
    ei_point_t bottom_right_center = {right_coord, bottom_coord};

    ei_linked_point_t* current;
    ei_point_t first_point;
    ei_linked_point_t* first = malloc(sizeof(ei_linked_point_t));

    ei_point_t mid_right, mid_left;
    ei_linked_point_t* mid_right_point, *mid_left_point;
    int h;

    switch(part){
        // Cas du rectangle enti??
        case 0:
            top_left_arc = ei_arc(top_left_center, radius, 90, 180, n);
            bottom_right_arc = ei_arc(bottom_right_center, radius, 270, 360, n);
            bottom_left_arc = ei_arc(bottom_left_center, radius, 180, 270, n);
            top_right_arc = ei_arc(top_right_center, radius, 0, 90, n);

            first_point.x = top_left_arc->point.x;
            first_point.y = top_left_arc->point.y;

            current = top_left_arc;
            result = current;

            while(current->next != NULL){
                current = current->next;
            }

            current->next = bottom_left_arc;
            while(current->next != NULL){
                current = current->next;
            }

            current->next = bottom_right_arc;
            while(current->next != NULL){
                current = current->next;
            }

            current->next = top_right_arc;
            while(current->next != NULL){
                current = current->next;
            }

            first->point = first_point;
            first->next = NULL;

            current->next = first;

            break;
            // Cas du rectangle du bas
        case 2:
            h = (int)MIN(height/2, width/2);
            mid_right_point = malloc(sizeof(ei_linked_point_t));
            mid_left_point = malloc(sizeof(ei_linked_point_t));

            bottom_right_arc = ei_arc(bottom_right_center, radius, 0, -90, n);
            top_right_arc = ei_arc(top_right_center, radius, 45, 0, n/2);
            bottom_left_arc = ei_arc(bottom_left_center, radius, -90, -145, n/2);

            first_point.x = top_right_arc->point.x;
            first_point.y = top_right_arc->point.y;

            current = top_right_arc;
            result = current;

            while(current->next != NULL){
                current = current->next;
            }

            current->next = bottom_right_arc;
            while(current->next != NULL){
                current = current->next;
            }

            current->next = bottom_left_arc;
            while(current->next != NULL){
                current = current->next;
            }

            mid_left.x = rect.top_left.x + h;
            mid_left.y = rect.top_left.y + height - h;

            mid_left_point->point = mid_left;
            mid_left_point->next = mid_right_point;

            mid_right.x = rect.top_left.x + width - h;
            mid_right.y = rect.top_left.y + h;

            mid_right_point->point = mid_right;
            mid_right_point->next = first;

            current->next = mid_left_point;

            first->point = first_point;
            first->next = NULL;


            break;
            /* Cas du rectangle du haut */
            case 1:
            h = (int)MIN(height/2, width/2);
            mid_right_point = malloc(sizeof(ei_linked_point_t));
            mid_left_point = malloc(sizeof(ei_linked_point_t));

            top_left_arc = ei_arc(top_left_center, radius, 180, 90, n);
            top_right_arc = ei_arc(top_right_center, radius, 90, 35, n/2);
            bottom_left_arc = ei_arc(bottom_left_center, radius, -135, -180, n/2);

            first_point.x = top_left_arc->point.x;
            first_point.y = top_left_arc->point.y;

            current = top_left_arc;
            result = current;

            while(current->next != NULL){
                current = current->next;
            }

            current->next = top_right_arc;
            while(current->next != NULL){
                current = current->next;
            }

            mid_right.x = rect.top_left.x + width - h;
            mid_right.y = rect.top_left.y + h;

            mid_right_point->point = mid_right;
            mid_right_point->next = mid_left_point;

            current->next = mid_right_point;

            mid_left.x = rect.top_left.x + h;
            mid_left.y = rect.top_left.y + height - h;

            mid_left_point->point = mid_left;
            mid_left_point->next = bottom_left_arc;

            while(current->next != NULL){
                current = current->next;
            }

            first->point = first_point;
            first->next = NULL;

            current->next = first;

            break;
        default:
            exit(1);
    }

    return result;
}

void ei_draw_button(ei_rect_t rect, int border, ei_surface_t surface, ei_surface_t pick_surface, int radius, ei_color_t color, ei_color_t pick_color, int button_type, ei_rect_t* clipper){

    ei_color_t top_color, bottom_color;
    switch (button_type) {
        // Cas non appuy??
        case 0:
            modify_color(&color, &top_color, 1.4);
            modify_color(&color, &bottom_color, 0.6);
            break;
        // Cas appuy??
        case 1:
            modify_color(&color, &top_color, 0.6);
            modify_color(&color, &bottom_color, 1.4);
            break;
        default:
            modify_color(&color, &top_color, 1.4);
            modify_color(&color, &bottom_color, 0.6);
            break;
    }

    ei_point_t little_rect_point;
    little_rect_point.x = rect.top_left.x + border;
    little_rect_point.y = rect.top_left.y + border;

    ei_size_t little_rect_size;
    little_rect_size.width = rect.size.width - 2* border;
    little_rect_size.height = rect.size.height - 2* border;

    ei_rect_t little_rect = {little_rect_point, little_rect_size};

    /* On r??cup??re toutes les parties du boutons */
    ei_linked_point_t *main = ei_rounded_frame(little_rect, radius, 0, 10);
    ei_linked_point_t *top = ei_rounded_frame(rect, radius, 1, 10);
    ei_linked_point_t *bottom = ei_rounded_frame(rect, radius, 2, 10);
    ei_linked_point_t *offscreen_points = ei_rounded_frame(rect, radius, 0, 10);

    /* On trace le button sur la surface offscreen puis sur notre onscreen surface */
    ei_draw_polygon(pick_surface, offscreen_points, pick_color, clipper);
    ei_draw_polygon(surface, top, top_color, clipper);
    ei_draw_polygon(surface, bottom, bottom_color, clipper);
    ei_draw_polygon(surface, main, color, clipper);

    free_polygon(&main);
    free_polygon(&top);
    free_polygon(&bottom);
    free_polygon(&offscreen_points);

}