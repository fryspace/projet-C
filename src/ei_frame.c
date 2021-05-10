//
// Created by bossu on 07/05/2021.
//
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include "bg_utils.h"
#include <string.h>
#include <malloc.h>

ei_widgetclass_t frame;

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_frame_t  *frame = (ei_frame_t*) widget;

    ei_color_t top_color, bottom_color;
    modify_color(frame->bg_color, &top_color, 1.5);
    modify_color(frame->bg_color, &bottom_color, 0.5);

   // ei_point_t top_left = {widget->screen_location.top_left.x + frame->border, widget->screen_location.top_left.y + frame->border};
    //ei_size_t rect_size = {widget->screen_location.size.width - 2 * frame->border, widget->screen_location.size.height - 2 * frame->border};

   // ei_rect_t rect = {top_left, rect_size};


   ei_fill(surface, frame->bg_color, &widget->screen_location);
   ei_fill(pick_surface, ((ei_frame_t*)widget) -> bg_color, clipper);




}

void* frame_allocfunc(){
    ei_frame_t *frame = calloc(1, sizeof(ei_frame_t));
    frame->border = calloc(1, sizeof(int));
    frame->bg_color = calloc(1, sizeof (ei_color_t));
    frame->relief = calloc(1, sizeof (ei_relief_t));

    frame->text_font = calloc(1, sizeof (ei_font_t));
    frame->text_color = calloc(1, sizeof (ei_color_t));
    frame->text_anchor = calloc(1, sizeof (ei_anchor_t));

    frame->img = calloc(1, sizeof (ei_surface_t));
    frame->img_rect = calloc(1, sizeof (ei_rect_t));
    frame->img_anchor = calloc(1, sizeof (ei_anchor_t));

    return frame;
}

void frame_release_func(struct ei_widget_t* widget){
    free(((ei_frame_t*)widget)->bg_color);
    free(((ei_frame_t*)widget)->border);
    free(((ei_frame_t*)widget)->relief);
    free(((ei_frame_t*)widget)->text);
    free(((ei_frame_t*)widget)->text_font);
    free(((ei_frame_t*)widget)->text_color);
    free(((ei_frame_t*)widget)->text_anchor);
    free(((ei_frame_t*)widget)->img);
    free(((ei_frame_t*)widget)->img_rect);
    free(((ei_frame_t*)widget)->img_anchor);
}

void frame_setdefaultsfunc(struct ei_widget_t* widget){
    *(((ei_frame_t*)widget)->bg_color) = ei_default_background_color;
    *(((ei_frame_t*)widget)->border) = 0;
    *(((ei_frame_t*)widget)->relief) = ei_relief_none;

    ((ei_frame_t*)widget)->text = NULL;
    *(((ei_frame_t*)widget)->text_font) = ei_default_font;
    *(((ei_frame_t*)widget)->text_color) = ei_font_default_color;
    *(((ei_frame_t*)widget)->text_anchor) = ei_anc_center;

    ((ei_frame_t*)widget)->img = NULL;
    ((ei_frame_t*)widget)->img_rect = NULL;
    *(((ei_frame_t*)widget)->img_anchor) = ei_anc_center;

}

void frame_register_class (){
    strcpy(frame.name, "frame");
    frame.drawfunc = &frame_drawfunc;
    frame.allocfunc = &frame_allocfunc;
    frame.releasefunc = &frame_release_func;
    frame.setdefaultsfunc = &frame_setdefaultsfunc;
    ei_widgetclass_register(&frame);
}