#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <malloc.h>
#include "bg_utils.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_frame.h"
#include "ei_types.h"
#include <assert.h>
#include <string.h>

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

    if(requested_size != NULL){
        widget->requested_size = *requested_size;
    }else{
            widget->requested_size.width = 30;
            widget->requested_size.height = 30;
    }

    button->bg_color = color!=NULL ? *color : ei_default_background_color;

    button->border = border_width!=NULL ? *border_width : k_default_button_border_width;

    button->corner_radius = corner_radius!=NULL ? *corner_radius : k_default_button_corner_radius;

    button->relief = relief!=NULL && border_width!=0 ? *relief : ei_relief_none;

    button->text_font= text_font!=NULL ? *text_font : ei_default_font;

    if(text != NULL){
        if(*text){
            if(button->text != NULL){
                button->text = realloc(button->text, strlen(*text) + 1);
            }else{
                button->text = malloc(strlen(*text) + 1);
            }
            strcpy(button->text, *text);

            ei_size_t text_size;
            hw_text_compute_size(button->text, button->text_font, &(text_size.width), &(text_size.height));

            if(button->widget.requested_size.width < text_size.width){
                button->widget.requested_size.width = text_size.width + 20;
            }

            if(button->widget.requested_size.height < text_size.height){
                button->widget.requested_size.height = text_size.height + 20;
            }
        }else{
            button->text = NULL;
        }
    }

    button->text_color = text_color!=NULL ? *text_color : ei_font_default_color;

    button->text_anchor = text_anchor!=NULL ? *text_anchor : ei_anc_center;

    if(img != NULL){
        ei_size_t image_size = hw_surface_get_size(*img);
        button->img = hw_surface_create(ei_app_root_surface(), image_size, EI_FALSE);
        ei_copy_surface(button->img, NULL, *img, NULL, EI_FALSE);
    }


    button->img_anchor = img_anchor!=NULL ? *img_anchor : ei_anc_center;

    if(img_rect != NULL){
        button->img_rect = malloc(sizeof ( ei_rect_t ));
        button->img_rect->size = (*img_rect)->size;
        button->img_rect->top_left.x = (*img_rect)->top_left.x;
        button->img_rect->top_left.y = (*img_rect)->top_left.y;
    }

    if(callback != NULL){
        button->callback = *callback;
    }

    if(user_param != NULL){
        button->user_param = *user_param;
    }

    if (widget->content_rect->size.width < widget->requested_size.width){
        widget->content_rect->size.width = widget->requested_size.width;
    }

    if (widget->content_rect->size.height < widget->requested_size.height){
        widget->content_rect->size.height = widget->requested_size.height;
    }

    ei_app_invalidate_rect(&widget->screen_location);
}

void ei_frame_configure	(ei_widget_t* widget, ei_size_t*	requested_size, const ei_color_t* color,int* border_width, ei_relief_t* relief,
                            char** text, ei_font_t* text_font, ei_color_t*	text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t*	img_anchor){
    ei_frame_t* frame=(ei_frame_t*)widget;
    if(requested_size!=NULL){
        frame->widget.requested_size=*requested_size;
    }

    if(color!=NULL){
        frame->bg_color=*color;
    }

    if(border_width!=NULL){
        frame->border=*border_width;
    }
    else{
        frame->border=0;
    }

    if(relief!=NULL && border_width!=0){
        frame->relief=*relief;
    }
    else{
        frame->relief=ei_relief_none;
    }
    //assert(text!=NULL && img!=NULL);

    if(text_font!=NULL){
        frame->text_font=*text_font;
    }
    else{
        frame->text_font=ei_default_font;
    }

    if(text != NULL){
        if(*text){
            if(frame->text != NULL){
                frame->text = realloc(frame->text, strlen(*text) + 1);
            }else{
                frame->text = malloc(strlen(*text) + 1);
            }
            strcpy(frame->text, *text);

            ei_size_t text_size;
            hw_text_compute_size(frame->text, frame->text_font, &(text_size.width), &(text_size.height));

            if(frame->widget.requested_size.width < text_size.width){
                frame->widget.requested_size.width = text_size.width + 10;
            }

            if(frame->widget.requested_size.height < text_size.height){
                frame->widget.requested_size.height = text_size.height + 10;
            }

        }else{
            frame->text = NULL;
        }
    }

    if(text_color!=NULL){
        frame->text_color=*text_color;
    }
    else{
        frame->text_color=ei_font_default_color;
    }
    if(text_anchor!=NULL){
        frame->text_anchor=*text_anchor;
    }
    else{
        frame->text_anchor= ei_anc_center;
    }

    if(img != NULL){
        ei_size_t image_size = hw_surface_get_size(*img);
        frame->img = hw_surface_create(ei_app_root_surface(), image_size, EI_FALSE);
        ei_copy_surface(frame->img, NULL, *img, NULL, EI_FALSE);
    }


    if(img_anchor!=NULL){
        frame->img_anchor=*img_anchor;
    }
    else{
        frame->img_anchor=ei_anc_center;
    }

    if(img_rect != NULL){
        frame->img_rect = malloc(sizeof ( ei_rect_t ));
        frame->img_rect->size = (*img_rect)->size;
        frame->img_rect->top_left.x = (*img_rect)->top_left.x;
        frame->img_rect->top_left.y = (*img_rect)->top_left.y;
    }

    if (widget->content_rect->size.width < widget->requested_size.width){
        widget->content_rect->size.width = widget->requested_size.width;
    }

    if (widget->content_rect->size.height < widget->requested_size.height){
        widget->content_rect->size.height = widget->requested_size.height;
    }

    ei_app_invalidate_rect(&widget->screen_location);
}

