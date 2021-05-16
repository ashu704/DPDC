/* ----------------------------------------------------------------------------- 
 * PmuGui.c
 * 
 * PMU Simulator - Phasor Measurement Unit Simulator
 *
 * Copyright (C) 2011-2012 Nitesh Pandit
 * Copyright (C) 2011-2012 Kedar V. Khandeparkar
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Authors: 
 *		Nitesh Pandit <panditnitesh@gmail.com>
 *		Kedar V. Khandeparkar <kedar.khandeparkar@gmail.com>			
 *
 * ----------------------------------------------------------------------------- */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "PmuGui.h"
#include "ServerFunction.h"
#include "CfgFunction.h"
#include "CfgGuiFunctions.h"
#include "ShearedMemoryStructure.h"
#include "function.h"

#define MAX_STRING_SIZE 5000

/* -------------------------------------------------------------------------------------- */
/*                                 Functions in PmuGui.c                                  */
/* -------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*	1. int  isNumber(char *s);              						     */
/*	2. int  checkip(char ip[]);				               		     */
/*	3. void destroy (GtkWidget *widget, gpointer udata);			          */
/*	4. void about_pmu (GtkButton *widget, gpointer udata);			          */
/*	5. void Pmu_Help (GtkButton *but, gpointer udata);			          */
/*	6. void validation_result (char *msg);					               */
/*	7. void pmu_colors();							                    */
/*	8. int  validation_pmu_server (GtkButton *button, gpointer udata);	     */
/*	9. void pmu_server ();							                    */
/*	10. void manage_data_sources();						               */
/*	11. void Set_data_source(GtkWidget *widget, gpointer udata);		     */
/*	12. void file_ok_sel(GtkWidget *w, GtkFileSelection *fs);		          */
/*   13. void pmu_setup_file_selection(GtkWidget *widget, gpointer udata);      */
/*   14. void view_setup_file (GtkWidget *w, GtkFileSelection *fs);             */
/*   15. void apply_pmu_setup (GtkWidget *w, gpointer udata);                   */
/*                                                                              */
/* ---------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  isNumber(char *s):                                                 */
/* It checks the passing character is a number or not, return 0 if its a 	     */
/* otherwise returns 1.								                    */
/* ----------------------------------------------------------------------------	*/

int isNumber(char *s)
{
	int dot=0, num=0, end=0;

	while(isspace(*s))s++;
	if(! *s)return 0;
	while(*s)
		if(isspace(*s))
			end=1, s++;
		else if(end)
			return 0;
		else if(isdigit(*s))
			num=1, s++;
		else if(*s=='.')
			if(dot)
				return 0;  
			else
				dot=1,s++;        
		else
			return 0;
	return num;
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  checkip(char ip[])		               					*/			
/* It checks the IP address of passing argument with IP standards.    		*/
/* Returns 0 in case of correct otherwise 1.					               */
/* ----------------------------------------------------------------------------	*/

int checkip(char ip[])
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ip, &(sa.sin_addr));

	return result;
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  destroy (GtkWidget *widget, gpointer udata)         			*/
/* It Destroy(& close) the PMU Simulator & his both processes.             	*/	
/* ----------------------------------------------------------------------------	*/

