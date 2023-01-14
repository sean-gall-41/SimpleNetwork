#include "gui.h"
#include "spike_gen.h"

struct gui application = {};

static void null_callback(GtkWidget *widget, gpointer data) {}

static void on_run_sim(GtkWidget *widget, struct cell *model_cell)
{
	init_rng();

	float times[NUM_TS]    = {0.0};
	uint8_t spikes[NUM_TS] = {0};

	printf("Simulating...\n");
	generate_spikes(spikes, NUM_TS);
	calc_cell_activity(model_cell, spikes, times, 0.0, 1.0);
	print_spikes(model_cell->spikes, model_cell->num_ts);
	gtk_widget_queue_draw(application.drawing_area);

	printf("Done simulating.\n");
}

static void on_draw(GtkWidget *drawing_area, cairo_t *cr, struct cell *model_cell)
{
	// background color setup
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	/* GtkDrawingArea size */
	GdkRectangle da;            
	GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(drawing_area));

	gdk_window_get_geometry(window,
		&da.x,
		&da.y,
		&da.width,
		&da.height);

	float len_scale_y   = model_cell->e_leak - model_cell->e_thresh;
	float coord_scale_y = -da.height / (2.0 * len_scale_y); 
	float coord_scale_x = da.width / (float)(NUM_TS / 3.0);

	cairo_scale(cr, coord_scale_x, -coord_scale_y);
	cairo_translate(cr, 0, 46.0); 

	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	for (int i = 0; i < NUM_TS; i++)
	{
		cairo_rectangle(cr, i, model_cell->voltage[i], 1.0, 0.05);
		if (model_cell->spikes[i])
		{
			cairo_rectangle(cr, i, model_cell->voltage[i], 1.0, 1.0);
		}
		cairo_fill(cr);
	}
}

int gui_init(int *argc, char ***argv, struct cell *model_cell)
{
	// Initialize GTK:
	if (!gtk_init_check(argc, argv)) {
		fprintf(stderr, "Could not initialize GTK\n");
		return 1;
	}

	application.window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_window_set_title(GTK_WINDOW(application.window), "Simple Sim");
	gtk_window_set_default_size(GTK_WINDOW(application.window), MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(application.window), GTK_WIN_POS_CENTER);
	gtk_widget_add_events(application.window, GDK_DELETE);
	g_signal_connect(application.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	application.horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	application.vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

	application.run_sim_button = gtk_button_new();
	gtk_button_set_label(GTK_BUTTON(application.run_sim_button), "run simulation");
	gtk_box_pack_start(GTK_BOX(application.vertical_box), application.run_sim_button, FALSE, FALSE, 1);
	g_signal_connect(application.run_sim_button, "clicked", on_run_sim, model_cell);
	gtk_box_pack_start(GTK_BOX(application.horizontal_box), application.vertical_box, FALSE, FALSE, 2);

	application.drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(application.drawing_area,
								MAIN_WINDOW_WIDTH - 100,
								MAIN_WINDOW_HEIGHT);
	g_signal_connect(G_OBJECT(application.drawing_area), "draw", on_draw, model_cell);
	gtk_box_pack_end(GTK_BOX(application.horizontal_box), application.drawing_area, FALSE, FALSE, 2);

	gtk_container_add(GTK_CONTAINER(application.window), application.horizontal_box);
	gtk_widget_show_all(application.window);
	return 0;
}

int gui_run(void)
{
	gtk_main();
	return 0;
}

