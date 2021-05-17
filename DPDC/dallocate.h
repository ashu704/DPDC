/* ----------------------------------------------------------------------------- 
 * dallocate.h
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

/* Frees memory allocated to cfg objects */
void free_cfgframe_object(struct cfg_frame *cfg); 

/* Frees memory allocated to data objects */
void free_dataframe_object(struct data_frame *); 

/* frees memory allocated to 2D arrays */
void free_2darray(unsigned char** array, int x); 

/**************************************** End of File *******************************************************/
