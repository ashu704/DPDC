/* ----------------------------------------------------------------------------- 
 * connections.h
 * 
 * DPDC - Distribution Phasor Data Concentrator		
 *
 * ----------------------------------------------------------------------------- */


#include <netinet/in.h>
#include <signal.h>

/* ---------------------------------------------------------------- */
/*                         global variable                          */
/* ---------------------------------------------------------------- */

#define DBPORT 9000     
#define MAXBUFLEN 2000

int DB_sockfd; //socket descriptor
struct sockaddr_in DB_server_addr,their_addr; 
int DB_addr_len,numbytes;
unsigned char buffer[MAXBUFLEN];

/* ---------------------------------------------------------------- */
/*                 	Function prototypes   		            */
/* ---------------------------------------------------------------- */

void setup();

void DB_udp();

void* DB_udphandler(void * udp_BUF);

void DB_process_UDP(unsigned char* udp_BUF);

/**************************************** End of File *******************************************************/
