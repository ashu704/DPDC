/* ----------------------------------------------------------------------------- 
 * dallocate.c			
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
/*       2. void free_dataframe_object(struct data_frame *df)				*/
/*       3. void free_2darray(char** array, int x)	           	    		*/

/* ------------------------------------------------------------------------------------ */


/* -------------------------------------------------------------*/
/* FUNCTION  free_cfgframe_object():                  	     	*/
/* It frees memory allocated to cfg objects. 			*/
/* ------------------------------------------------------------ */

void free_cfgframe_object(struct cfg_frame *cfg) { 

	int j = 0;
	unsigned int phn,ann,dgn,num_pmu;
	struct dgnames *t_dgnames,*r_dgnames; 

	num_pmu = to_intconvertor(cfg->num_pmu);

	while(j<num_pmu) {		  					

		free(cfg->pmu[j]->stn);
		free(cfg->pmu[j]->idcode);
		free(cfg->pmu[j]->data_format);
		free(cfg->pmu[j]->fmt);

		// Extract PHNMR, DGNMR, ANNMR
		phn = to_intconvertor(cfg->pmu[j]->phnmr);
		ann = to_intconvertor(cfg->pmu[j]->annmr);
		dgn = to_intconvertor(cfg->pmu[j]->dgnmr);

		if(phn != 0)
			free_2darray(cfg->pmu[j]->cnext->phnames,phn);
		if(ann != 0)
			free_2darray(cfg->pmu[j]->cnext->angnames,ann);

		if(dgn != 0) {
			t_dgnames = cfg->pmu[j]->cnext->first;

			while(t_dgnames != NULL) {

				r_dgnames = t_dgnames->dg_next;
				free_2darray(t_dgnames->dgn,16);
				t_dgnames = r_dgnames;
			}
		}

		if(phn != 0)
			free_2darray(cfg->pmu[j]->phunit,phn);
		if(ann != 0)
			free_2darray(cfg->pmu[j]->anunit,ann);
		if(dgn != 0)
			free_2darray(cfg->pmu[j]->dgunit,dgn);

		free(cfg->pmu[j]->phnmr);
		free(cfg->pmu[j]->annmr);
		free(cfg->pmu[j]->dgnmr);
		free(cfg->pmu[j]->fnom);
		free(cfg->pmu[j]->cfg_cnt);

		j++;
	} // End of While 

	free(cfg->framesize);
	free(cfg->idcode);
	free(cfg->soc);
	free(cfg->fracsec);
	free(cfg->time_base);
	free(cfg->data_rate);
	free(cfg->num_pmu);
	free(cfg);			
}


/* -------------------------------------------------------------*/
/* FUNCTION  free_dataframe_object():                  	     	*/
/* It frees memory allocated to data objects. 			*/
/* -------------------------------------------------------------*/

void free_dataframe_object(struct data_frame *df) { 

	int j = 0;

	// Free the blocks
	while(j < df->num_pmu) {		  					

		if(df->dpmu[j]->stat[1] == 0x0F) {

			free(df->dpmu[j]->stat);						
			j++;
			continue;
		}

		free(df->dpmu[j]->stat);		
		free(df->dpmu[j]->freq);
		free(df->dpmu[j]->dfreq);

		if(df->dpmu[j]->phnmr != 0)
			free_2darray(df->dpmu[j]->phasors,df->dpmu[j]->phnmr);
		if(df->dpmu[j]->annmr != 0)
			free_2darray(df->dpmu[j]->analog,df->dpmu[j]->annmr);
		if(df->dpmu[j]->dgnmr != 0)
			free_2darray(df->dpmu[j]->digital,df->dpmu[j]->dgnmr);
		j++;
	} // End of While

	free(df->framesize);
	free(df->idcode);
	free(df->soc);
	free(df->fracsec);
	free(df);		
}


/* -------------------------------------------------------------*/
/* FUNCTION  free_2darray:  					*/
/* It frees memory allocated to 2D Arrays. 			*/
/* -------------------------------------------------------------*/

void free_2darray(unsigned char** array, int n){ 

	int i;
	for(i = 0; i < n; i++) 
		free(array[i]);
	free(array);
} 

/**************************************** End of File *******************************************************/