void ei_toplevel_configure(ei_widget_t*		widget,
                                              ei_size_t*		requested_size,
                                              ei_color_t*		color,
                                              int*			border_width,
                                              char**			title,
                                              ei_bool_t*		closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_t**		min_size){
    ei_toplevel_t* toplevel=(ei_toplevel_t*)widget;
    if(requested_size!=NULL){
        toplevel->widget.requested_size=*requested_size;
    }

    if(color!=NULL){
        toplevel->bg_color=*color;
    }
    else{
        toplevel->bg_color=ei_default_background_color;
    }

    if(border_width!=NULL){
        toplevel->border=*border_width;
    }
    else{
        toplevel->border=0;
    }

    toplevel->widget.content_rect->top_left.x = toplevel->widget.screen_location.top_left.x + toplevel->border;
    toplevel->widget.content_rect->top_left.y = toplevel->widget.screen_location.top_left.y + k_top_size + toplevel->border;

    if(closable != NULL){
        toplevel->closable = *closable;
    }

    if(resizable != NULL){
        toplevel->resizable = *resizable;
    }


    if(min_size != NULL){
        toplevel->size_min = *min_size;
    }else if (!toplevel->size_min)
    {
        toplevel->size_min = calloc(1, sizeof(ei_size_t));
        toplevel->size_min->width = 160;
        toplevel->size_min->height = 120;
    }


    if(title!=NULL){
        toplevel->title= realloc(toplevel->title, strlen(*title) + 1);
        strcpy(toplevel->title, *title);
    }

    if (widget->content_rect->size.width < widget->requested_size.width){
        widget->content_rect->size.width = widget->requested_size.width;
    }

    if (widget->content_rect->size.height < widget->requested_size.height){
        widget->content_rect->size.height = widget->requested_size.height;
    }

    ei_app_invalidate_rect(&widget->screen_location);
}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t class_name, ei_widget_t* parent, void* user_data, ei_widget_destructor_t destructor){
    ei_widgetclass_t* w_class = ei_widgetclass_from_name(class_name);
    if (w_class) {
        ei_widget_t *widget = w_class->allocfunc();
        widget->wclass = w_class;

        widget->parent = parent;
        if (parent) {
            widget->screen_location.top_left.y = parent->content_rect->top_left.y;
            widget->screen_location.top_left.x = parent->content_rect->top_left.x;
            if (parent->children_head != NULL) {
                parent->children_tail->next_sibling = widget;
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
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
        widget->requested_size.width = 0;
        widget->requested_size.height = 0;
        ei_rect_t location = {0, 0};
        widget->content_rect = malloc(sizeof(ei_rect_t));
        widget->screen_location = location;
        widget->content_rect->top_left.x = widget->screen_location.top_left.x;
        widget->content_rect->top_left.y = widget->screen_location.top_left.y;
        widget->content_rect->size.width = widget->screen_location.size.width;
        widget->content_rect->size.height = widget->screen_location.size.height;
        w_class->setdefaultsfunc(widget);
        return widget;
    }
    return NULL;
}

void ei_widget_destroy(ei_widget_t*	widget){
    if(widget != NULL){
        if(widget->children_head){
            ei_widget_t *child = widget->children_head;
            while(child){
                ei_widget_t * sibling = child->next_sibling;
                if(child->destructor){
                    child->destructor(child);
                }

                ei_widget_destroy(child);
                child = sibling;
            }
        }

        if(widget->parent){
            ei_widget_t *parent = widget->parent;
            ei_widget_t *queue = parent->children_head;

            if(widget == parent->children_tail && widget == parent->children_head){
                parent->children_head = NULL;
                parent->children_tail = NULL;
            }else if(widget == parent->children_head){
                parent->children_head = widget->next_sibling;
            }else if(widget == parent->children_tail){
                while(queue->next_sibling != parent->children_tail){
                    queue = queue->next_sibling;
                }
                parent->children_tail = queue;
                parent->children_tail->next_sibling = NULL;
            }else{
                while(queue->next_sibling != widget){
                    queue = queue->next_sibling;
                }
                queue->next_sibling = widget->next_sibling;
            }
        }
        ei_app_invalidate_rect(&widget->screen_location);
        widget->wclass->releasefunc(widget);
        widget = NULL;
        set_destroy(EI_TRUE);
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



