//
// Created by bossu on 06/05/2021.
//

#include "ei_placer.h"
#include "ei_widget.h"
#include <stdlib.h>

void ei_place (struct ei_widget_t*	widget, ei_anchor_t* anchor, int* x, int* y, int* width, int* height, float* rel_x, float* rel_y, float* rel_width, float* rel_height){
    ei_anchor_t  anchor2=(anchor==NULL)?ei_anc_northwest:*anchor;
    int x_anchor = (x==NULL)?0 : *x;
    int y_anchor = (y==NULL)?0 : *y;
    float rx_anchor = (rel_x==NULL)?0 : *rel_x;
    float ry_anchor = (rel_y==NULL)?0 : *rel_y;
    float rw = (rel_width==NULL)?0 : *rel_width;
    float rh = (rel_height==NULL)?0 : *rel_height;
    int w;
    if(width==NULL){
        if (rel_width==NULL){
            w=widget->requested_size.width;
        }
        else {
            w=(int)(*rel_width*(float)widget->parent->screen_location.size.width);
        }
    }else{w=*width;}

    int h;
    if(height==NULL){
        if (rel_height==NULL){
            h=widget->requested_size.height;
        }
        else{
            h=(int)(*rel_width*(float)widget->parent->screen_location.size.height);
        }
    }else{h=*height;}

    ei_placer_params_t  placer={&anchor2, anchor2, &x_anchor, x_anchor, &y_anchor, y_anchor, &w, w, &h, h, &rx_anchor, rx_anchor,
                                &ry_anchor, ry_anchor, &rw, rw, &rh, rh};
    widget->placer_params=&placer;
    ei_placer_run(widget);

}


void ei_placer_run(struct ei_widget_t* widget){
    ei_placer_params_t* placer = (ei_placer_params_t*)(widget -> placer_params);
    int x_anchor;
    int y_anchor;
    int width_anchor;
    int height_anchor;

    if (*placer->w!=0 || *placer->rw!=0.0){ //au moins un des 2 non nul
        if(*placer->w!=0){ //gestion des priorités, choix: width avant rel_width
            width_anchor=*placer->w;
        }
        else{
            width_anchor=(int)(*placer->rw * (float)widget->parent->requested_size.width);
        }
    }
    else{
        width_anchor=widget->requested_size.width;
    }

    if (*placer->h!=0 || *placer->rh!=0.0){ //pareil mais pour la hauteur
        if(*placer->h!=0){
            height_anchor=*placer->h;
        }
        else{
            height_anchor=(int)(*placer->rh * (float)widget->parent->requested_size.height);
        }
    }
    else{
        height_anchor = widget->requested_size.height;
    }
    widget->screen_location.size.height=height_anchor;
    widget->screen_location.size.width=width_anchor;

    //cherche position de l'ancre
    x_anchor= ((int)(*placer->rx * (float)widget->parent->requested_size.width) + *placer->x) + widget->parent->screen_location.top_left.x;
    y_anchor= ((int)(*placer->ry * (float)widget->parent->requested_size.height) + *placer->y) + widget->parent->screen_location.top_left.y;

    //en fonction d'où est l'ancre on donne position du top_left
    if (*placer->anchor==ei_anc_northwest || *placer->anchor==0){
        widget->screen_location.top_left.x=x_anchor;
        widget->screen_location.top_left.y=y_anchor;
    }
    else if (*placer->anchor==ei_anc_north){
        widget->screen_location.top_left.x=x_anchor - (int)width_anchor/2;
        widget->screen_location.top_left.y=y_anchor;
    }
    else if (*placer->anchor==ei_anc_west){
        widget->screen_location.top_left.x=x_anchor;
        widget->screen_location.top_left.y=y_anchor - (int)height_anchor/2;
    }
    else if (*placer->anchor==ei_anc_south){
        widget->screen_location.top_left.x=x_anchor - (int)width_anchor/2;
        widget->screen_location.top_left.y=y_anchor - height_anchor;
    }
    else if (*placer->anchor==ei_anc_east){
        widget->screen_location.top_left.x=x_anchor - width_anchor;
        widget->screen_location.top_left.y=y_anchor - (int)height_anchor/2;
    }
    else if (*placer->anchor==ei_anc_southwest){
        widget->screen_location.top_left.x=x_anchor;
        widget->screen_location.top_left.y=y_anchor - height_anchor;
    }
    else if (*placer->anchor==ei_anc_southeast){
        widget->screen_location.top_left.x=x_anchor - width_anchor;
        widget->screen_location.top_left.y=y_anchor - height_anchor;
    }
    else if (*placer->anchor==ei_anc_northeast){
        widget->screen_location.top_left.x=x_anchor - width_anchor;
        widget->screen_location.top_left.y=y_anchor;
    }
    else if (*placer->anchor==ei_anc_center) {
        widget->screen_location.top_left.x = x_anchor - width_anchor/2;
        widget->screen_location.top_left.y = y_anchor - height_anchor/2;
    }
    ei_widget_t *child = widget->children_head;
    while ( child != NULL) {
        if (child->placer_params != NULL){
            ei_placer_run(child); //on recalcule la taille et position de tous les enfant
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