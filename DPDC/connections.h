/* ----------------------------------------------------------------------------- 
 * connections.h
 * ----------------------------------------------------------------------------- */


#include <netinet/in.h>
#include <signal.h>


/* ---------------------------------------------------------------- */
/*                         global variable                          */
/* ---------------------------------------------------------------- */

#define DBPORT 9000     /* The port users will be connecting to */
#define BACKLOG 10      /* How many pending connections queue will hold */
#define MAXBUFLEN 2000

int UL_UDP_addr_len,UL_TCP_sin_size, numbytes;
unsigned char UL_udp_command[19];
unsigned char UL_tcp_command[19];


/* ---------------------------------------------------------------- */
/*                         global Data Structure                    */
/* ---------------------------------------------------------------- */

struct sigaction sa;

struct Lower_Layer_Details {

	unsigned int pmuid;
	char ip[16];
	int port;
	char protocol[4];
	int sockfd;
	int up; 			/* used only in tcp */
	struct sockaddr_in llpmu_addr;
	pthread_t thread_id;
	int data_transmission_off;
	int pmu_remove;
	int request_cfg_frame;
	struct Lower_Layer_Details *next;
	struct Lower_Layer_Details *prev;

}*LLfirst,*LLlast;


struct Upper_Layer_Details {

	char ip[16];
	int port;
	char protocol[4];
	int sockfd;
	int tcpup;
	pthread_t thread_id;
	struct sockaddr_in pdc_addr;
	int config_change;
	int UL_upper_pdc_cfgsent;
	int UL_data_transmission_off;
	int address_set;
	struct Upper_Layer_Details *next;
	struct Upper_Layer_Details *prev;

}*ULfirst,*ULlast;


/* ---------------------------------------------------------------- */
/*                 	Function prototypes   		            */
/* ---------------------------------------------------------------- */

void setup();

void* UL_tcp();

void* UL_tcp_connection(void * newfd); 

void* UL_udp();

void PMU_process_UDP(unsigned char *,struct sockaddr_in,int sockfd);

void PMU_process_TCP(unsigned char tcp_buffer[],int sockfd);

void sigchld_handler(int s);	/* TCP signal handler */

/**************************************** End of File *******************************************************/
