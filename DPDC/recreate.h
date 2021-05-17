/* ----------------------------------------------------------------------------- 
 * recreate.h
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

void recreate_cfg_objects();

void init_cfgparser(unsigned char []); 

void recreate_Connection_Table();

void recreate_udp_connections(struct Lower_Layer_Details *t11);

void recreate_tcp_connections(struct Lower_Layer_Details *t12);

/**************************************** End of File *******************************************************/