void destroy (GtkWidget *widget, gpointer udata)
{
     kill(pidLocal, SIGKILL);

	gtk_main_quit();

	g_print ("\nPMU Simulator Closing Now.\n");
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  about_pmu (GtkButton *widget, gpointer udata)       			*/
/* It display the information about PMU Simulator Software like authors,   	*/
/* licensing, features, usage, etc.                                             */
/* ----------------------------------------------------------------------------	*/

void about_pmu (GtkButton *widget, gpointer udata)
{
     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/usr/local/share/PMU/images/PmuLogo1.png",0);

	const gchar *auth[] = {"\nDr. A.M. Kulkarni <http://www.ee.iitb.ac.in/wiki/faculty/anil>", "\nNitesh Pandit <panditnitesh@gmail.com>", "\nKedar V. Khandeparkar <kedar.khandeparkar@gmail.com>", NULL};

	/* Create a about dialog for displaying about the iPDC. */
	about_dialog = gtk_about_dialog_new ();

	/* Define the diffrent properties of about_dialog. */
	gtk_about_dialog_set_program_name ((GtkAboutDialog *)about_dialog, " ");
	gtk_about_dialog_set_comments ((GtkAboutDialog *)about_dialog, "Authors :\nNitesh Pandit\nKedar Khandeparkar\n\nWide Aera Measurement System\nDepartment of Electrical Engineering\nIndian Institute of Technology, Bombay\nMumbai, INDIA");

	gtk_about_dialog_set_license ((GtkAboutDialog *)about_dialog, "iPDC - A Phasor Data Concentrator & PMU Simulator\n\nGeneral Public License (GPL)\n\nAuthors:\n\t\tNitesh Pandit <panditnitesh@gmail.com>\n\t\tKedar V. Khandeparkar <kedar.khandeparkar@gmail.com>\n\n\nThis program is free software; you can redistribute it and/or\nmodify it under the terms of the GNU General Public License\nas published by the Free Software Foundation; either version 2\nof the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n");

     gtk_about_dialog_set_logo ((GtkAboutDialog *)about_dialog, pixbuf);
	gtk_about_dialog_set_website ((GtkAboutDialog *)about_dialog, "http://ipdc.codeplex.com/");
	gtk_about_dialog_set_copyright ((GtkAboutDialog *)about_dialog, "Copyright (C) 2011-2012 GPL3");
	gtk_about_dialog_set_authors ((GtkAboutDialog *)about_dialog, auth);

	/* Ensure that the dialog box is destroyed when the user responds */
	g_signal_connect_swapped (about_dialog, "response", G_CALLBACK (gtk_widget_destroy), about_dialog);

	/* Finally show the about_dialog. */
	gtk_widget_show_all (about_dialog);
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  Pmu_Help (GtkButton *but, gpointer udata)				          */
/* It shows the general guidelines to user to fill the entries for connecting,	*/
/* or disconnecting devices in PMU Simulator Software.				          */
/* ----------------------------------------------------------------------------	*/

void Pmu_Help (GtkButton *but, gpointer udata)
{

	printf("\n\t\t|-------------------------------------------------------|\n");      
	printf("\t\t|\t\tPMU HELP\t\t|\n");      
	printf("\t\t|-------------------------------------------------------|\n");      

	/* Create the widgets */
	help_dialog = gtk_about_dialog_new ();

	//GtkWidget *image1 = gtk_image_new_from_file("pdcIMG.jpg");
	gtk_about_dialog_set_program_name ((GtkAboutDialog *)help_dialog, "PMU HELP\n\n");
	gtk_about_dialog_set_comments ((GtkAboutDialog *)help_dialog, "1) ID Code should be integer i.e. 50, 60, etc.\t\t\t\n\n2) Port should be greater than 1500 and less than 65535.\t\n\n3) Channel Name could be upto 16 characters.\t\t\t\n\n4) Enter the integer value for phasors, analogs and digitals.\n\n5) Fixed/Floating point and Rectangular/Polar are the\t\trepresentation of measurements.");

	/* Ensure that the dialog box is destroyed when the user responds */
	g_signal_connect_swapped (help_dialog, "response", G_CALLBACK (gtk_widget_destroy), help_dialog);
	gtk_widget_show_all (help_dialog);
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  validation_result (char *msg)					               */
/* It displays the status message of performed operation.			          */
/* ----------------------------------------------------------------------------	*/

void validation_result (char *msg)
{
	GtkWidget *val_dialog;

	/* Create the widgets a new dialog box */
	val_dialog = gtk_about_dialog_new ();

	/* Define the diffrent properties of val_dialog. */
	gtk_about_dialog_set_program_name ((GtkAboutDialog *)val_dialog, " ");
	gtk_about_dialog_set_comments ((GtkAboutDialog *)val_dialog, msg);

	/* Ensure that the dialog box is destroyed when the user responds */
	g_signal_connect_swapped (val_dialog, "response", G_CALLBACK (gtk_widget_destroy), val_dialog);

	/* Finally show the val_dialog. */
	gtk_widget_show_all (val_dialog);
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  pmu_colors()							                    */
/* It fills the colors in text on lables and set the images on given buttons. 	*/
/* It called in main only one time to initialized all the parameters.		     */
/* ----------------------------------------------------------------------------	*/

void pmu_colors()
{
	/* Set the font and color for the heading on 'welcome_pmu' lable */
//	markup = g_markup_printf_escaped ("<span foreground=\"#006666\" font='16'><b>Phasor Measurement Unit\n\t  PMU Simulator</b></span>");
//	gtk_label_set_markup (GTK_LABEL (pmu_data->welcome_pmu), markup);
//	g_free (markup);

	// markup = g_markup_printf_escaped ("<span foreground=\"#330000\" font='10'><b>\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------</b></span>");

	// gtk_label_set_markup (GTK_LABEL (pmu_data->footer_label), markup);
	// g_free (markup);

	// markup = g_markup_printf_escaped ("<span foreground=\"#333333\" font='10'><b>Admin Operations</b></span>");
	// gtk_label_set_markup (GTK_LABEL (pmu_data->admin_label), markup);
	// g_free (markup);

	// markup = g_markup_printf_escaped ("<span foreground=\"#330000\" font='8'>All Rights Reserved</span>");
	// gtk_label_set_markup (GTK_LABEL (pmu_data->rights_lable), markup);
	// g_free (markup);

	/* Add a "close" button to the bottom of the dialog */ 
	// GdkColor color;
	//GtkWidget *image, *image1;
	// gdk_color_parse( "#999999", &color ); 
	// gtk_widget_modify_bg( GTK_WIDGET(pmu_data->img_label), GTK_STATE_NORMAL, &color );

	/* Set the general image on a button */
	// image = gtk_image_new_from_file("/usr/local/share/PMU/images/PSLab.PNG");
	// gtk_button_set_image ((GtkButton *)pmu_data->img_label, image);
	// gtk_widget_show (pmu_data->img_label);

	/* Set the image (PMU-logo) on a specific button */
	// image1 = gtk_image_new_from_file("/usr/local/share/PMU/images/PmuLogo.png");
	// gtk_button_set_image ((GtkButton *)pmu_data->logo_butun, image1);
	// gtk_widget_show (pmu_data->logo_butun);
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  validation_pmu_server (GtkButton *button, gpointer udata)		*/
/* It Validates the user enterd details for PMU Simulator. It gives the		*/
/* successful message and go ahead with entered data if all entries		     */
/* were right, else it shows the validation error message to user.		     */
/* ----------------------------------------------------------------------------	*/

int validation_pmu_server (GtkButton *button, gpointer udata)
{

	printf("\n\t\t|-------------------------------------------------------|\n");      
	printf("\t\t|\t\tWelcome to PMU SERVER VALIDATION\t\t|\n");      
	printf("\t\t|-------------------------------------------------------|\n");      
	/* local variables */
	int tmp_p;
	char *msg;
	const char *text1, *text2;

	/* Get the text entries filled by user */
	text1 = gtk_entry_get_text(GTK_ENTRY(p_port));
	text2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(p_protocol));

	if(!isNumber((char *)text1) && (atoi(text1) < 1500 || (atoi(text1) > 65535))) 	/* Check for valid port number */		
	{ 
		msg = "\tPlease enter valid value for PMU Server Port\t\n";
		validation_result (msg);		/* Show the unsuccess message to user */
		return 0;
	}
	else
	{
		strncpy(PMUprotocol, text2, 3);
		PMUport = atoi(text1);

		if(!strcmp(text2, "UDP"))
			tmp_p = 1;			
		else
			tmp_p = 2;			

		p1 = *ShmPTR;
		p1.cfg_bit_change_info = 0;
		ShmPTR->cfg_bit_change_info = 0;
		ShmPTR->serverProtocol = tmp_p;
		ShmPTR->serverPort = atoi(text1);

		p1.pid = ShmPTR->pid;

		kill(p1.pid, SIGUSR2);
		printf("PORT & Protocol information has sent to PMU Server through signal.\n");

		memset(pmuFilePath, '\0', 200);

		gtk_widget_set_sensitive(GTK_WIDGET(pmu_data->start_server), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(pmu_data->open_cfg), FALSE);

		gtk_widget_set_sensitive(GTK_WIDGET(pmu_data->create_cfg_button), TRUE);					
		gtk_widget_set_sensitive(GTK_WIDGET(pmu_data->menu_setup_cfg), TRUE);					

		/* Close/destroy the pmu_server_window */
		gtk_widget_destroy(pmu_server_window);

//		msg = "PMU Server run Successfully. Create the Configuration Frame.\n";
//		validation_result (msg);		/* Show the success message to user */
		return 1;
	}
};

/* ----------------------------------------------------------------------------	*/
/* FUNCTION  pmu_server ()							                    */
/* It displays the windows for entring PMU Server details to user.		     */
/* ----------------------------------------------------------------------------	*/

void pmu_server ()
{
	/* local variables */
	GtkWidget *table, *label, *cancel_button;
	GtkWidget *valdbutton, *help_button;

	/* Create a new dialog window for PMU Server Setup */
	pmu_server_window = gtk_dialog_new ();
	g_signal_connect (pmu_server_window, "destroy", G_CALLBACK (gtk_widget_destroy), pmu_server_window);
	gtk_window_set_title (GTK_WINDOW (pmu_server_window), "PMU Server Setup");
	gtk_container_set_border_width (GTK_CONTAINER (pmu_server_window), 10);

	/* Create a table of 4 by 2 squares. */
	table = gtk_grid_new (); // table = gtk_table_new (6, 2, FALSE);

	/* Set the spacing to 50 on x and 5 on y */
	gtk_grid_set_row_spacing (GTK_GRID(table), 16); // gtk_table_set_row_spacings (GTK_TABLE  (table), 8);
	gtk_grid_set_column_spacing (GTK_GRID(table), 16); // gtk_table_set_col_spacings (GTK_TABLE  (table), 2);

	/* Pack the table into the window */
	//gtk_box_pack_start (GTK_BOX (GTK_DIALOG(pmu_server_window)->vbox), table, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG(pmu_server_window))), table, TRUE, TRUE, 0);
	gtk_widget_show (table);

	/* Add few buttons to the bottom of the dialog */
	valdbutton = gtk_dialog_add_button((GtkDialog *)pmu_server_window, "Run", GTK_RESPONSE_OK); // gtk_button_new_with_label ("Run");
	help_button = gtk_dialog_add_button((GtkDialog *)pmu_server_window, "Help", GTK_RESPONSE_APPLY); // gtk_button_new_with_label ("Help");
	cancel_button = gtk_dialog_add_button((GtkDialog *)pmu_server_window, "Cancel", GTK_RESPONSE_CLOSE); //gtk_button_new_with_label ("Cancel");

	/* This simply creates a grid of toggle buttons on the table */
	label = gtk_label_new (" ");
	markup = g_markup_printf_escaped ("<span foreground=\"#990033\" font='12'><b>Enter PMU Server Details</b></span>");
	gtk_label_set_markup (GTK_LABEL (label), markup);
	gtk_grid_attach (GTK_GRID (table), label, 0,0,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 0, 1);
	gtk_widget_show (label);
	g_free (markup);

	label = gtk_label_new ("Server Port");
     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
     	gtk_label_set_xalign (GTK_LABEL (label),0);
    	gtk_label_set_yalign (GTK_LABEL (label),0);
	gtk_grid_attach (GTK_GRID (table), label, 0, 2, 1, 1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
	gtk_widget_show (label);

	label = gtk_label_new ("Protocol");
     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
     	gtk_label_set_xalign (GTK_LABEL (label),0);
    	gtk_label_set_yalign (GTK_LABEL (label),0);
	gtk_grid_attach (GTK_GRID (table), label, 0, 3, 1, 1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);
	gtk_widget_show (label);

	/* Create text boxes for user to enter appropriate values */
	p_port = gtk_entry_new();
	gtk_entry_set_max_length ((GtkEntry *)p_port, 5);
	gtk_grid_attach (GTK_GRID (table), p_port, 1, 2, 1, 1); // gtk_table_attach_defaults (GTK_TABLE (table), p_port, 1, 2, 2, 3);
	gtk_widget_show (p_port);

	label = gtk_label_new (" ");
	markup = g_markup_printf_escaped ("<span foreground=\"#333333\" font='8'><b>Note : Use the unreserved ports for PMU Server.</b></span>");
	gtk_label_set_markup (GTK_LABEL (label), markup);
     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
     	gtk_label_set_xalign (GTK_LABEL (label),0);
    	gtk_label_set_yalign (GTK_LABEL (label),0);
	gtk_grid_attach (GTK_GRID (table), label, 0, 5, 2, 1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 5, 6);
	gtk_widget_show (label);
	g_free (markup);

	/* Create combo boxe for user with some fixed values */
	p_protocol = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(p_protocol), "UDP");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(p_protocol), "TCP");
	gtk_combo_box_set_active(GTK_COMBO_BOX(p_protocol), 0);
	gtk_grid_attach (GTK_GRID (table), p_protocol, 1, 3, 1, 1); // gtk_table_attach_defaults (GTK_TABLE (table), p_protocol, 1, 2, 3, 4);
	gtk_widget_show (p_protocol);

	/* Signal handling for buttons on PMU Server Setup Window */
	g_signal_connect (valdbutton, "clicked", G_CALLBACK (validation_pmu_server), valdbutton);
	g_signal_connect (help_button, "clicked", G_CALLBACK (Pmu_Help), NULL);
	g_signal_connect_swapped (cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), pmu_server_window);
	g_signal_connect (pmu_server_window, "destroy", G_CALLBACK (gtk_widget_destroy), pmu_server_window);

	/* This makes it so the button is the default. */
	gtk_widget_set_can_default (valdbutton, TRUE);
	gtk_widget_set_can_default (help_button, TRUE);
	gtk_widget_set_can_default (cancel_button, TRUE);
	// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (pmu_server_window)->action_area), valdbutton, TRUE, TRUE, 0);
	// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (pmu_server_window)->action_area), help_button, TRUE, TRUE, 0);
	// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (pmu_server_window)->action_area), cancel_button, TRUE, TRUE, 0);	    

	/* This grabs this button to be the default button. Simply hitting the "Enter" key will cause this button to activate. */
	gtk_widget_grab_default (valdbutton);
	gtk_widget_show (valdbutton);
	gtk_widget_grab_default (help_button);
	gtk_widget_show (help_button);
	gtk_widget_grab_default (cancel_button);
	gtk_widget_show (cancel_button);

	/* Finally show the pmu_server_window */
	gtk_widget_show (pmu_server_window);
};



/* ----------------------------------------------------------------------------	*/
/* FUNCTION  manage_data_sources()						                    */
/* It displays the option window to user for selection of data/measurement      */
/* source. Either auto generated or from measurement file.                      */
/* ----------------------------------------------------------------------------	*/

void manage_data_sources()
{

	/* local variables */
	GtkWidget *table;
	GtkWidget *close_butn, *label;
	GtkWidget *Addbutton, *Removebutton;

	/* Create a new dialog window for the Option Window */ 
	data_managing_window = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (data_managing_window), "Manage Data Source");
	gtk_container_set_border_width (GTK_CONTAINER (data_managing_window), 10);
	//gtk_widget_set_size_request (data_managing_window, 350, 350);

	/* Create a table of 4 by 2 squares */
	table = gtk_grid_new (); // table = gtk_table_new (6, 2, FALSE);
	
	/* Deactivate manage data source */
	gtk_window_set_modal (GTK_WINDOW (data_managing_window), TRUE);
        gtk_window_set_keep_above(GTK_WINDOW (data_managing_window), TRUE);
	/* Set the spacing to 15 on x and 25 on y */
	gtk_grid_set_row_spacing (GTK_GRID(table), 8); // gtk_table_set_row_spacings (GTK_TABLE  (table), 8);
	gtk_grid_set_column_spacing (GTK_GRID(table), 2); // gtk_table_set_col_spacings (GTK_TABLE  (table), 2);

	/* Pack the table into the window */
	// gtk_box_pack_start (GTK_BOX (GTK_DIALOG(data_managing_window)->vbox), table, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG(data_managing_window))), table, TRUE, TRUE, 0);
	gtk_widget_show (table);

	label = gtk_label_new (" ");
	markup = g_markup_printf_escaped ("<span foreground=\"#990033\" font='12'><b>Select Measurement Source</b></span>");
	gtk_label_set_markup (GTK_LABEL (label), markup);
	
	gtk_grid_attach (GTK_GRID (table), label, 0,0,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 0, 1);
	gtk_widget_show (label);
	g_free (markup);

	/* Add few radio buttons on the dialog window */
	Addbutton = gtk_radio_button_new_with_label (NULL, "Auto Genetared");
	gtk_grid_attach (GTK_GRID (table), Addbutton, 0,2,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), Addbutton, 0, 1, 2, 3);
     if (ShmPTR->dataFileVar == 0 )
     {
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Addbutton), TRUE);
     }
     gtk_widget_show (Addbutton);

	Removebutton = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (Addbutton), "Measurement File");
	gtk_grid_attach (GTK_GRID (table), Removebutton, 0,3,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), Removebutton, 0, 1, 3, 4);
     if (ShmPTR->dataFileVar == 1 )
     {
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Removebutton), TRUE);
     }
	gtk_widget_show (Removebutton);

	label = gtk_label_new (" ");
	markup = g_markup_printf_escaped ("<span foreground=\"#333333\" font='8'><b>Note : Auto Genetared Data may be raw measurements.\nMay use CSV file of Phasor measurements.\nFor CSV file format see our Sample measurements files.</b></span>");
	gtk_label_set_markup (GTK_LABEL (label), markup);
	gtk_grid_attach (GTK_GRID (table), label, 0,5,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 5, 6);
	gtk_widget_show (label);
	g_free (markup);

	/* Add 'Close' buttons on the dialog window */
	close_butn = gtk_dialog_add_button((GtkDialog *)data_managing_window, "Close", GTK_RESPONSE_CLOSE); // gtk_button_new_with_label ("Close");

	/* Signal handling for buttons on Option Window */
	g_signal_connect (Addbutton, "toggled", G_CALLBACK (Set_data_source), (gpointer) "1");
	g_signal_connect (Removebutton, "toggled", G_CALLBACK (Set_data_source), (gpointer) "2");
	g_signal_connect_swapped (close_butn, "clicked", G_CALLBACK (gtk_widget_destroy), data_managing_window);
	g_signal_connect_swapped (data_managing_window, "response", G_CALLBACK (gtk_widget_destroy), data_managing_window);

	/* This makes it so the button is the default. */
	// gtk_widget_set_can_default (close_butn, TRUE);
	// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (data_managing_window)->action_area), close_butn, TRUE, TRUE, 0);

	/* This grabs this button to be the default button. Simply hitting the "Enter" key will cause this button to activate. */
	gtk_widget_grab_default (close_butn);
	gtk_widget_show (close_butn);

	/* Finally show the data_managing_window */
	gtk_widget_show (data_managing_window);
};


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  Set_data_source(GtkWidget *widget, gpointer udata)			     */
/* It will show user the dialog box that have browser of his system to		*/
/* choose the file for measurements reading, otherwise just raw data.		     */
/* ----------------------------------------------------------------------------	*/

