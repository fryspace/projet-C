#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_button.h"
#include "ei_event.h"
#include "bg_button.h"
#include "bg_utils.h"
#include <string.h>
#include <malloc.h>

ei_widgetclass_t button;

void button_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_button_t  *button = (ei_button_t*) widget;

    if(button->relief == ei_relief_none){
        ei_draw_button(widget->screen_location, button->border, surface, pick_surface, button->corner_radius, button->bg_color, *(widget->pick_color), 0, clipper);
    }

    if(button->relief == ei_relief_raised){
        ei_draw_button(widget->screen_location, button->border, surface, pick_surface, button->corner_radius, button->bg_color, *(widget->pick_color), 0, clipper);
    }

    if(button->relief == ei_relief_sunken){
        ei_draw_button(widget->screen_location, button->border, surface, pick_surface, button->corner_radius, button->bg_color, *(widget->pick_color), 1, clipper);
    }

    if (button->text != NULL){

        ei_point_t text_position;
        ei_size_t text_size;
        hw_text_compute_size(button->text, button->text_font, &(text_size.width), &(text_size.height));
        ei_anchor(button->text_anchor, &text_size, &widget->screen_location, &text_position);

        if(button->relief == ei_relief_sunken) {
            text_position.x += button->border/2;
            text_position.y += button->border/2;
        }
        ei_draw_text(surface, &text_position, button->text, button->text_font, button->text_color, clipper);
    }

    if(button->img != NULL){
        ei_point_t image_position;
        ei_size_t image_size;
        ei_rect_t image_rect;

        if (button->img_rect)
        {
            image_rect = *(button->img_rect);
            image_size = button->img_rect->size;
        }
        else
        {
            image_rect = hw_surface_get_rect(button->img);
            image_size = image_rect.size;
        }
        ei_anchor(button->img_anchor, &image_size, clipper, &image_position);
        ei_rect_t rect_img = {image_position, image_size};

        hw_surface_lock(button->img);
        ei_bool_t alpha_img = hw_surface_has_alpha(button->img);
        ei_rect_t surface_rect = hw_surface_get_rect(ei_app_root_surface());

        ei_rect_t image_clipper;
        ei_intersection(&button->widget.screen_location, &surface_rect, &image_clipper);

        ei_rect_t final_clipper;
        ei_intersection(&image_clipper, &rect_img, &final_clipper);

        rect_img.top_left.x = image_rect.top_left.x + final_clipper.top_left.x - image_position.x;
        rect_img.top_left.y = image_rect.top_left.y;
        rect_img.size = final_clipper.size;
        ei_copy_surface(surface, &final_clipper, button->img, &rect_img, alpha_img);
        hw_surface_unlock(button->img);
    }

}

void* button_allocfunc(){
    ei_button_t *button = calloc(1, sizeof(ei_button_t));

    button->img = calloc(1, sizeof (ei_surface_t));
    button->img_rect = calloc(1, sizeof (ei_rect_t));

    button->callback = calloc(1, sizeof (ei_callback_t));

    return button;
}

void button_release_func(struct ei_widget_t* widget){
    free(((ei_button_t*)widget));
}

void button_setdefaultsfunc(struct ei_widget_t* widget){
    (((ei_button_t*)widget)->bg_color) = ei_default_background_color;
    ((ei_button_t*)widget)->border = k_default_button_border_width;
    (((ei_button_t*)widget)->relief) = ei_relief_none;

    ((ei_button_t*)widget)->text = NULL;
    (((ei_button_t*)widget)->text_font) = ei_default_font;
    (((ei_button_t*)widget)->text_color) = ei_font_default_color;
    (((ei_button_t*)widget)->text_anchor) = ei_anc_center;

    ((ei_button_t*)widget)->img = NULL;
    ((ei_button_t*)widget)->img_rect = NULL;
    (((ei_button_t*)widget)->img_anchor) = ei_anc_center;

    ((ei_button_t*)widget)->corner_radius = k_default_button_corner_radius;
    ((ei_button_t*)widget)->callback = NULL;
    ((ei_button_t*)widget)->user_param = NULL;

}

ei_bool_t button_handle_func(ei_widget_t* widget, ei_event_t* event){
    ei_button_t *button = ((ei_button_t*)widget);

    if(event->type == ei_ev_mouse_buttondown){
        button->relief = ei_relief_sunken;
        ei_event_set_active_widget(widget);
    }

    if(event->type == ei_ev_mouse_buttonup){
        button->relief = ei_relief_raised;
        ei_event_set_active_widget(NULL);
        if(point_in_rectangle(event->param.mouse.where, widget->screen_location) == EI_TRUE){
            if(button->callback != NULL) {
                (button->callback)(widget, event, button->user_param);
            }
        }
    }

}

void button_geomnotifyfunc(ei_widget_t*	widget, ei_rect_t rect){
    widget->screen_location = rect;
}

void button_register_class (){
    strcpy(button.name, "button");
    button.drawfunc = &button_drawfunc;
    button.allocfunc = &button_allocfunc;
    button.releasefunc = &button_release_func;
    button.handlefunc = &button_handle_func;
    button.geomnotifyfunc = &button_geomnotifyfunc;
    button.setdefaultsfunc = &button_setdefaultsfunc;
    ei_widgetclass_register(&button);
}