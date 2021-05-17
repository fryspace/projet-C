//
// Created by bossu on 07/05/2021.
//
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_application.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include "bg_utils.h"
#include "ei_event.h"
#include <string.h>
#include <malloc.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

ei_widgetclass_t frame;

ei_linked_point_t* ei_flat_frame(ei_rect_t rect, int part){
    ei_linked_point_t *result;

    int width = rect.size.width;
    int height = rect.size.height;

    ei_point_t mid_right, mid_left, top_right, bottom_left, bottom_right;
    ei_linked_point_t* first_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* top_left_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* top_right_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* bottom_left_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* bottom_right_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* mid_right_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* mid_left_point = malloc(sizeof(ei_linked_point_t));

    int h;

    switch(part){
        // If we want the whole rectangle
        case 0:
            top_right.x = rect.top_left.x + width;
            top_right.y = rect.top_left.y;
            bottom_right.x = rect.top_left.x + width;
            bottom_right.y = rect.top_left.y + height;
            bottom_left.x = rect.top_left.x;
            bottom_left.y = rect.top_left.y + height;

            first_point->point = rect.top_left;
            first_point->next = NULL;

            top_left_point->point = rect.top_left;
            top_left_point->next = top_right_point;

            top_right_point->point = top_right;
            top_right_point->next = bottom_right_point;

            bottom_right_point->point = bottom_right;
            bottom_right_point->next = bottom_left_point;

            bottom_left_point->point = bottom_left;
            bottom_left_point->next = first_point;


            result = top_left_point;
            break;
            // If we want the bottom rectangle
        case 2:

            h = (int)MIN(height/2, width/2);

            top_right.x = rect.top_left.x + width;
            top_right.y = rect.top_left.y;
            bottom_right.x = rect.top_left.x + width;
            bottom_right.y = rect.top_left.y + height;
            bottom_left.x = rect.top_left.x;
            bottom_left.y = rect.top_left.y + height;

            first_point->point = top_right;
            first_point->next = NULL;

            top_right_point->point = top_right;
            top_right_point->next = bottom_right_point;

            bottom_right_point->point = bottom_right;
            bottom_right_point->next = bottom_left_point;

            bottom_left_point->point = bottom_left;
            bottom_left_point->next = mid_left_point;

            mid_left.x = rect.top_left.x + h;
            mid_left.y = rect.top_left.y + height - h;

            mid_left_point->point = mid_left;
            mid_left_point->next = mid_right_point;

            mid_right.x = rect.top_left.x + width - h;
            mid_right.y = rect.top_left.y + h;

            mid_right_point->point = mid_right;
            mid_right_point->next = first_point;




            result = top_right_point;
            break;
            // Top rectangle
        case 1:
            h = (int)MIN(height/2, width/2);

            top_right.x = rect.top_left.x + width;
            top_right.y = rect.top_left.y;
            bottom_left.x = rect.top_left.x;
            bottom_left.y = rect.top_left.y + height;

            first_point->point = rect.top_left;
            first_point->next = NULL;

            top_left_point->point = rect.top_left;
            top_left_point->next = top_right_point;

            top_right_point->point = top_right;
            top_right_point->next = mid_right_point;

            mid_left.x = rect.top_left.x + h;
            mid_left.y = rect.top_left.y + height - h;

            mid_left_point->point = mid_left;
            mid_left_point->next = bottom_left_point;

            mid_right.x = rect.top_left.x + width - h;
            mid_right.y = rect.top_left.y + h;

            mid_right_point->point = mid_right;
            mid_right_point->next = mid_left_point;


            bottom_left_point->point = bottom_left;
            bottom_left_point->next = first_point;


            result = top_left_point;

            break;
        default:
            exit(1);
    }

    return result;
}

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_frame_t  *frame = (ei_frame_t*) widget;

    ei_rect_t rect = widget->screen_location;

    ei_color_t top_color, bottom_color;

    if(frame->relief == ei_relief_raised){
        modify_color(&frame->bg_color, &top_color, 1.5);
        modify_color(&frame->bg_color, &bottom_color, 0.5);

        ei_point_t little_rect_point;
        little_rect_point.x = rect.top_left.x + frame->border;
        little_rect_point.y = rect.top_left.y + frame->border;

        ei_size_t little_rect_size;
        little_rect_size.width = rect.size.width - 2* frame->border;
        little_rect_size.height = rect.size.height - 2* frame->border;

        ei_rect_t little_rect = {little_rect_point, little_rect_size};

        ei_linked_point_t *main = ei_flat_frame(little_rect, 0);
        ei_linked_point_t *pick_points = ei_flat_frame(rect, 0);
        ei_linked_point_t *top = ei_flat_frame(rect, 1);
        ei_linked_point_t *bottom = ei_flat_frame(rect, 2);

        ei_draw_polygon(pick_surface, pick_points, *(widget->pick_color), clipper);
        ei_draw_polygon(surface, top, top_color, clipper);
        ei_draw_polygon(surface, bottom, bottom_color, clipper);
        ei_draw_polygon(surface, main, frame->bg_color, &rect);
    }

    if(frame->relief == ei_relief_sunken){
        modify_color(&frame->bg_color, &top_color, 0.5);
        modify_color(&frame->bg_color, &bottom_color, 1.5);

        ei_point_t little_rect_point;
        little_rect_point.x = rect.top_left.x + frame->border;
        little_rect_point.y = rect.top_left.y + frame->border;

        ei_size_t little_rect_size;
        little_rect_size.width = rect.size.width - 2* frame->border;
        little_rect_size.height = rect.size.height - 2* frame->border;

        ei_rect_t little_rect = {little_rect_point, little_rect_size};

        ei_linked_point_t *main = ei_flat_frame(little_rect, 0);
        ei_linked_point_t *pick_points = ei_flat_frame(rect, 0);
        ei_linked_point_t *top = ei_flat_frame(rect, 1);
        ei_linked_point_t *bottom = ei_flat_frame(rect, 2);

        ei_draw_polygon(pick_surface, pick_points, *(widget->pick_color), clipper);
        ei_draw_polygon(surface, top, top_color, clipper);
        ei_draw_polygon(surface, bottom, bottom_color, clipper);
        ei_draw_polygon(surface, main, frame->bg_color, &rect);
    }

    if(frame->relief == ei_relief_none){
        ei_fill(surface, &frame->bg_color, &rect);
        ei_fill(pick_surface, widget->pick_color, &rect);
    }

    if(frame->text != NULL && strcmp(frame->text, "") != 0){
        ei_point_t text_position;
        ei_size_t text_size = {0, 0};
        hw_text_compute_size(frame->text, frame->text_font, &(text_size.width), &(text_size.height));
        ei_anchor(frame->text_anchor, &text_size, clipper, &text_position);
        ei_draw_text(surface, &text_position, frame->text, frame->text_font, frame->text_color, clipper);
    }

    if(frame->img != NULL){
        ei_point_t image_position;
        ei_size_t image_size;
        ei_rect_t image_rect;

        if (frame->img_rect)
        {
            image_rect = *(frame->img_rect);
            image_size = frame->img_rect->size;
        }
        else
        {
            image_rect = hw_surface_get_rect(frame->img);
            image_size = image_rect.size;
        }
        ei_anchor(frame->img_anchor, &image_size, clipper, &image_position);
        ei_rect_t rect_img = {image_position, image_size};

        hw_surface_lock(frame->img);
        ei_bool_t alpha_img = hw_surface_has_alpha(frame->img);
        ei_rect_t surface_rect = hw_surface_get_rect(ei_app_root_surface());

        ei_rect_t image_clipper;
        ei_intersection(&frame->widget.screen_location, &surface_rect, &image_clipper);

        ei_rect_t final_clipper;
        ei_intersection(&image_clipper, &rect_img, &final_clipper);

        rect_img.top_left.x = image_rect.top_left.x + final_clipper.top_left.x - image_position.x;
        rect_img.top_left.y = image_rect.top_left.y;
        rect_img.size = final_clipper.size;
        ei_copy_surface(surface, &final_clipper, frame->img, &rect_img, alpha_img);
        hw_surface_unlock(frame->img);
    }


}