void Set_data_source(GtkRadioButton *radiobutton, gpointer udata)
{
    char *file_path;
    file_path = malloc(200*sizeof(char));
    strcpy(file_path, getenv("HOME"));
    strcat(file_path,"/iPDC/DataDir/");
    gint event;
   
    if(strcmp("Auto Genetared", gtk_button_get_label(GTK_BUTTON(radiobutton))))
    {
        strcpy(ShmPTR->filePath, "nil");
        ShmPTR->dataFileVar = 0;
        p1.pid = ShmPTR->pid;

        kill(p1.pid, SIGUSR1);
        printf("Raw Data selection is new choice of user, sent to the PMU Server by signal.\n");

        gtk_widget_destroy(data_managing_window);
    }   
    else
    {
        gtk_widget_destroy(data_managing_window);

        /* Create a new file selection widget */
        //data_managing_window = gtk_file_selection_new ("PMU simulator measurement file selection");

         data_managing_window = gtk_file_chooser_dialog_new ("PMU Simulator measurement file selection",
                NULL,
                GTK_FILE_CHOOSER_ACTION_OPEN,
                "_Open", GTK_RESPONSE_ACCEPT,
                "_Cancel", GTK_RESPONSE_CANCEL, NULL);

        /* Connect the ok_button to file_ok_sel function */
        //gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (data_managing_window)->ok_button), "clicked", (GtkSignalFunc) file_ok_sel, data_managing_window );

        /* Connect the cancel_button to destroy the widget */
        //gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (data_managing_window)->cancel_button), "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (data_managing_window));

        /* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
        //gtk_file_selection_set_filename (GTK_FILE_SELECTION(data_managing_window), ptr1);
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(data_managing_window), file_path);

        event = gtk_dialog_run( GTK_DIALOG(data_managing_window ) );
        gtk_widget_show(data_managing_window);

        gtk_widget_show(data_managing_window);

        if (event == GTK_RESPONSE_ACCEPT)
        {
            file_ok_sel(NULL, GTK_FILE_CHOOSER (data_managing_window));
        }
        else if (event == GTK_RESPONSE_CANCEL)
        {
            gtk_widget_destroy(GTK_WIDGET (data_managing_window));
        }
    }
}

/* ----------------------------------------------------------------------------	*/
/* FUNCTION  file_ok_sel(GtkWidget *w, GtkFileChooser *fs)			     */
/* This function gets the user file and check whether it has proper number      */
/* of columns or not, if yes then sent signal to server.	                    */
/* ----------------------------------------------------------------------------	*/

void file_ok_sel(GtkWidget *w, GtkFileChooser *fs)
{
    /* Local Variables */
	int total_column, count;
	char *l1, *d3, *d4;

	size_t l2 = 0;
	FILE *temp_fp;
	ssize_t result;

	char *s = (char *)gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fs));

     /* 3 for UNIX time stamp, FREQ & DFREQ */
	total_column = cfg_info->cfg_phnmr_val*2 + cfg_info->cfg_annmr_val + 3; 

     /* Open the user given measurements in read mode */
	temp_fp = fopen(s, "r");

	if (temp_fp == NULL)
	{
		validation_result("\tMeasurement File doesn't exist.\t\t");
	}
	else 
	{
		if ((result = getdelim(&l1, &l2, ('\n'), temp_fp)) >0)
		{
			d3 = strtok (l1, ","); 

			for(count=1; ; count++) //count<=total_column
			{
				if((d4 = strtok (NULL,",")) == NULL)
					break;
			}

			if(count == total_column)
			{
				strcpy(ShmPTR->filePath, s);
				ShmPTR->dataFileVar = 1;
				p1.pid = ShmPTR->pid;

				kill(p1.pid, SIGUSR1);
				printf("\nInform PMU Server that Data Source has changed to measurement file by signal.\n");

				fclose(temp_fp);
				validation_result("Now measurements are taken from measurement file.\t\n");
			}
			else
			{
				fclose(temp_fp);
				validation_result("File data does not match with configuration.\nPlease Select a proper file as per your configuration.");
			}
		}
	}

	gtk_widget_destroy(data_managing_window);
}


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  pmu_setup_file_selection(GtkWidget *widget, gpointer udata)	*/
/* It will display a file selector window to the user.	User have the          	*/
/* responsibility to choose the correct previously saved file for PMU Setup.	*/
/* ----------------------------------------------------------------------------	*/

