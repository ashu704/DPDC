/* ----------------------------------------------------------------------------- 
 * global.h
 * 		
 *
 * ----------------------------------------------------------------------------- */


#include <mysql.h>
#include <pthread.h>


/* ---------------------------------------------------------------- */
/*                 	Database variables						   */
/* ---------------------------------------------------------------- */

char *server;
char *user;
char *password ;
char *database ;
int BUFF_LEN;

MYSQL *conn_data; // Connection to database for data insertion
MYSQL *conn_cfg;  // Connection to database for cfg insertion

pthread_mutex_t mutex_cfg;  // To lock cfg objects
pthread_mutex_t mutex_file;  // To lock cfg.txt
pthread_mutex_t mutex_MYSQL_CONN_ON_DATA;  // To lock 'conn_data' connection for DATA insertion
pthread_mutex_t mutex_MYSQL_CONN_ON_CFG;   // To lock 'conn_cfg' connection for CFG insertion

pthread_mutex_t mutex_phasor_buffer;
char *phasorBuff;
char *frequencyBuff;
char *digitalBuff;
char *analogBuff;
char *delayBuff;

/**************************************** End of File *******************************************************/
