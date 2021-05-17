/* ----------------------------------------------------------------------------- 
 * global.h			
 *
 * ----------------------------------------------------------------------------- */


#include <pthread.h>
#include  <netinet/in.h>
#include  <stdio.h>

#define MAX_STRING_SIZE 5000


/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

pthread_mutex_t mutex_cfg;  /* To lock cfg data objects */
pthread_mutex_t mutex_file;  /* To lock PMU Setup File */
pthread_mutex_t mutex_Lower_Layer_Details;  /* To lock objects of connection table that hold lower layer PMU/PDC ip and protocol */
pthread_mutex_t mutex_Upper_Layer_Details;  /* To lock objects of connection table that hold upper layer PDC ip and protocol */
pthread_mutex_t mutex_status_change;

unsigned char *cfgframe,*dataframe;

struct sockaddr_in UDP_my_addr,TCP_my_addr; /* my address information */
struct sockaddr_in UL_UDP_addr,UL_TCP_addr; /* connector’s address information */
int UL_UDP_sockfd,UL_TCP_sockfd; /* socket descriptors */
pthread_t UDP_thread,TCP_thread,p_thread,Deteache_thread;

FILE *fp_log,*fp_updc;
char tname[20];

/* DPDC Setup File path globaly */
char dpdcFolderPath[200];
char dpdcFilePath[200];


/* --------------------------------------------------------------------	*/
/*				global DataBase variables		*/
/* --------------------------------------------------------------------	*/

struct sockaddr_in DB_Server_addr; // address information
int DB_sockfd,DB_addr_len;

int PDC_IDCODE,TCPPORT,UDPPORT;
long int TSBWAIT;
char dbserver_ip[20];

unsigned char DATASYNC[3],CFGSYNC[3],CMDSYNC[3],CMDDATASEND[3],CMDDATAOFF[3],CMDCFGSEND[3];

/**************************************** End of File *******************************************************/
