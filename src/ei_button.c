#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_button.h"
#include "ei_event.h"
#include "bg_button.h"
#include "bg_utils.h"
#include <string.h>
#include <malloc.h>

ei_widgetclass_t button;

void button_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_button_t  *button = (ei_button_t*) widget;

    if(*(button->relief) == ei_relief_none){
        ei_draw_button(widget->screen_location, *(button->border), surface, pick_surface, *button->corner_radius, *button->bg_color, *(widget->pick_color), 0, clipper);
    }

    if(*(button->relief) == ei_relief_raised){
        ei_draw_button(widget->screen_location, *(button->border), surface, pick_surface, *button->corner_radius, *button->bg_color, *(widget->pick_color), 0, clipper);
    }

    if(*(button->relief) == ei_relief_sunken){
        ei_draw_button(widget->screen_location, *(button->border), surface, pick_surface, *button->corner_radius, *button->bg_color, *(widget->pick_color), 1, clipper);
    }

    if (button->text != NULL){

        ei_point_t text_position;
        ei_size_t text_size;
        hw_text_compute_size(*(button->text), button->text_font, &(text_size.width), &(text_size.height));
        ei_anchor(*(button->text_anchor), &text_size, clipper, &text_position);

        if(button->relief == ei_relief_sunken) {
            text_position.x += *(button->border)/2;
            text_position.y += *(button->border)/2;
        }
        ei_draw_text(surface, &text_position, *(button->text), button->text_font, *(button->text_color), clipper);
    }

}

void* button_allocfunc(){
    ei_button_t *button = calloc(1, sizeof(ei_button_t));
    button->border = calloc(1, sizeof(int));
    button->corner_radius = calloc(1, sizeof(int));
    button->bg_color = calloc(1, sizeof (ei_color_t));
    button->relief = calloc(1, sizeof (ei_relief_t));

    button->text_font = calloc(1, sizeof (ei_font_t));
    button->text_color = calloc(1, sizeof (ei_color_t));
    button->text_anchor = calloc(1, sizeof (ei_anchor_t));

    button->img = calloc(1, sizeof (ei_surface_t));
    button->img_rect = calloc(1, sizeof (ei_rect_t));
    button->img_anchor = calloc(1, sizeof (ei_anchor_t));

    button->callback = calloc(1, sizeof (ei_callback_t));

    return button;
}

void button_release_func(struct ei_widget_t* widget){
    free(((ei_button_t*)widget)->bg_color);
    free(((ei_button_t*)widget)->border);
    free(((ei_button_t*)widget)->corner_radius);
    free(((ei_button_t*)widget)->relief);
    free(((ei_button_t*)widget)->text);
    free(((ei_button_t*)widget)->text_font);
    free(((ei_button_t*)widget)->text_color);
    free(((ei_button_t*)widget)->text_anchor);
    free(((ei_button_t*)widget)->img);
    free(((ei_button_t*)widget)->img_rect);
    free(((ei_button_t*)widget)->img_anchor);

    free(((ei_button_t*)widget)->callback);
}

void button_setdefaultsfunc(struct ei_widget_t* widget){
    *(((ei_button_t*)widget)->bg_color) = ei_default_background_color;
    *(((ei_button_t*)widget)->border) = 0;
    *(((ei_button_t*)widget)->relief) = ei_relief_none;

    ((ei_button_t*)widget)->text = NULL;
    *(((ei_button_t*)widget)->text_font) = ei_default_font;
    *(((ei_button_t*)widget)->text_color) = ei_font_default_color;
    *(((ei_button_t*)widget)->text_anchor) = ei_anc_center;

    ((ei_button_t*)widget)->img = NULL;
    ((ei_button_t*)widget)->img_rect = NULL;
    *(((ei_button_t*)widget)->img_anchor) = ei_anc_center;

    ((ei_button_t*)widget)->corner_radius = 40;
    ((ei_button_t*)widget)->callback = NULL;
    ((ei_button_t*)widget)->user_param = NULL;

}

ei_bool_t button_handle_func(ei_widget_t* widget, ei_event_t* event){
    ei_button_t *button = ((ei_button_t*)widget);

    if(event->type == ei_ev_mouse_buttondown){
        *button->relief = ei_relief_sunken;
    }

    if(event->type == ei_ev_mouse_buttonup){
        *button->relief = ei_relief_raised;
        (*button->callback)(widget, event, button->user_param);
    }

}

void button_register_class (){
    strcpy(button.name, "button");
    button.drawfunc = &button_drawfunc;
    button.allocfunc = &button_allocfunc;
    button.releasefunc = &button_release_func;
    button.handlefunc = &button_handle_func;
    button.setdefaultsfunc = &button_setdefaultsfunc;
    ei_widgetclass_register(&button);
}