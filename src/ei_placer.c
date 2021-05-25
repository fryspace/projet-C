#include "ei_placer.h"
#include "ei_widget.h"
#include "bg_utils.h"
#include "ei_application.h"
#include <stdlib.h>

void ei_place (struct ei_widget_t*	widget, ei_anchor_t* anchor, int* x, int* y, int* width, int* height, float* rel_x, float* rel_y, float* rel_width, float* rel_height){
    ei_placer_params_t * current_placer = widget->placer_params;

    ei_anchor_t anchor2=(anchor==NULL)?ei_anc_northwest:*anchor;

    int x_anchor, y_anchor, w, h;
    float rx_anchor, ry_anchor, rw, rh;


    if(x != NULL){
        x_anchor = *x;
    }else if(current_placer->x != NULL) {
        x_anchor = current_placer->x_data;
    }else{
        x_anchor = 0;
    }

    if(y != NULL){
        y_anchor = *y;
    }else if(current_placer->y != NULL) {
        y_anchor = current_placer->y_data;
    }else{
        y_anchor = 0;
    }

    if(rel_x != NULL){
        rx_anchor = *rel_x;
    }else if(current_placer->rx != NULL){
        rx_anchor = current_placer->rx_data;
    }else{
        rx_anchor = 0;
    }

    if(rel_y != NULL){
        ry_anchor = *rel_y;
    }else if(current_placer->ry != NULL){
        ry_anchor = current_placer->ry_data;
    }else{
        ry_anchor = 0;
    }

    if(rel_width != NULL){
        rw = *rel_width;
    }else if(current_placer->rw != NULL){
        rw = current_placer->rw_data;
    }else{
        rw = 0;
    }

    if(rel_height != NULL){
        rh = *rel_height;
    }else if(current_placer->rh != NULL){
        rh = current_placer->rh_data;
    }else{
        rh = 0;
    }

    if(width != NULL){
        w = *width;
    }else if(current_placer->w != NULL){
        w = current_placer->w_data;
    }
    else if(rel_width != NULL){
        w = (int)(*rel_width*(float)widget->parent->content_rect->size.width);
    }else{
        w = widget->requested_size.width;
    }

    if(height != NULL){
        h = *height;
    }else if(current_placer->h != NULL){
        h = current_placer->h_data;
    }
    else if(rel_height != NULL){
        h = (int)(*rel_height*(float)widget->parent->content_rect->size.height);
    }else{
        h = widget->requested_size.height;
    }


    ei_placer_params_t  placer={&anchor2, anchor2, &x_anchor, x_anchor, &y_anchor, y_anchor, &w, w, &h, h, &rx_anchor, rx_anchor,
                                &ry_anchor, ry_anchor, &rw, rw, &rh, rh};
    *(widget->placer_params) = placer;
    ei_placer_run(widget);

}


void ei_placer_run(struct ei_widget_t* widget){
    ei_placer_params_t* placer = (ei_placer_params_t*)(widget -> placer_params);
    int height, width;

    if (widget->parent != NULL)
    {
        if (placer->w_data !=0 || placer->rw_data !=0.0){ //au moins un des 2 non nul
            if(placer->w_data != 0){ //gestion des priorités, choix: width avant rel_width
                width = placer->w_data;
            }
            else{
                width = (int)(placer->rw_data * (float)widget->parent->requested_size.width);
            }
        }
        else{
            width = widget->requested_size.width;
        }

        if (placer->h_data !=0 || placer->rh_data != 0.0){ //pareil mais pour la hauteur
            if(placer->h_data != 0){
                height =placer->h_data;
            }
            else{
                height =(int)(placer->rh_data * (float)widget->parent->requested_size.height);
            }
        }
        else{
            height = widget->requested_size.height;
        }

    }

    int x, y, new_x, new_y;

    //en fonction d'où est l'ancre on donne position du top_left
    switch (placer->anchor_data)
    {
        case ei_anc_center:
            x = width / 2;
            y = height / 2;
            break;
        case ei_anc_northeast:
            x = width;
            y = 0;
            break;
        case ei_anc_north:
            x = width / 2;
            y = 0;
            break;
        case ei_anc_southeast:
            x = width;
            y = height;
            break;
        case ei_anc_east:
            x = width;
            y = height / 2;
            break;
        case ei_anc_west:
            x = 0;
            y = height / 2;
            break;
        case ei_anc_southwest:
            x = 0;
            y = height;
            break;
        case ei_anc_south:
            x = width / 2;
            y = height;
            break;
        default:
            x = 0;
            y = 0;
    }

    if (widget->parent != NULL)
    {
        new_x = widget->parent->content_rect->top_left.x + placer->rx_data * widget->parent->content_rect->size.width + placer->x_data - x;
        new_y = widget->parent->content_rect->top_left.y + placer->ry_data * widget->parent->content_rect->size.height + placer->y_data - y;

        if((widget->screen_location.size.width != width && widget->screen_location.size.width != 0) || (widget->screen_location.size.height != height && widget->screen_location.size.height != 0) || (widget->screen_location.top_left.x != new_x && widget->screen_location.top_left.x != 0) || (widget->screen_location.top_left.y != y && widget->screen_location.top_left.y != 0)){
            ei_rect_t begin_rect;

            ei_intersection(widget->parent->content_rect, &widget->screen_location, &begin_rect);
            if(widget->parent->parent != NULL){
                ei_intersection(widget->parent->parent->content_rect, &begin_rect, &begin_rect);
            }
            ei_app_invalidate_rect(&begin_rect);
        }

        widget->screen_location.size.width = width;
        widget->screen_location.size.height = height;
        widget->screen_location.top_left.x = new_x;
        widget->screen_location.top_left.y = new_y;
        widget->wclass->geomnotifyfunc(widget, widget->screen_location);

        if((widget->screen_location.size.width != width && widget->screen_location.size.width != 0) || (widget->screen_location.size.height != height && widget->screen_location.size.height != 0) || (widget->screen_location.top_left.x != new_x && widget->screen_location.top_left.x != 0) || (widget->screen_location.top_left.y != y && widget->screen_location.top_left.y != 0)){
            ei_rect_t final_rect;

            ei_intersection(widget->parent->content_rect, &widget->screen_location, &final_rect);

            if(widget->parent->parent != NULL){
                ei_intersection(widget->parent->parent->content_rect, &final_rect, &final_rect);
            }

            ei_app_invalidate_rect(&final_rect);
        }
    }

    ei_widget_t *child = widget->children_head;
    while ( child != NULL) {
        if (child->placer_params != NULL){
            ei_placer_run(child); //on recalcule la taille et position de tous les enfant
            ei_widget_t *sibling = child->next_sibling;
            while (sibling)
            {
                ei_placer_run(sibling);
                sibling = sibling->next_sibling;
            }
        }
        child = child->next_sibling;
    }

}

void ei_placer_forget(struct ei_widget_t* widget){
    ei_placer_params_t* placer = (ei_placer_params_t*)(widget -> placer_params);
    free(placer->x);
    free(placer->y);
    free(placer->rh);
    free(placer->rw);
    free(placer->rx);
    free(placer->ry);
    free(placer->h);
    free(placer->w);
}