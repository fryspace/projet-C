#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


// /!\ IL MANQUE : pour le button : pb quand on mets pas de radius (n'affiche pas) de même quand pas de height et pas de rel_height
//                                 ou de width et pas de rel_width (manque une valeur par défaut?)
//              clipper enfant dans le parent

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_event_t* event)
{
	if (event->type == ei_ev_keydown)
		if (event->param.key.key_code == SDLK_ESCAPE) {
			ei_app_quit_request();
			return EI_TRUE;
		}

	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t	screen_size		= {800, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button;
	ei_anchor_t	button_anchor		= ei_anc_southeast;
	int		button_x		= -20;
	int		button_y		= -20;
	ei_size_t button_size = {150,100};
	float		button_rel_x		= (float)1.0;
	float		button_rel_y		= (float)1.0;
	float		button_rel_width	= (float)0.5;
    float		button_rel_height	= (float)0.5;
	int         radius              = 40;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "click";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 2;
	ei_callback_t	button_callback 	= button_press;

	ei_widget_t*	window;
	ei_widget_t *   window2;
	ei_size_t	window_size		= {320,240};
    ei_size_t	window_size2		= {160,160};
	char*		window_title		= "Hello World";
    char*		window_title2		= "Hello Again";
	ei_color_t	window_color		= {0xA0,0xA0,0xA0, 0xff};
    ei_color_t	window_color2		= {0x80,0x00,0xb0, 0xff};
	int		window_border_width	= 2;
	ei_bool_t	window_closable		= EI_TRUE;
	ei_axis_set_t	window_resizable	= ei_axis_both;
    ei_axis_set_t	window_resizable2	= ei_axis_x;
	ei_point_t	window_position		= {30, 10};
    ei_point_t	window_position2		= {100, 110};
	ei_anchor_t anchor_window2      = ei_anc_north;

	
	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE); 
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_event_set_default_handle_func(process_key);


	/* Create, configure and place a toplevel window on screen. */
	window = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure(window, &window_size, &window_color, &window_border_width,
				&window_title, &window_closable, &window_resizable, NULL);
	ei_place(window, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);


	/* Create, configure and place a button as a descendant of the toplevel window. */
	button = ei_widget_create("button", window, NULL, NULL);
	ei_button_configure	(button, NULL, &button_color,
				 &button_border_width, &radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
				NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_width, &button_rel_height);


    /* Create, configure and place a toplevel window as a descendant of the first toplevel window. */
    window2 = ei_widget_create("toplevel", window, NULL, NULL);
    ei_toplevel_configure(window2, &window_size2, &window_color2, &window_border_width,
                          &window_title2, &window_closable, &window_resizable2, NULL);
    ei_place(window2, &anchor_window2, &(window_position2.x), &(window_position2.y), NULL, NULL, NULL, NULL, NULL, NULL);


    /* Run the application's main loop. */
	ei_app_run();

	ei_app_free();

	return (EXIT_SUCCESS);
}
