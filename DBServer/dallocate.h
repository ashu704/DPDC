/* ----------------------------------------------------------------------------- 
 * dallocate.h
 * 
 * DPDC - Distribution Phasor Data Concentrator			
 *
 * ----------------------------------------------------------------------------- */


void free_cfgframe_object(struct cfg_frame *cfg); // Frees memory allocated to cfg objects

void free_2darray_l(long int** array, int x); // frees memory allocated to 2D arrays

void free_2darray(unsigned char** array, int x); // frees memory allocated to 2D arrays

/**************************************** End of File *******************************************************/

