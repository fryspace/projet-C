#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "bg_button.h"



/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[2];

	pts[0].point.x = 200; pts[0].point.y = 200; pts[0].next = &pts[1];
	pts[1].point.x = 600; pts[1].point.y = 400; pts[1].next = NULL;
	
	ei_draw_polyline(surface, pts, color, clipper);
}

void test_button(ei_surface_t surface, ei_rect_t* clipper)
{
    ei_color_t		color		= { 150, 0, 160, 255 };
    ei_point_t	top_left1 = {100, 200};
    ei_point_t	top_left2 = {450, 200};
    ei_size_t size = {300, 200};

    ei_rect_t rect1 = {top_left1, size};
    ei_rect_t rect2 = {top_left2, size};

    ei_draw_button(rect1, surface, 40, color, 0, clipper);
    ei_draw_button(rect2, surface, 40, color, 1, clipper);
}

/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_heart(ei_surface_t surface, ei_rect_t* clipper)
{
    ei_color_t		color		= { 255, 0, 0, 255 };
    ei_point_t center1 = {360, 300};
    ei_point_t center2 = {440, 300};
    ei_linked_point_t arc;
    ei_linked_point_t *arc1 = &arc;
    ei_linked_point_t arcb;
    ei_linked_point_t *arc2 = &arcb;

    arc1 = ei_arc(center1, 50, 35, 200, 2000);
    arc2 = ei_arc(center2, 50, -10, 145, 2000);

    ei_linked_point_t	pts[3];

    pts[0].point.x = 310; pts[0].point.y = 315; pts[0].next = &pts[1];
    pts[1].point.x = 400; pts[1].point.y = 450; pts[1].next = &pts[2];
    pts[2].point.x = 490; pts[2].point.y = 305; pts[2].next = NULL;

    ei_draw_polyline(surface, pts, color, clipper);

    ei_draw_polyline(surface, arc1, color, clipper);
    ei_draw_polyline(surface, arc2, color, clipper);
}



/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 255, 0, 255 };
	ei_linked_point_t	pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 400;
	pts[0].point.y = 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
		ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polygon(surface, pts, color, clipper);
}



/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 0, 255 };
	ei_linked_point_t	pts[5];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 300;
	pts[0].point.y = 400;

	/* Draw the square */
	for(i = 1; i <= 4; i++) {
		/*	Add or remove 200 pixels or 0 for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 0 or 1, according to which coordinate grows
			The third term is simply the side of the square */
		xdiff = pow(-1, i / 2) * (i % 2) * 200;
		ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polygon(surface, pts, color, clipper);
}



/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special 
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 0, 255 };
	ei_linked_point_t	pts[3];

	pts[0].point.x = 400; pts[0].point.y = 300; pts[0].next = &(pts[1]);
	pts[1].point.x = 400; pts[1].point.y = 300; pts[1].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);
}



/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
	//ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
	//clipper_ptr		= &clipper;
	ei_event_t		event;

	hw_init();
		
	main_window = hw_create_window(win_size, EI_FALSE);
	
	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	/* Draw polylines. */
    test_button(main_window, clipper_ptr);
	//test_line(main_window, clipper_ptr);
    //test_heart(main_window, clipper_ptr);
	//test_octogone	(main_window, clipper_ptr);
	//test_square	(main_window, clipper_ptr);
	//test_dot	(main_window, clipper_ptr);
	
	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown)
		hw_event_wait_next(&event);

	hw_quit();
	return (EXIT_SUCCESS);
}
