#ifndef GUI_H_
#define GUI_H_

#include <gtk/gtk.h>
#include "cell.h"

#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 600

struct gui
{
	GtkWidget *window;
	GtkWidget *horizontal_box;
	GtkWidget *vertical_box;
	GtkWidget *run_sim_button;
	GtkWidget *drawing_area;
};

extern struct gui application;

int gui_init(int *argc, char ***argv, struct cell *model_cell);
int gui_run(void);

#endif /* GUI_H_ */

