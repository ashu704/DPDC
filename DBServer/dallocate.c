/* ----------------------------------------------------------------------------- 
 * dallocate.c
 * 		
 *
 * ----------------------------------------------------------------------------- */


#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h> 
#include  "parser.h"
#include  "dallocate.h" 
#include  "global.h"

/* ------------------------------------------------------------------------------------ */
/*                           Functions defined in dallocate.c           		*/
/* ------------------------------------------------------------------------------------ */

/*       1. void free_cfgframe_object(struct cfg_frame *cfg)         			*/
/*       2. void free_2darray(char** array, int x)	           	    		*/

/* ------------------------------------------------------------------------------------ */


/* -------------------------------------------------------------*/
/* FUNCTION  free_cfgframe_object():                  	     	*/
/* It frees memory allocated to cfg objects. 			*/
/* ------------------------------------------------------------ */

void free_cfgframe_object(struct cfg_frame *cfg) { 

	int j = 0;
	struct dgnames *t_dgnames,*r_dgnames; 
	printf("Inside free_cfgframe_object()\n");

	while(j<cfg->num_pmu) {		  					

		if(cfg->pmu[j]->phnmr != 0)
			free_2darray(cfg->pmu[j]->cnext->phnames,cfg->pmu[j]->phnmr);
		if(cfg->pmu[j]->annmr != 0)
			free_2darray(cfg->pmu[j]->cnext->angnames,cfg->pmu[j]->annmr);

		if(cfg->pmu[j]->dgnmr != 0) {

			t_dgnames = cfg->pmu[j]->cnext->first;
			while(t_dgnames != NULL) {

				r_dgnames = t_dgnames->dg_next;
				free_2darray(t_dgnames->dgn,16);
				t_dgnames = r_dgnames;

			}
		}

		if(cfg->pmu[j]->dgnmr != 0) 
			free_2darray(cfg->pmu[j]->dgunit,cfg->pmu[j]->dgnmr);

		j++;
	} // End of While 

	free(cfg);			
}

/* -------------------------------------------------------------*/
/* FUNCTION  free_2darray:  					*/
/* It frees memory allocated to 2D Arrays. 			*/
/* -------------------------------------------------------------*/

void free_2darray_l(long int **array, int x){ 

	int i;
	for(i=0; i<x; i++)
		free(array[i]);
	free(array);
} 

/* -------------------------------------------------------------*/
/* FUNCTION  free_2darray:  					*/
/* It frees memory allocated to 2D Arrays. 			*/
/* -------------------------------------------------------------*/

void free_2darray(unsigned char **array, int x){ 

	int i;
	for(i=0; i<x; i++)
		free(array[i]);
	free(array);
} 

/**************************************** End of File *******************************************************/
