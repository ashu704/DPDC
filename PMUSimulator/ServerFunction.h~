/* ----------------------------------------------------------------------------- 
 * ServerFunction.h
 * 
 * PMU Simulator - Phasor Measurement Unit Simulator
 *
 * Copyright (C) 2011-2012 Nitesh Pandit
 * Copyright (C) 2011-2012 Kedar V. Khandeparkar
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Authors: 
 *		Nitesh Pandit <panditnitesh@gmail.com>
 *		Kedar V. Khandeparkar <kedar.khandeparkar@gmail.com>			
 *
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
