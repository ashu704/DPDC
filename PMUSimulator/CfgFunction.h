/* ----------------------------------------------------------------------------- 
 * CfgFunction.h
 * ----------------------------------------------------------------------------- */


#include<gtk/gtk.h>

/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

int  PMUport;
char PMUprotocol[4];
char *hdr_frame, *markup1;
unsigned char phunit_final_val[500];
unsigned char anunit_final_val[500];


/* ---------------------------------------------------------------- */
/*                         global Data Structure                    */
/* ---------------------------------------------------------------- */

struct ConfigurationFrame 
{
	int  cfg_pmuID; 
	int  cfg_fdf; 
	int  cfg_af; 
	int  cfg_pf; 
	int  cfg_pn; 
	int  cfg_phnmr_val; 
	int  cfg_annmr_val; 
	int  cfg_dgnmr_val; 
	int  cfg_fnom; 
	int  cfg_dataRate; 
	char *cfg_STNname; 
	char *cfg_phasor_channels; 
	char *cfg_analog_channels; 
	char *cfg_digital_channels; 
};
struct ConfigurationFrame *cfg_info;

struct ConfigurationChange 
{
	int  add_remove_choice; 
	int  new_cfg_phnmr_val; 
	int  new_cfg_annmr_val; 
	int  data_rate_choice; 
	int  new_data_rate; 
	char *new_cfg_phasor_channels; 
	char *new_cfg_analog_channels; 
};
struct ConfigurationChange *new_cfg_info;


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

void header_frm_gen(int len);

void reconfig_cfg_CC();

void show_pmu_details (GtkWidget *widget, gpointer udata);

int  create_cfg();

/**************************************** End of File *******************************************************/
