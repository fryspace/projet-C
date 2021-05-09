//
// Created by bossu on 06/05/2021.
//
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <malloc.h>
#include "ei_draw.h"
#include "ei_application.h"
#include "ei_frame.h"
#include "ei_types.h"
#include <assert.h>




void ei_frame_configure	(ei_widget_t* widget, ei_size_t*	requested_size, const ei_color_t* color,int* border_width, ei_relief_t* relief,
                            char** text, ei_font_t* text_font, ei_color_t*	text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t*	img_anchor){
    ei_frame_t* frame=(ei_frame_t*)widget; //voir page22
    if(color!=NULL){
        frame->bg_color=color;
    }
    else{
        frame->bg_color=&ei_default_background_color;
    }

    if(border_width!=NULL){
        frame->border=border_width;
    }
    else{
        frame->border=0;
    }
    if(relief!=NULL && border_width!=0){
        frame->relief=relief;
    }
    else{
        frame->relief=ei_relief_none;
    }
    //assert(text!=NULL && img!=NULL);
    frame->text=text;

    if(text_font!=NULL){
        frame->text_font=text_font;
    }
    else{
        frame->text_font=ei_default_font;
    }
    if(text_color!=NULL){
        frame->text_color=text_color;
    }
    else{
        frame->text_color=&ei_font_default_color;
    }
    if(text_anchor!=NULL){
        frame->text_anchor=text_anchor;
    }
    else{
        frame->text_anchor= ei_anc_center;
    }
    frame->img=img;

    if(img_anchor!=NULL){
        frame->img_anchor=img_anchor;
    }
    else{
        frame->img_anchor=ei_anc_center;
    }
    frame->img_rect=img_rect;

}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, ei_widget_t* parent, void* user_data, ei_widget_destructor_t destructor){
    ei_widgetclass_t* wclasse= ei_widgetclass_from_name(class_name);
    if (wclasse) {
        ei_widget_t *widget = wclasse->allocfunc();
        widget->wclass = wclasse;

        widget->parent = parent;
        if (parent) {
            widget->screen_location.top_left.y = parent->content_rect->top_left.y;
            widget->screen_location.top_left.x = parent->content_rect->top_left.x;
            if (parent->children_head != NULL) {
                parent->children_tail->next_sibling = widget; //on rajoute widget à la chaine
                parent->children_tail = widget; //nouveau dernier enfant
            } else { //1er enfant de parent
                parent->children_head = widget;
                parent->children_tail = widget;
            }
        } else {   //pas de parent: widget est root
            widget->screen_location.top_left.x = 0;
            widget->screen_location.top_left.y = 0;
        }
        widget->destructor = destructor;
        widget->user_data = user_data;
        widget->children_head = NULL;
        widget->children_tail = NULL;
        widget->next_sibling = NULL;
        widget->pick_color = malloc(sizeof (ei_color_t));
        widget->pick_id = 0;
        widget->placer_params = NULL;
        widget->requested_size.width = 0;
        widget->requested_size.height = 0;
        ei_rect_t location = {0, 0};
        widget->content_rect = malloc(sizeof(ei_rect_t));
        widget->screen_location = location;
        widget->content_rect->top_left.x = widget->screen_location.top_left.x;
        widget->content_rect->top_left.y = widget->screen_location.top_left.y;
        widget->content_rect->size.width = 0;
        widget->content_rect->size.height = 0;
        wclasse->setdefaultsfunc(widget);
        return widget;
    }
    return NULL;
}




