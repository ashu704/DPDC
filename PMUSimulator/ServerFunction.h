/* ----------------------------------------------------------------------------- 
 * ServerFunction.h
 * ----------------------------------------------------------------------------- */


#include  <stdio.h>
#include  <gtk/gtk.h>
#include  <pthread.h>
#include  <netinet/in.h>

#define BACKLOG 10            /* How many pending connections queue will hold */
#define MAX_STRING_SIZE 5000

/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

int cfg_pmu_trigger, dataFileVar; 
int cfg_change, cfg_invalid_data; 
int cfg_pmu_error, cfg_sorting_data, cfg_crc_error; 
int temp_pahsor_type[50], temp_analog_type[50];

long int temp_PHUNIT_val[50], temp_ANUNIT_val[50];

char pmuFolderPath[200];
char pmuFilePath[200];

unsigned char cline[MAX_STRING_SIZE];
unsigned char hdrline[MAX_STRING_SIZE];
unsigned char data_frm[MAX_STRING_SIZE], df_temp[2], df_temp_1[4];

pthread_t UDP_thread, TCP_thread;

FILE *fp_DataFile;


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

void  frame_size();

void generate_data_frame();

void* udp_send_data();

void* pmu_udp();

void* tcp_send_data(void * newfd);

void* new_pmu_tcp(void * nfd);

void* pmu_tcp();

void start_server();

int get_header_frame();

void  SIGUSR1_handler(int);   /* Signal handler */

void  SIGUSR2_handler(int);   /* Signal handler */

/**************************************** End of File *******************************************************/