void* frame_allocfunc(){
    ei_frame_t *frame = calloc(1, sizeof(ei_frame_t));
    /*
    frame->border = calloc(1, sizeof(int));
    frame->bg_color = calloc(1, sizeof (ei_color_t));
    frame->relief = calloc(1, sizeof (ei_relief_t));

    frame->text_font = calloc(1, sizeof (ei_font_t));
    frame->text_color = calloc(1, sizeof (ei_color_t));
    frame->text_anchor = calloc(1, sizeof (ei_anchor_t));

    frame->img = calloc(1, sizeof (ei_surface_t));
    frame->img_rect = calloc(1, sizeof (ei_rect_t));
    frame->img_anchor = calloc(1, sizeof (ei_anchor_t));
*/
    return frame;
}

void frame_release_func(struct ei_widget_t* widget){
    free(((ei_frame_t*)widget));
}

void frame_setdefaultsfunc(struct ei_widget_t* widget){
    (((ei_frame_t*)widget)->bg_color) = ei_default_background_color;
    (((ei_frame_t*)widget)->border) = 0;
    (((ei_frame_t*)widget)->relief) = ei_relief_none;

    ((ei_frame_t*)widget)->text = NULL;
    (((ei_frame_t*)widget)->text_font) = ei_default_font;
    (((ei_frame_t*)widget)->text_color) = ei_font_default_color;
    (((ei_frame_t*)widget)->text_anchor) = ei_anc_center;

    ((ei_frame_t*)widget)->img = NULL;
    ((ei_frame_t*)widget)->img_rect = NULL;
    (((ei_frame_t*)widget)->img_anchor) = ei_anc_center;

}

void frame_handlefunc(ei_widget_t* widget, ei_event_t* event){


}

void frame_register_class (){
    strcpy(frame.name, "frame");
    frame.drawfunc = &frame_drawfunc;
    frame.allocfunc = &frame_allocfunc;
    frame.handlefunc = &frame_handlefunc;
    frame.releasefunc = &frame_release_func;
    frame.setdefaultsfunc = &frame_setdefaultsfunc;
    ei_widgetclass_register(&frame);
}