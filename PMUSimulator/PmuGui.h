 ----------------------------------------------------------------------------- 
 * PmuGui.h
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

/* PMU's main data structure definition */
typedef struct _pmuStruct pmuStruct;

struct _pmuStruct
{
     /* Main application window */
	GtkWidget *Pmu_Simulator;  

     /* Main window button widgets */
	GtkWidget *create_cfg_button;
	GtkWidget *header_frm_button;
	GtkWidget *pmu_details_button;
	GtkWidget *stat_modification_button;
	GtkWidget *cfg_modification_button;
	GtkWidget *manage_data_source;

     /* Manubar button widgets */
	GtkWidget *pmu_menubar;
	GtkWidget *start_server;
	GtkWidget *open_cfg;
	GtkWidget *pmu_properties;
	GtkWidget *exit_menuitem;
	GtkWidget *about_menuitem;

	GtkWidget * menu_setup_cfg;
	GtkWidget * menu_data_source;
	GtkWidget * menu_cfg_modify;
	GtkWidget * menu_stat_modify;
	GtkWidget * menu_header_frm;

     /* Main window extra widgets */
	GtkWidget *rights_lable;
	GtkWidget *admin_label;
	GtkWidget *img_label;
	GtkWidget *logo_butun;
	GtkWidget *footer_label;
	GtkWidget *E_button;
};


/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

int srvr_entry;
long int ps_thrd_id;
char *markup;

/* iPDC Setup File path globaly */
char pmuFolderPath[200];
char pmuFilePath[200];
char fptr[200];

pid_t  pidLocal;
pmuStruct *pmu_data;
GtkWidget *pmu_server_window;
GtkWidget *p_id, *p_udp, *p_tcp, *p_ip;
GtkWidget *p_port, *p_protocol;
GtkWidget *CCWin, *setup_display_window;	
GtkWidget *data_managing_window;
GtkWidget *new_window;
GtkWidget *about_dialog;
GtkWidget *help_dialog;
GtkWidget *pmu_setup_window;


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

int  checkip(char ip[]);

int  isNumber(char *s);

void destroy (GtkWidget *widget, gpointer udata);

void about_pmu (GtkButton *widget, gpointer udata);

void Pmu_Help (GtkButton *but, gpointer udata);

void validation_result (char *msg);

void pmu_server ();

int  validation_pmu_server (GtkButton *button, gpointer udata);

void manage_data_sources();

void Set_data_source(GtkRadioButton *radiobutton, gpointer udata);

void file_ok_sel(GtkWidget *w, GtkFileChooser *fs);

void pmu_setup_file_selection(GtkWidget *widget, gpointer udata);

void view_setup_file (GtkWidget *w, GtkFileChooser *fs);

void apply_pmu_setup (GtkWidget *w, gpointer udata);

#endif 

/**************************************** End of File *******************************************************/
