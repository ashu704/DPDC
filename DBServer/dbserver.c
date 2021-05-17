/* ----------------------------------------------------------------------------- 
 * dbserver.c		
 *
 * ----------------------------------------------------------------------------- */


#include  <stdio.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdlib.h> 
#include  <string.h>
#include  "connections.h"
#include  "parser.h"
#include  "recreate.h"

/* ---------------------------------------------------------------- */
/*                   main program starts here                       */
/* ---------------------------------------------------------------- */

int main(void) {

	system("clear");	
	recreate_cfg_objects() ;
	setup();
	return 0;
}

/**************************************** End of File *******************************************************/
