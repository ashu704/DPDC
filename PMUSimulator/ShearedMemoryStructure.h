/* ----------------------------------------------------------------------------- 
 * function.c
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                  global Data Structure for Shared Memory           */
/* ------------------------------------------------------------------ */

struct P_id {

	pid_t pid;               /* Single pid */

	int dataFileVar;		/* For data source options : 0-Auto & 1-File Measurements */

	char filePath[400];		/* File path of user selected measurement file */

	int serverPort;		/* User given port number for server */

	int serverProtocol;		/* User given protocol preference UDP/TCP */

	int cfg_bit_change_info;	/* STAT Word change bit status if any otherwise default set to 0 */

	char cfgPath[200];       /* Its a PMU Setup File path given by use */

	int phasor_type[50];	/* save the phasor type (currently not useful) */

}p1, *ShmPTR;
