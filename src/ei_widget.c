//
// Created by bossu on 06/05/2021.
//
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <malloc.h>
#include "bg_utils.h"
#include "ei_draw.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_frame.h"
#include "ei_types.h"
#include <assert.h>

static uint32_t BASE_PICK_ID = 0x00000000;
extern ei_surface_t surface_offscreen;
extern ei_widget_t *root;

void ei_button_configure(ei_widget_t*		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            char**			text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_t**		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            void**			user_param){

    ei_button_t* button=(ei_button_t*)widget;
    int default_radius = 40;

    if(requested_size != NULL){
        widget->requested_size = *requested_size;
    }

    button->bg_color = color!=NULL ? color : &ei_default_background_color;

    button->border = border_width!=NULL ? border_width : 0;

    button->corner_radius = corner_radius!=NULL ? corner_radius : &default_radius;

    button->relief = relief!=NULL && border_width!=0 ? relief : ei_relief_none;

    button->text=text;

    button->text_font= text_font!=NULL ? text_font : ei_default_font;

    button->text_color = text_color!=NULL ? text_color : &ei_font_default_color;

    *(button->text_anchor) = text_anchor!=NULL ? *text_anchor : ei_anc_center;

    button->img = img;

    button->img_anchor = img_anchor!=NULL ? img_anchor : ei_anc_center;

    button->img_rect = img_rect;

    button->callback = callback;
    button->user_param = user_param;

}

void ei_frame_configure	(ei_widget_t* widget, ei_size_t*	requested_size, const ei_color_t* color,int* border_width, ei_relief_t* relief,
                            char** text, ei_font_t* text_font, ei_color_t*	text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t*	img_anchor){
    ei_frame_t* frame=(ei_frame_t*)widget; //voir page22
    if(requested_size!=NULL){
        frame->widget.requested_size=*requested_size;
    }

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
        *(frame->relief)=*relief;
    }
    else{
        *(frame->relief)=ei_relief_none;
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
        *(frame->text_anchor)=*text_anchor;
    }
    else{
        *(frame->text_anchor)= ei_anc_center;
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

void ei_toplevel_configure(ei_widget_t*		widget,
                                              ei_size_t*		requested_size,
                                              ei_color_t*		color,
                                              int*			border_width,
                                              char**			title,
                                              ei_bool_t*		closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_t**		min_size){
    ei_toplevel_t* toplevel=(ei_toplevel_t*)widget; //voir page22
    if(requested_size!=NULL){
        toplevel->widget.requested_size=*requested_size;
    }

    if(color!=NULL){
        toplevel->bg_color=color;
    }
    else{
        toplevel->bg_color=&ei_default_background_color;
    }

    if(border_width!=NULL){
        toplevel->border=border_width;
    }
    else{
        toplevel->border=0;
    }

    toplevel->closable = closable;
    toplevel->resizable = resizable;
    toplevel->size_min = min_size;

    if(title!=NULL){
        toplevel->title=title;
    }

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
        uint8_t *pick = (uint8_t *)&BASE_PICK_ID;
        widget->pick_color = malloc(sizeof(ei_color_t));
        widget->pick_color->red = pick[0];
        widget->pick_color->green = pick[1];
        widget->pick_color->blue = pick[2];
        widget->pick_color->alpha = 255;
        widget->pick_id = ei_map_rgba(surface_offscreen, *widget->pick_color);
        BASE_PICK_ID += 1;
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

static void ei_widget_destroy_children(ei_widget_t*	widget) {
    if(widget != NULL){
        ei_widget_destroy_children(widget->children_head);

        if(widget->next_sibling != NULL){
            ei_widget_destroy_children(widget->next_sibling);
        }

        widget->wclass->releasefunc(widget);
        free(widget);
    }
}

void ei_widget_destroy(ei_widget_t*	widget){
    if(widget != NULL){
        if(widget->parent != NULL){
            ei_widget_t *parent = widget->parent;
            ei_widget_t *current_widget = parent->children_head;

            if(current_widget == widget){
                parent->children_head = widget->next_sibling;
            }
            else if(parent->children_tail == widget){
                parent->children_tail = parent->children_head;
            }
            else{
                while(current_widget->next_sibling != NULL && current_widget->next_sibling != widget){
                    current_widget = current_widget->next_sibling;
                }

                if(current_widget->next_sibling == widget){
                    current_widget->next_sibling = widget->next_sibling;
                }
            }
            widget->parent = NULL;
            widget->next_sibling = NULL;
        }
        ei_widget_destroy_children(widget);
    }
}

ei_widget_t* ei_widget_pick(ei_point_t*	where){
    hw_surface_lock(surface_offscreen);
    ei_size_t size = hw_surface_get_size(surface_offscreen);
    uint32_t* pick_id = (uint32_t *)hw_surface_get_buffer(surface_offscreen) + where->y*size.width + where->x;
    ei_widget_t *widget = search_in_widget(root, *pick_id);
    hw_surface_unlock(surface_offscreen);

    return widget == root ? NULL : widget;

}



