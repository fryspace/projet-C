#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"

#include "ei_frame.h"

// /!\ IL MANQUE : tests images

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
    ei_widget_t*	frame2;
    ei_widget_t*    frame3;
	ei_size_t	frame_size		= {300,200};
    ei_size_t	frame_size2		= {200,100};
	int		frame_x			= 150;
	int		frame_y			= 200;
    int		frame_x2			= 150;
    int		frame_y2			= 200;
    float   frame_rx3           =(float)0.5;
    float   frame_ry3           =(float)0.5;
    float   frame_rw3           =(float)0.2;
    float   frame_rh3           =(float)0.3;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_none;
    ei_color_t	frame_color2		= {0x80, 0x00, 0xb0, 0xff};
    ei_color_t	frame_color3		= {0x00, 0xb4, 0x80, 0xff};
    ei_relief_t	frame_relief2		= ei_relief_raised;
	int		frame_border_width	= 4;
    char*		text		= "Mon magnifique frame !";
    ei_color_t	text_color	= {0xff, 0xff, 0xff, 0xff};
    ei_anchor_t anchor_text=ei_anc_north;
    ei_anchor_t anchor = ei_anc_center;

	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);


	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);


	/* Create, configure and place a gray frame on screen with a text and anchor_text default. */
	frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure	(frame, &frame_size, &frame_color,&frame_border_width, &frame_relief, &text, NULL, &text_color,
                        NULL,NULL, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );


    /* Create, configure and place a purple frame on screen within the last frame (its parent) and a text. */
    frame2 = ei_widget_create("frame", frame, NULL, NULL);
    ei_frame_configure	(frame2, &frame_size2, &frame_color2,&frame_border_width, &frame_relief2, &text , NULL, &text_color,
                           &anchor_text,NULL, NULL, NULL);
    ei_place(frame2, &anchor, &frame_x2, &frame_y2, NULL, NULL, NULL, NULL, NULL, NULL );


    /* Create, configure and place a aqua (color vert d'eau) frame on screen within the first frame (its parent). */
    frame3 = ei_widget_create("frame", frame, NULL, NULL);
    ei_frame_configure	(frame3, &frame_size2, &frame_color3,&frame_border_width, &frame_relief2, NULL , NULL, NULL,
                           NULL,NULL, NULL, NULL);
    ei_place(frame3, &anchor, 0, 0, NULL, NULL, &frame_rx3, &frame_ry3, &frame_rw3, &frame_rh3);


    /* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