void pmu_setup_file_selection(GtkWidget *widget, gpointer udata)
{
    char  *file_path;
    file_path = malloc( 200*sizeof(char));
    memset(file_path,'\0',200);
    strcpy(file_path,getenv("HOME"));
    strcat(file_path,"/iPDC/PMU/");

	/* Create a new file selection widget */
	// pmu_setup_window = gtk_file_selection_new ("PMU Setup File Selector");

	// /* Connect the ok_button to view_setup_file function */
	// gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (pmu_setup_window)->ok_button), "clicked", (GtkSignalFunc) view_setup_file, pmu_setup_window);

	// /* Connect the cancel_button to destroy the widget */
	// gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (pmu_setup_window)->cancel_button), "clicked", (GtkSignalFunc) gtk_widget_destroy,
	// 		GTK_OBJECT (pmu_setup_window));

	// /* Lets set the filename, as if this were a save dialog, and we are giving a default filename */
	// gtk_file_selection_set_filename (GTK_FILE_SELECTION(pmu_setup_window), ptr1);

	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

	gint event;
	pmu_setup_window = gtk_file_chooser_dialog_new ("PMU Setup File Selector", GTK_WINDOW(udata), action, "_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(pmu_setup_window), file_path);

	event = gtk_dialog_run( GTK_DIALOG(pmu_setup_window ) );
	gtk_widget_show(pmu_setup_window);

    if (event == GTK_RESPONSE_ACCEPT)
    {
            view_setup_file(GTK_WIDGET(udata), GTK_FILE_CHOOSER (pmu_setup_window));
    }
    else if (event == GTK_RESPONSE_CANCEL)
    {
            gtk_widget_destroy(GTK_WIDGET (pmu_setup_window));
    }

	
}


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  view_setup_file(GtkWidget *w, GtkFileChooser *fs)                */
/* This function displays the different attributes of selected PMU Setup File.  */
/* It will show the PMU Server & CFG details before apply.                      */
/* ----------------------------------------------------------------------------	*/

void view_setup_file (GtkWidget *w, GtkFileChooser *fs)
{
	/* local variables */
	GtkWidget *table, *label;
	GtkWidget *apply_button, *cancel_button;

	int tempi, i;
	char stn[17], *s,*rline = NULL;
	char *d1, buff[15], *markup;
	unsigned int framesize;
	unsigned char *line, tempC[2];
	size_t len = 0;
	ssize_t read;
	FILE *fp1;

	s = (char *)gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fs));
	strcpy(fptr, s);

	gtk_widget_destroy(pmu_setup_window);

	fp1 = fopen(s, "rb");

	if (fp1 == NULL)
	{
		validation_result("\tPMU Setup file doesn't exist. Try again\t\t");
	}
	else 
	{
		/* If its available, then create a new dialog window for displaying the iPDC details. */
		setup_display_window = gtk_dialog_new ();
		gtk_window_set_title (GTK_WINDOW (setup_display_window), "PMU Simulator Setup");
		gtk_container_set_border_width (GTK_CONTAINER (setup_display_window), 10);
		//gtk_widget_set_size_request (setup_display_window, 350, 400);

		/* Create a table of ? by 2 squares */
		table = gtk_grid_new (); // gtk_table_new (17, 2, FALSE);

		/* set the spacing to 10 on x and 10 on y */
		gtk_grid_set_row_spacing (GTK_GRID(table), 8); // gtk_table_set_row_spacings (GTK_TABLE  (table), 8);
		gtk_grid_set_column_spacing (GTK_GRID(table), 2); // gtk_table_set_col_spacings (GTK_TABLE  (table), 2);

		// gtk_box_pack_start (GTK_BOX (GTK_DIALOG(setup_display_window)->vbox), table, TRUE, TRUE, 0);
		gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG(setup_display_window))), table, TRUE, TRUE, 0);
		gtk_widget_show (table);

		/* Add a "close" button to the bottom of the dialog */
		apply_button = gtk_dialog_add_button((GtkDialog *)setup_display_window, "Apply", GTK_RESPONSE_APPLY); // gtk_button_new_with_label ("Apply");
		cancel_button = gtk_dialog_add_button((GtkDialog *)setup_display_window, "Cancel", GTK_RESPONSE_CLOSE); // gtk_button_new_with_label ("cancel");

		label = gtk_label_new (" ");
		markup = g_markup_printf_escaped ("<span foreground=\"#7D2252\" font='12'><b>PMU Setup File Details</b></span>");
		gtk_label_set_markup (GTK_LABEL (label), markup);
		gtk_grid_attach (GTK_GRID (table), label, 0,0,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 0, 1);
		gtk_widget_show (label);
		g_free (markup);

		label = gtk_label_new (" ");
		markup = g_markup_printf_escaped ("<span foreground=\"#0000FF\" font='12'>Server Info</span>");
		gtk_label_set_markup (GTK_LABEL (label), markup);
		gtk_grid_attach (GTK_GRID (table), label, 0,2,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 2, 3);
		gtk_widget_show (label);
		g_free (markup);

		label = gtk_label_new ("PMU Port");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,3,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);
		gtk_widget_show (label);

		label = gtk_label_new ("Protocol");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,4,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 4, 5);
		gtk_widget_show (label);

		label = gtk_label_new (" ");
		markup = g_markup_printf_escaped ("<span foreground=\"#0000FF\" font='12'>Configuration Info</span>");
		gtk_label_set_markup (GTK_LABEL (label), markup);
		gtk_grid_attach (GTK_GRID (table), label, 0,6,2,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 6, 7);
		gtk_widget_show (label);
		g_free (markup);

		label = gtk_label_new ("PMU ID");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,7,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 7, 8);
		gtk_widget_show (label);

		label = gtk_label_new ("Station Name");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,8,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 8, 9);
		gtk_widget_show (label);

		label = gtk_label_new ("Pasor Number");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,9,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 9, 10);
		gtk_widget_show (label);

		label = gtk_label_new ("Analog Number");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,10,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 10, 11);
		gtk_widget_show (label);

		label = gtk_label_new ("Digital Number");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,11,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 11, 12);
		gtk_widget_show (label);

		label = gtk_label_new ("Format Word");
	     // gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,12,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 12, 13);
		gtk_widget_show (label);

		label = gtk_label_new ("CFG Count");
	     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     	gtk_label_set_xalign (GTK_LABEL (label),0);
    	 	gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,13,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 13, 14);
		gtk_widget_show (label);

		label = gtk_label_new ("Data Rate ");
	     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     	gtk_label_set_xalign (GTK_LABEL (label),0);
    	 	gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,14,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 14, 15);
		gtk_widget_show (label);

		label = gtk_label_new (" ");
		markup = g_markup_printf_escaped ("<span foreground=\"#333333\" font='8'><b>Note : Configuration information of selected PMU Setup file.</b></span>");
		gtk_label_set_markup (GTK_LABEL (label), markup);
	     	// gtk_misc_set_alignment (GTK_MISC(label),0,0);
	     gtk_label_set_xalign (GTK_LABEL (label),0);
    	 gtk_label_set_yalign (GTK_LABEL (label),0);
		gtk_grid_attach (GTK_GRID (table), label, 0,16,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 2, 16, 17);
		gtk_widget_show (label);
		g_free (markup);

		tempi = 1;
		while(tempi < 4)
		{
			read = getline(&rline, &len, fp1);

			if(read == 0)
				break;

			if(tempi == 1)
			{
				label = gtk_label_new (rline);
				gtk_grid_attach (GTK_GRID (table), label, 1,3,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 3, 4);
				gtk_widget_show (label);
			}
			else if(tempi == 2)
			{
				label = gtk_label_new (rline);
				gtk_grid_attach (GTK_GRID (table), label, 1,4,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 4, 5);
				gtk_widget_show (label);
			}

			tempi++;
		}

		if(read > 0)
		{
			d1 = strtok (rline," ");
			d1 = strtok (NULL," ");
			tempi = atoi(d1);

			if (tempi == 1)
			{
				d1 = strtok (NULL,"\n");
				framesize = atoi(d1);

				line = malloc(framesize*sizeof(unsigned char));
				fread(line, sizeof(unsigned char), framesize, fp1);

				tempC[0] = line[4];
				tempC[1] = line[5];

	               tempi = tempC[0];
	              	tempi<<=8;
	              	tempi |=tempC[1];

				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 1,7,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 7, 8);
				gtk_widget_show (label);

				for(i=0; i<16; i++)
				{
					stn[i] = line[20+i];
				}
				stn[16] = '\0';

				label = gtk_label_new (stn);
				gtk_grid_attach (GTK_GRID (table), label, 1,8,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 8, 9);
				gtk_widget_show (label);

				tempC[0] = line[38];
				tempC[1] = line[39];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 1,12,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 12, 13);
				gtk_widget_show (label);

				tempC[0] = line[40];
				tempC[1] = line[41];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 1,9,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 9, 10);
				gtk_widget_show (label);

				tempC[0] = line[42];
				tempC[1] = line[43];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 1,10,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 10, 11);
				gtk_widget_show (label);

				tempC[0] = line[44];
				tempC[1] = line[45];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 0,11,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 11, 12);
				gtk_widget_show (label);

				tempC[0] = line[framesize-6];
				tempC[1] = line[framesize-5];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 0,13,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 13, 14);
				gtk_widget_show (label);

				tempC[0] = line[framesize-4];
				tempC[1] = line[framesize-3];
				tempi = c2i(tempC);

				memset(buff, '\0', 15);
				sprintf(buff, "%d", tempi);
				label = gtk_label_new (buff);
				gtk_grid_attach (GTK_GRID (table), label, 0,14,1,1); // gtk_table_attach_defaults (GTK_TABLE (table), label, 1, 2, 14, 15);
				gtk_widget_show (label);
			}
		}
		fclose(fp1);

		/* Define signal on ok_button when it "clicked". */
		g_signal_connect_swapped (cancel_button, "clicked", G_CALLBACK (gtk_widget_destroy), setup_display_window);
		g_signal_connect_swapped (apply_button, "clicked", G_CALLBACK (apply_pmu_setup), setup_display_window);
		g_signal_connect (setup_display_window, "destroy", G_CALLBACK (gtk_widget_destroy), setup_display_window);

		/* This makes it so the ok_button is the default. */
		// gtk_widget_set_can_default (cancel_button, TRUE);
		// gtk_widget_set_can_default (apply_button, TRUE);
		// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (setup_display_window)->action_area), apply_button, TRUE, TRUE, 0);
		// gtk_box_pack_start (GTK_BOX (GTK_DIALOG (setup_display_window)->action_area), cancel_button, TRUE, TRUE, 0);

		/* This grabs this button to be the default button. Simply hitting the "Enter" key will cause this button to activate. */
		gtk_widget_grab_default (cancel_button);
		gtk_widget_show (cancel_button);
		gtk_widget_grab_default (apply_button);
		gtk_widget_show (apply_button);

		/* Finally show the setup_display_window. */
		gtk_widget_show (setup_display_window);
	}
}


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  apply_pmu_setup (GtkWidget *w, gpointer udata)			*/
/* This function apply the user selected PMU Setup File and go ahead with the 	*/
/* Setup file's information.                                                    */
/* ----------------------------------------------------------------------------	*/

