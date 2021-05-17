/* ----------------------------------------------------------------------------- 
 * dpdcGui.h
 * ----------------------------------------------------------------------------- */


#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include<gtk/gtk.h>

/* Convenience macros for obtaining objects from UI file */
#define CH_GET_OBJECT(builder, name, type, data) \
		data->name = type(gtk_builder_get_object (builder, #name) )
#define CH_GET_WIDGET(builder, name, data) \
		CH_GET_OBJECT(builder, name, GTK_WIDGET, data)


/* ---------------------------------------------------------------- */
/*                         global Data Structure                    */
/* ---------------------------------------------------------------- */

/* Main data structure definition */
typedef struct _ChData ChData;

struct _ChData
{
	/* Main application window */
	GtkWidget *dpdc;  

	/* Main window button widgets */
	GtkWidget *add_pmu_button;
	GtkWidget *remove_pmu_button;
	GtkWidget *cmd_data_off_button;
	GtkWidget *cmd_data_on_button;
	GtkWidget *cmd_cfg_button;
	GtkWidget *add_pdc_button;
	GtkWidget *remove_pdc_button;
	GtkWidget *display_conn_table_button;
	GtkWidget *exit_button;
	
    	/* Manubar button widgets */
	GtkWidget *menubar;
	GtkWidget *open_dpdc_setup;
	GtkWidget *enter_pdc_detail_menuitem;
	GtkWidget *pdc_details_menuitem;
	GtkWidget *exit_menuitem;
	GtkWidget *about_menuitem;

	GtkWidget *menuitem2;
	GtkWidget *menu_add_source;
	GtkWidget *menu_remove_source;
	GtkWidget *menu_data_off;
	GtkWidget *menu_data_on;
	GtkWidget *menu_request_cfg;
	GtkWidget *menu_add_destination;
	GtkWidget *menu_remove_destination;
	GtkWidget *menu_conn_table;

    	/* Main window extra widgets */
	GtkWidget *admin_label;
	GtkWidget *rights_label;
	GtkWidget *time_lable;
	GtkWidget *text_view;
	GtkWidget *text_view1;
};

/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

/* Dtat Stucture object for DPDC */
ChData    *data;

char find_butn[2];
char buff[8000];
char fptr[200];
char *markup;
long int time_thrd_id;
int set_all;

GtkWidget *dpdc_setup_window;
GtkWidget *pdc_detail_window;
GtkWidget *add_pmu_window, *chkBtn;
GtkWidget *new_window, *setup_display_window;
GtkWidget *p_id, *p_udp, *p_tcp, *p_ip;
GtkWidget *p_port, *p_protocol, *waitT;
pthread_t show_sys_time;


/* ---------------------------------------------------------------- */
/*                 	Function prototypes   		            */
/* ---------------------------------------------------------------- */

int isNumber(char *s);

void destroy (GtkWidget *widget, gpointer udata);

GdkPixbuf *create_pixbuf(const gchar * filename);

void display_pdc_detail (GtkButton *widget, gpointer udata);

void about_dpdc (GtkButton *widget, gpointer udata);

void dpdc_help (GtkButton *but, gpointer udata);

void validation_result (char *msg);

void fill_pdc_details (char *filePath);

int validation_pdc_detail (GtkButton *button, gpointer udata);

void validation_setup_file_name (GtkWidget *widget, gpointer data);

void final_pdc_setup_call();

void add_pmu (GtkButton *but, gpointer udata);

int add_pmu_validation (GtkButton *but, gpointer udata);

void cmd_or_remove_pmu (GtkButton *but, gpointer udata);

void select_function(GtkWidget *widget, gpointer data);	//

int cmd_or_remove_pmu_validation (GtkButton *but, gpointer udata);

void add_new_pdc (GtkButton *but, gpointer udata);

int new_pdc_validation (GtkButton *but, gpointer udata);

void remove_pdc (GtkButton *but, gpointer udata);

int remove_pdc_validation (GtkButton *but, gpointer udata);

void connection_table (GtkButton *but, gpointer udata);

void dpdc_setup_fileSelector (GtkWidget *widget, gpointer udata);

void view_setup_file (GtkWidget *w, GtkFileChooser *fs);

void apply_dpdc_setup (GtkWidget *w, gpointer udata);

void* display_time();

#endif 

/**************************************** End of File *******************************************************/