void apply_pmu_setup (GtkWidget *w, gpointer udata)
{
	/* local variables */
	int  tmp_p, tempi, tmp_port;
	char *d1, *rline=NULL;

	size_t len = 0;
	FILE *fp1;
	ssize_t read;

	gtk_widget_destroy(setup_display_window);

	/* Open the saved PMU Setup File and read the informations */
	fp1 = fopen (fptr,"rb");

	if (fp1 != NULL)			
	{
		tempi = 1;

          /* To get the PmuServer details from the setup file */
		while(tempi < 3)
		{
			read = getline(&rline, &len, fp1);

			if(read == 0)
				break;

			if(tempi == 1)
			{
				PMUport = atoi(rline);
				tmp_port = PMUport;
			}
			else
			{
				d1 = strtok (rline,"\n");
				strcat(PMUprotocol, d1);
				PMUprotocol[4] = '\0';
			}

			tempi++;
		}

          /* Make this file as a common file to operate on */
		memset(pmuFilePath,'\0',sizeof(pmuFilePath));
		strcpy(pmuFilePath, fptr);
		fclose(fp1);

		if(read > 0)
		{
			if(!strcmp(PMUprotocol, "UDP"))
				tmp_p = 1;
			else
				tmp_p = 2;

               /* Finally for starting the PMU Server */
			p1 = *ShmPTR;
			p1.cfg_bit_change_info = 0;
			ShmPTR->cfg_bit_change_info = 0;
			ShmPTR->serverProtocol = tmp_p;
			ShmPTR->serverPort = tmp_port;

			PMUport = tmp_port;

			p1.pid = ShmPTR->pid;

			kill(p1.pid, SIGUSR2);
			printf("PORT = %d & Protocol = %s, info sent to PMU Server by signal.\n", PMUport, PMUprotocol);

			gtk_widget_set_sensitive(GTK_WIDGET(pmu_data->start_server), FALSE);

               /* Finally creating the CFG objects */
			final_cfg_create ();
		}
		else
		{
			validation_result("\tSelected file is not a PMU Setup File.\t\n");
		}
	}
	else 
	{
		validation_result("\tPMU Setup File doesn't exist.\t\t");
	}
}

/**************************************** End of File *******************************************************/
