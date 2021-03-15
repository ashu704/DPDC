/* ----------------------------------------------------------------------------- 
 * ServerFunction.c
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "function.h"
#include "ServerFunction.h"
#include "ShearedMemoryStructure.h"


/* -------------------------------------------------------------------------------------- */
/*                            Functions in ServerFunction.c                               */
/* -------------------------------------------------------------------------------------- */

/* ----------------------------------------- */
/*                                           */
/*   1. int   get_header_frame();            */
/*	2. void  frame_size();                  */
/*	3. void  generate_data_frame();		*/
/*	4. void* udp_send_data();			*/
/*	5. void* pmu_udp();					*/
/*	6. void* tcp_send_data(void * newfd);	*/
/*	7. void* new_pmu_tcp(void * nfd);		*/
/*	8. void* pmu_tcp();					*/
/*	9. void  start_server();				*/
/*   10.void  SIGUSR1_handler(int);          */
/*   11.void  SIGUSR2_handler(int);          */
/*                                           */
/* ----------------------------------------- */


/* ---------------------------------------------------------------- */
/*                         global variables                         */
/* ---------------------------------------------------------------- */

int df_pmu_id, df_fdf, df_af, df_pf, df_pn, df_phnmr, df_annmr, df_dgnmr;
int df_data_frm_size = 0, df_data_rate = 0, old_data_rate = 0, cfg_size, hdr_size=0;
int count = 0, pmuse=0, sc1 = 0, PORT, tmp_wait = 1, protocol_choice, df_fnom;
int UDP_sockfd, TCP_sockfd, yes=1, TCP_sin_size, UDP_addr_len, PhasorType[50];
int udp_cfg_flag = 0, tcp_cfg_flag = 0, tcp_data_flag = 0, udp_data_flag = 0;
int err, errno, udp_data_trans_off = 1, tcp_data_trans_off = 1, stat_flag = 0;
long int df_soc, fsec = 0, curnt_soc = 0, prev_soc = 0, udp_send_thrd_id = 0, tcp_send_thrd_id = 0,soc1,soc2;

struct sockaddr_in UDP_my_addr, TCP_my_addr, UDP_addr, TCP_addr;
struct sigaction sa;
pthread_t pmu_server_thread;
uint16_t df_chk;

/* ----------------------------------------------------------------------------	*/
/* FUNCTION  get_header_frame():							               */
/* This function get the header frame from the PMU Setup File.                  */
/* ----------------------------------------------------------------------------	*/

int get_header_frame()
{
	/* Local variables */
	int tempi;
	char *rline = NULL, *d1;
	ssize_t read;
	size_t len = 0;
	FILE *fp1;

	/* Open the PMU Setup File to read the header frame, if present in file? */
	fp1 = fopen (pmuFilePath,"rb");

	tempi = 1;

     /* Read all the unnecessary lines - PMUServer and CFG */
	while(tempi < 6)
	{
		read = getline(&rline, &len, fp1);

		if(read == 0)
			break;

		tempi++;
	}

	if(read > 0)
	{
		d1 = strtok (rline," ");
		d1 = strtok (NULL," ");
		d1 = strtok (NULL,"\n");
		tempi = atoi(d1);

		if(tempi > 0)
		{
			fread(hdrline, sizeof(unsigned char), tempi, fp1);
			fclose(fp1);
			return 1;
		}
	}
	else
	{
		printf("\nHeader Frame not created by the PMU operator.\n");
	}

	return 0;
}


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  frame_size():							                    */
/* Function To calculate the recent data frame size via reading cfg frm.        */
/* Also initializing some of global variables. 	                              */
/* ----------------------------------------------------------------------------	*/

void frame_size()
{
	/* Local variables */
	int format, i, j;
	int tempi,index=2;
	long int temp_li; 
	char filename[200];
	char *rline = NULL, *d1;
	ssize_t read;
	size_t len = 0;
	FILE *fp1;

	strcpy(filename, pmuFilePath);

	/* Open the PMU Setup File and read the last CFG frame */
	fp1 = fopen (filename,"rb");

	if (fp1 != NULL)			
	{
		tempi = 1;

          /* Read all the unnecessary lines - PMUServer only */
		while(tempi < 4)
		{
			read = getline(&rline, &len, fp1);

			if(read == 0)
				break;

			tempi++;
		}

		if(read > 0)
		{
			d1 = strtok (rline," ");
			d1 = strtok (NULL," ");
			tempi = atoi(d1);

			if (tempi == 1)
			{
				memset(cline,'\0',sizeof(cline));

				d1 = strtok (NULL,"\n");
				tempi = atoi(d1);

                    /* Copy the complete Configuration frame into an unsigned charactor array called cline */
				fread(cline, sizeof(unsigned char), tempi, fp1);
				fclose(fp1);

				/* Get the CFG size & store globally */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				cfg_size = c2i(df_temp);
				cline[cfg_size] = '\0';

				/* Get the PMU ID from CFG FRM & store globally */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				df_pmu_id = c2i(df_temp);
				index = index + 32;

				/* Get the FORMAT word from CFG FRM */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				format = c2i(df_temp);

				/* Initialize the format bits as in his appropriate global variable */
				if(format == 15)
				{
					df_fdf=1, df_af=1, df_pf=1, df_pn=1;
				}
				else if(format == 14)
				{
					df_fdf=1, df_af=1, df_pf=1, df_pn=0;
				}
				else if(format == 13)
				{
					df_fdf=1, df_af=1, df_pf=0, df_pn=1;
				}
				else if(format == 12)
				{
					df_fdf=1, df_af=1, df_pf=0, df_pn=0;
				}
				else if(format == 11)
				{
					df_fdf=1, df_af=0, df_pf=1, df_pn=1;
				}
				else if(format == 10)
				{
					df_fdf=1, df_af=0, df_pf=1, df_pn=0;
				}
				else if(format == 9)
				{
					df_fdf=1, df_af=0, df_pf=0, df_pn=1;
				}
				else if(format == 8)
				{
					df_fdf=1, df_af=0, df_pf=0, df_pn=0;
				}
				else if(format == 7)
				{
					df_fdf=0, df_af=1, df_pf=1, df_pn=1;
				}
				else if(format == 6)
				{
					df_fdf=0, df_af=1, df_pf=1, df_pn=0;
				}
				else if(format == 5)
				{
					df_fdf=0, df_af=1, df_pf=0, df_pn=1;
				}
				else if(format == 4)
				{
					df_fdf=0, df_af=1, df_pf=0, df_pn=0;
				}
				else if(format == 3)
				{
					df_fdf=0, df_af=0, df_pf=1, df_pn=1;
				}
				else if(format == 2)
				{
					df_fdf=0, df_af=0, df_pf=1, df_pn=0;
				}
				else if(format == 1)
				{
					df_fdf=0, df_af=0, df_pf=0, df_pn=1;
				}
				else
				{
					df_fdf=0, df_af=0, df_pf=0, df_pn=0;
				}

				/* Get the PHNMR from CFG FRM & store globally */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				df_phnmr = c2i(df_temp);

				/* Get the df_annmr from CFG FRM & store globally */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				df_annmr = c2i(df_temp);

				/* Get the df_dgnmr from CFG FRM & store globally */
				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				df_dgnmr = c2i(df_temp);

				/* To escape the some of fields in cfg frame */
				index = index + (16*df_phnmr) + (16*df_annmr) + (256*df_dgnmr);

				/* Extract the value of PHUNIT for each Phasor channel */
				for(i=0, j=0; i<(df_phnmr); i++, j++)
				{
					if(cline[index++] == 0)
						temp_pahsor_type[j] = 0;
					else
						temp_pahsor_type[j] = 1;

					temp_li =cline[index++];
					temp_li<<=8;
					temp_li |=cline[index++];
					temp_li<<=8;
					temp_li |=cline[index++];

					temp_PHUNIT_val[j] = temp_li;
				}

				/* Extract the value of ANUNIT for each Analog channel */
				for(i=0, j=0; i<(df_annmr); i++, j++)
				{
					temp_analog_type[j] = (int)cline[index++];

					temp_li =cline[index++];
					temp_li<<=8;
					temp_li |=cline[index++];
					temp_li<<=8;
					temp_li |=cline[index++];

					temp_ANUNIT_val[j] = temp_li;
				}
				index = index + (4*df_dgnmr); // for skiping 2 byte for DIGUNIT

				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				int temp_df_fnom = c2i(df_temp);
				if(temp_df_fnom == 0)
                         {
     					df_fnom = 60;
                              printf("Set Fnom = 60\n");                    
                         }
				else
                         {
     					df_fnom = 50;
                              printf("Set Fnom = 50\n");                    
                         }

				df_temp[0] = cline[index++];
				df_temp[1] = cline[index++];
				df_data_rate = c2i(df_temp);

				/* Calculate the data frame size */
				df_data_frm_size = 0;
				df_data_frm_size = df_data_frm_size + 18;	/* 18 Bytes or 36 char is sum of all static fields in data frame */

				/* Calculate 4/8 bytes for each PHNMR & store globally */
				if (df_pf == 0)
				{
					df_data_frm_size = df_data_frm_size + (4*df_phnmr);
				}
				else
				{
					df_data_frm_size = df_data_frm_size + (8*df_phnmr);
				}

				/* Calculate 2/4 bytes for each df_annmr & store globally */
				if (df_af == 0)
				{
					df_data_frm_size = df_data_frm_size + (2*df_annmr);
				}
				else
				{
					df_data_frm_size = df_data_frm_size + (4*df_annmr);
				}

				/* Calculate 2/4 bytes for both (FREQ + DFREQ) & store globally */
				if (df_fdf == 0)
				{
					df_data_frm_size = df_data_frm_size + 4;
				}
				else
				{
					df_data_frm_size = df_data_frm_size + 8;
				}

				/* Calculate 2 bytes for each DGNMR & store globally */
				df_data_frm_size = df_data_frm_size + (2*df_dgnmr);
				printf("Server side calculated data frame size = %d\n", df_data_frm_size);
			}
		}
	} /* end of else of fopen*/

	else
		exit(1);
} /* end of function frame_size() */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  generate_data_frame():	               					*/
/* Function to generate the data frame. Based on the Configuration Frame 	     */
/* attributes.                          					               */
/* ----------------------------------------------------------------------------	*/

void generate_data_frame()
{
	/* local variables */
	int freqI, phasorI, analogI;
	int indx = 0, j, temp_i, freq, dfreq, dsw = 0, ka = 1;
	int analog[3] = {100, 1000, 10000}, rand_ph, rand_ang;
	long int freq_f, dfreq_f, analog_f;

	float freqF, phasorF, analogF;
	float phasor = 100.85, angle, result;

	/* If configuration has changed then call the function "frame_size()" to read new CFG and
        reinitialize global variables & generate new Data frames with STAT word bit-10(CFG change bit)
        set to 1 till CFG request not received from connected PDC */

	if ((cfg_change == 1) || (cfg_pmu_error == 1))
	{
		frame_size();
	}

	memset(data_frm,'\0',sizeof(df_data_frm_size));

	/* Insert SYNC Word in data frame */
	data_frm[indx++] = 0xAA; 
	data_frm[indx++] = 0x01; 

	/* Insert data frame size in data frame */
	i2c(df_data_frm_size, df_temp);
	B_copy(data_frm, df_temp, indx, 2);
	indx = indx + 2;

	/* Insert PMU ID in data frame */
	i2c(df_pmu_id, df_temp);
	B_copy(data_frm, df_temp, indx, 2);
	indx = indx + 2;

	/* Insert SOC value in data frame */
	time_t tm = time(NULL);
	df_soc = tm;
	li2c(df_soc, df_temp_1);
	B_copy(data_frm, df_temp_1, indx, 4);
	indx = indx + 4;

     /* No PPS so have to manage by seeing local time */
	if (sc1 == 0)
	{
		soc1 = df_soc;
		soc2 = df_soc;
		sc1 = 1;
	}
	else
	{
		soc2 = df_soc;
	}

	/* Insert Time Quality flag + fraction of second in data frame */
	if ((soc2 > soc1) || fsec > ((df_data_rate-1) * (1000000/df_data_rate)))
	{	
		fsec = 0;
	}
	li2c(fsec, df_temp_1);
	B_copy(data_frm, df_temp_1, indx, 4);
	indx = indx + 4;
	soc1 = soc2;
	fsec = fsec+(1000000/df_data_rate);   

	/* Insert STAT Word in data frame Default or Changed */
	time_t curnt_soc = time(NULL);
	if(pmuse == 0) 
	{
		prev_soc = curnt_soc;
	} 
	if (cfg_change == 1)
	{
		printf("\tSTAT word Changed due to Configuration Change.");
		data_frm[indx++] = 0x04;
		data_frm[indx++] = 0x00;
	}
	else if (cfg_pmu_error == 1)
	{
		printf("\tSTAT word Changed due to PMU Error.");
		data_frm[indx++] = 0x40;
		data_frm[indx++] = 0x00;
	}
	else if((curnt_soc-prev_soc) > 1)
	{ 
		printf("\tSTAT word Changed due to PMU SYNC Error.");
		data_frm[indx++] = 0x20;
		data_frm[indx++] = 0x00;
	}
	else if (cfg_sorting_data == 1)
	{
		printf("\tSTAT word Changed due to Data sorting.");
		data_frm[indx++] = 0x10;
		data_frm[indx++] = 0x00;
		cfg_sorting_data = 0;
	}
	else if (cfg_pmu_trigger == 1)
	{
		printf("\tSTAT word Changed due to PMU trigger detected.");
		data_frm[indx++] = 0x08;
		data_frm[indx++] = 0x00;
		cfg_pmu_trigger = 0;
	}
	else if (cfg_invalid_data == 1)
	{
		printf("\tSTAT word Changed due to Data Invalidation.");
		data_frm[indx++] = 0x80;
		data_frm[indx++] = 0x00;
		cfg_invalid_data = 0;
	}
	else
	{
		/* If not insert default STAT Word: 0000 */
		data_frm[indx++] = 0x00;
		data_frm[indx++] = 0x00;
	}
	prev_soc = curnt_soc;
	pmuse = 1;

	/*----------------Auto Generated Data------------------*/	
	if(dataFileVar == 0)
	{
//		printf("Auto - ");

		/* Insert Fix point phasor values in data frame */
		if(df_pf == 0)		
		{
			/* For rendom phasor values */
			if(df_pn == 0)	/* Rectangular */
			{
				for(j=0; j<df_phnmr; j++)
				{
					rand_ph = rand() % 9 + 1;
					rand_ang = rand() % 29 + 1;
					angle = 120.89;

					if(ka == 1)
					{
						phasor = phasor + rand_ph;
						angle = angle + rand_ang;
						ka = 0;
					}
					else
					{
						phasor = phasor - rand_ph;
						angle = angle - rand_ang;
						ka = 1;
					}

					angle = (angle*3.1415)/180;
					result = cos(angle)*phasor;
					temp_i = 65535+result;
					i2c(temp_i, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;

					temp_i = sin(angle)*phasor;
					i2c(temp_i, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;
				}
			}
			else	/* Polar */
			{
				/* For rendom phasor values */
				for(j=0; j<df_phnmr; j++)
				{
					rand_ph = rand() % 9 + 1;
					rand_ang = rand() % 29 + 1;
					angle = 120.89;

					if(ka == 1)
					{
						phasor = phasor + rand_ph;
						angle = angle + rand_ang;
						ka = 0;
					}
					else
					{
						phasor = phasor - rand_ph;
						angle = angle - rand_ang;
						ka = 1;
					}

					angle = ((angle*3.1415)/180)*100000;
					temp_i = phasor*100000;
					i2c(temp_i, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;

					temp_i = angle;
					i2c(temp_i, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;
				}
			}
		}
		else	      /* Insert Floating point phasor values in data frame */
		{
			if(df_pn == 0)	/* Rectangular */
			{
				/* For rendom phasor values */
				for(j=0; j<df_phnmr; j++)
				{
					rand_ph = rand() % 9 + 1;
					rand_ang = rand() % 29 + 1;
					angle = 120.89;

					if(ka == 1)
					{
						phasor = phasor + rand_ph;
						angle = angle + rand_ang;
						ka = 0;
					}
					else
					{
						phasor = phasor - rand_ph;
						angle = angle - rand_ang;
						ka = 1;
					}

					angle = (angle*3.1415)/180;
					result = cos(angle)*phasor;
					result = 65535+result;
					f2c(result, df_temp_1);
					B_copy(data_frm, df_temp_1, indx, 4);
					indx = indx + 4;

					result = sin(angle)*phasor;
					f2c(result, df_temp_1);
					B_copy(data_frm, df_temp_1, indx, 4);
					indx = indx + 4;
				}
			}
			else	  /* Polar */
			{
				/* For rendom phasor values */
				for(j=0; j<df_phnmr; j++)
				{
					rand_ph = rand() % 9 + 1;
					rand_ang = rand() % 29 + 1;
					angle = 120.89;

					if(ka == 1)
					{
						phasor = phasor + rand_ph;
						angle = angle + rand_ang;
						ka = 0;
					}
					else
					{
						phasor = phasor - rand_ph;
						angle = angle - rand_ang;
						ka = 1;
					}

					angle = (angle*3.1415)/180;
					result = phasor;
					f2c(result, df_temp_1);
					B_copy(data_frm, df_temp_1, indx, 4);
					indx = indx + 4;

					result = angle;
					f2c(result, df_temp_1);
					B_copy(data_frm, df_temp_1, indx, 4);
					indx = indx + 4;
				}
			}
		}

		/* Insert Fix point Frequency & DFrequency values in data frame */
		if(df_fdf == 0)
		{
			/* For rendom values of FREQ & DFREQ */
			freq = (rand() % 5 + 1)*100;
			i2c(freq, df_temp);
			B_copy(data_frm, df_temp, indx, 2);
			indx = indx + 2;

			dfreq = 0;
			i2c(dfreq, df_temp);
			B_copy(data_frm, df_temp, indx, 2);
			indx = indx + 2;
		}
		else	      	/* Insert Floating point Frequency & DFrequency values in data frame */
		{
			/* For rendom values of FREQ & DFREQ */
			freq_f = (rand() % 5 + 1)*100;
			li2c(freq_f, df_temp_1);
			B_copy(data_frm, df_temp_1, indx, 4);
			indx = indx + 4;

			dfreq_f = (rand() % 5 + 1)*0.00639;
			i2c(dfreq_f, df_temp_1);
			B_copy(data_frm, df_temp_1, indx, 4);
			indx = indx + 4;
		}

		/* Insert Fix point Analog values in data frame */
		if(df_af == 0)
		{
			for(j=0, ka=0; ka<df_annmr; j++, ka++)
			{
				if (j == 3) j = 0;
				i2c(analog[j], df_temp);
				B_copy(data_frm, df_temp, indx, 2);
				indx = indx + 2;
			}
		}
		else      /* Insert Floating point Analog values in data frame */
		{
			for(j=0, ka=0; ka<df_annmr; j++, ka++)
			{
				if (j == 3) j = 0;
				analog_f = analog[j];
				li2c(analog_f, df_temp_1);
				B_copy(data_frm, df_temp_1, indx, 4);
				indx = indx + 4;
			}
		}
	}

	/*----------------Read measurements from file------------------*/	
	else
	{
		char *mData, *d1, *d2;
		int j;

		//printf("File - ");

		mData = measurement_Return ();
		d1 = strtok (mData,","); 

		/* Insert Fix point phasor values in data frame */
		if(df_pf == 0)		
		{
			for(j=0; j<df_phnmr; j++)
			{
				if(df_pn == 0)	/* Rectangular Values */
				{
					d1 = strtok (NULL,",\""); 
					phasorI = (atof(d1)*100000/temp_PHUNIT_val[j]);
					i2c(phasorI, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;

					d2 = strtok (NULL,",\""); 
					phasorI = (atof(d2)*100000/temp_PHUNIT_val[j]);
					i2c(phasorI, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;
				}
				else	/* Polar Values */
				{
					d1 = strtok (NULL,",\""); 
					phasorI = (atof(d1)*100000/temp_PHUNIT_val[j]);
					i2c(phasorI, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;

					d2 = strtok (NULL,",\""); 
					phasorI = (((atof(d2)*M_PI)/180)*10000);
					i2c(phasorI, df_temp);
					B_copy(data_frm, df_temp, indx, 2);
					indx = indx + 2;
				}
			}
		}
		else	      /* Insert Floating point phasor values in data frame */
		{
			for(j=0; j<df_phnmr; j++)
			{
				d1 = strtok (NULL,",\""); 
				phasorF = atof(d1);
				f2c(phasorF, df_temp_1);
				B_copy(data_frm, df_temp_1, indx, 4);
				indx = indx + 4;

				d2 = strtok (NULL,",\""); 
				phasorF = atof(d2);
				f2c(phasorF, df_temp_1);
				B_copy(data_frm, df_temp_1, indx, 4);
				indx = indx + 4;
			}
		}

		/* Insert Fix point Frequency & DFrequency values in data frame */
		if(df_fdf == 0)
		{
			/* For values of FREQ & DFREQ */
			d1 = strtok (NULL,",\""); 
			freqI = (atof(d1)-df_fnom)*1000;
			i2c(freqI, df_temp);
			B_copy(data_frm, df_temp, indx, 2);
			indx = indx + 2;

			d2 = strtok (NULL,",\""); 
			freqI = (atof(d2)*100);
			i2c(freqI, df_temp);
			B_copy(data_frm, df_temp, indx, 2);
			indx = indx + 2;
		}
		else	      	/* Insert Floating point Frequency & DFrequency values in data frame */
		{
			/* For values of FREQ & DFREQ */
			d1 = strtok (NULL,",\""); 
			freqF = atof(d1);
			f2c(freqF, df_temp_1);
			B_copy(data_frm, df_temp_1, indx, 4);
			indx = indx + 4;

			d2 = strtok (NULL,",\""); 
			freqF = atof(d2);
			f2c(freqF, df_temp_1);
			B_copy(data_frm, df_temp_1, indx, 4);
			indx = indx + 4;
		}

		/* Insert Fix point Analog values in data frame */
		if(df_af == 0)
		{
			for(j=0; j<df_annmr; j++)
			{
				d1 = strtok (NULL,",\"");
				analogI = (atof(d1)*1e5/temp_ANUNIT_val[j]);
				i2c(analogI, df_temp);
				B_copy(data_frm, df_temp, indx, 2);
				indx = indx + 2;
			}
		}
		else      /* Insert Floating point Analog values in data frame */
		{
			for(j=0; j<df_annmr; j++)
			{
				d2 = strtok (NULL,",\""); 
				analogF = (atof(d2));
				f2c(analogF, df_temp_1);
				B_copy(data_frm, df_temp_1, indx, 4);
				indx = indx + 4;
			}
		}
	} /* end of measurements from file */

	/* Insert Digital values in data frame */
	for(j=1; j<=df_dgnmr; j++)
	{
		i2c(dsw, df_temp);
		B_copy(data_frm, df_temp, indx, 2);
		indx = indx + 2;
	}

	/* Calculate and insert the Checksum value in data frame (till now) */
	df_chk = compute_CRC(data_frm,indx);

     /* Right checksum calculation */
	if (cfg_crc_error == 0)
	{
	     data_frm[indx++] = (df_chk >> 8) & ~(~0<<8);  	/* CHKSUM high byte; */
	     data_frm[indx++] = (df_chk ) & ~(~0<<8);     	/* CHKSUM low byte;  */
	}
	else
	{
		printf("\nInvalid CheckSum in a sending Data Frame.\n");
	     data_frm[indx++] = (df_chk ) & ~(~0<<8);     	/* CHKSUM low byte;  */
	     data_frm[indx++] = (df_chk >> 8) & ~(~0<<8);  	/* CHKSUM high byte; */
          cfg_crc_error = 0;
	}
     
	/* Print the generated data frame */
/*	printf("[");
	for(j=0; j<indx; j++)
	{
		printf("%x",data_frm[j]);
	}
	printf("]-(%d)\n", indx);
*/
} /* end of function generate_data_frame() */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  void* udp_send_data():	               					*/
/* This function run by a seprate thread only for data transmission.            */
/* Function to generate and send the data frame periodically to client's   	*/
/* destination address or to PDC client.                                        */
/* ----------------------------------------------------------------------------	*/

void* udp_send_data()
{
     /* Calculate the waiting time during sending data frames */
	int udp_data_waiting = 1000000/df_data_rate, i=1;

	udp_data_flag = 1;
	udp_data_trans_off = 1;
	udp_send_thrd_id = pthread_self();

	while(1)
	{
//		printf("%d. ", i); 
		if ( i == df_data_rate) i=0;

		/* Call the function generate_data_frame() to create a fresh new Data Frame */
		generate_data_frame();

          /* Send the newly created data frame to connected PDC address */
		if (sendto(UDP_sockfd, data_frm, df_data_frm_size, 0, (struct sockaddr *)&UDP_addr,sizeof(UDP_addr)) == -1)
		{
			perror("sendto");
		}
		i++;
		usleep(udp_data_waiting);			
	}
} /* end of function udp_send_data() */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION void* pmu_udp():							                    */
/* This is a UDP Server of PMU and it will continuously on listening mode.      */
/* Function for receives frames from authentic PDC & reply back the 	     	*/
/* requested frame (if available) to PDC.					               */
/* It will create separate thread only for data transmission.                   */
/* ----------------------------------------------------------------------------	*/

void* pmu_udp()
{
	/* local variables */
	unsigned char c;
	int n, start1_u = 0, ind, id_pdc;
	char udp_command[18],filename1[200];

	FILE *fp1;
	pthread_t udp_send;

	/* Call the function frame_size() to initialized all globals as Configuration frame  */
     /* This will wait until CFG has not been set by user.                                */
	while(1)
	{
		usleep(1000);
		strcpy(filename1, pmuFilePath);

		if(pmuFilePath == NULL)
			exit(1);

		if((fp1=fopen(filename1, "rb"))!=NULL)
		{
			fclose(fp1);
			printf("Configuration File Found.\n");
			break;
		}
		else 
			continue;
	}

     /* Now calculate the global variables and other from newly stablished PMU Setup File */
	frame_size();

     /* This while is always in listening mode to receiving frames from PDC and their respective reply */
	while(1)	
	{
		memset(udp_command,'\0',18);
		ind = 2;

          /* UDP data Received */
		if ((recvfrom(UDP_sockfd, udp_command, 18, 0, (struct sockaddr *)&UDP_addr, (socklen_t *)&UDP_addr_len)) == -1)
		{ 
			perror("recvfrom");
			exit(1);
		}
		else		/* New datagram has been received */
		{ 
			c = udp_command[1];
			c <<= 1;
			c >>= 5;

			if(c  == 0x04) 		/* Check if it is a command frame from PDC */ 
			{	
				df_temp[0] = udp_command[4];
				df_temp[1] = udp_command[5];
				id_pdc  = c2i (df_temp);

				if(id_pdc == df_pmu_id)		/* Check if it is coming from authentic PDC/iPDC */ 
				{	
					c = udp_command[15];

					if((c & 0x05) == 0x05)		/* Command frame for Configuration Frame request from PDC */
					{ 				
						printf("\n\n\nConfiguration Frame request is received.\n"); 
						fp1 = fopen (filename1,"rb");

						if (fp1 == NULL)
						{
							perror (filename1); 
							printf("\nConfiguration Frame is not present in PMU Setup File.\n");
							exit(1);
						}
						else 
						{ 
							fclose(fp1);

							/* Get the CFG size & store in global variable */
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind];
							cfg_size = c2i(df_temp);
							cline[cfg_size] = '\0';

							/* Send Configuration frame to PDC Device */
							if (sendto(UDP_sockfd,cline, cfg_size, 0, (struct sockaddr *)&UDP_addr,sizeof(UDP_addr)) == -1)
							{
								perror("sendto");
							}

							udp_cfg_flag = 1;
							cfg_change = 0;
							cfg_pmu_error = 0;
							ind = cfg_size - 4;
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind];
							df_data_rate = c2i(df_temp);

							if (start1_u > 0)		
							{
								if(old_data_rate != df_data_rate)
								{
									/* Cancle the data sending thread if data rate is changed */
									n = pthread_cancel(udp_send_thrd_id);
									if (n == 0)
									{
										if( (err = pthread_create(&udp_send,NULL,udp_send_data,NULL)))
										{ 
											perror(strerror(err));
											exit(1);
										}
									}
								}
							}

							printf("\nThe Configuration frame [%d Bytes] is sent to requested PDC.\n", cfg_size);
							old_data_rate = df_data_rate;			
							start1_u = 1;
						} 
					}

					else if((c & 0x03) == 0x03)		/* Command frame for Header frame request from PDC */
					{ 				
						printf("\nHeader frame request is Received.\n"); 
						fp1 = fopen (filename1,"rb");

						if (fp1 == NULL)
						{
							printf("\nHeader Frame is not present in PMU Setup File.\n");
							exit(1);
						}
						else 
						{ 
							fclose(fp1);

							if(get_header_frame() == 1)
							{
								/* Get the CFG size & store in global variable */
								df_temp[0] = hdrline[2];
								df_temp[1] = hdrline[3];
								hdr_size = c2i(df_temp);
								hdrline[hdr_size] = '\0';

								/* Send Header frame to PDC Device */
								if (sendto(UDP_sockfd,hdrline, hdr_size, 0, (struct sockaddr *)&UDP_addr,sizeof(UDP_addr)) == -1)
								{
									perror("sendto");
								}

								printf("\nHeader Frame is sent to requested PDC.\n");
							}
						} 
					}

					else if((c & 0x01) == 0x01)		/* Command frame for Turn off transmission request from PDC */
					{ 
						printf("\nRequest received for data transmission OFF.\n"); 

						if (udp_data_flag == 1)
						{
							/* Cancle the data sending thread if data rate is changed */
							n = pthread_cancel(udp_send_thrd_id);
							if (n == 0)
							{
								udp_data_flag = 0;
								udp_data_trans_off = 0;
								udp_send_thrd_id = 0;
								pmuse = 1;
								printf("\nData Transmission OFF.\n"); 
							}
							else
							{
								printf("\n Data sending thread is unable to kill \n"); 
							}								
						}
						else if (udp_cfg_flag == 0)
						{
							printf("\nData Transmission not started yet!\n");
						}
						else
						{
							printf("\nData Transmission is Already in OFF mode!\n"); 
						}								
					} 

					else if((c & 0x02) == 0x02)		/* Command frame for Turn ON transmission request from PDC */
					{ 				
						printf("\nRequest received for data transmission ON.\n"); 

						/* Send data frames if and Only if cfg is sent to PDC */
						if((udp_data_trans_off == 0) || (udp_cfg_flag == 1 && udp_data_flag == 0))  
						{
							if (udp_send_thrd_id == 0)
							{
								/* Create a Send data thread */
								if( (err = pthread_create(&udp_send,NULL,udp_send_data,NULL)))
								{ 
									perror(strerror(err));
									exit(1);	
								}
                                        printf("\nData Transmission ON.\n"); 
							}
							else 
							{
								printf("\nData sending already in ON mode!\n");
							} 
						}
						else if(udp_data_flag == 1)
						{
							printf("\nData sending already in ON mode!\n");
						}
						else
						{
							printf("\nData can not be send as CFG is not sent!\n");
						}
					} 

					else if((c & 0x04) == 0x04)		 /* Command frame for Configuration frame-1 request from PDC */
					{
						printf("\nConfiguration frame-1 request is Received.\n"); 
						fp1 = fopen (filename1,"rb");

						if (fp1 == NULL)
						{
							printf("\nConfiguration Frame-1 is not present in PMU Setup File.\n");
						}
						else 
						{ 
							fclose(fp1);

							/* Get the CFG size & store in global variable */
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind++];
							cfg_size = c2i(df_temp);
							cline[cfg_size] = '\0';
							if (sendto(UDP_sockfd,cline, cfg_size, 0, (struct sockaddr *)&UDP_addr,sizeof(UDP_addr)) == -1)
							{
								perror("sendto");
							}
							printf("\nThe Configuration Frame-1[%d] is sent to request PDC.\n", cfg_size);
						}
					}
				} 

				else		/* If Received Command Frame Id code not matched */
				{  
					printf("\nReceived Command Frame not from authentic PDC, ID code not matched in command frame from PDC!\n");
					continue;
				}
			} /* end of processing with received Command frame */

			else		/* If it is other than command frame */				
			{ 
				printf("\nReceived Frame is not a command frame!\n");						
				continue;				
			}
		} /* end of if-else-if */
	} /* end of while */
} /* end of pmu_udp(); */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION void* tcp_send_data(void * newfd):         					*/
/* This function run by a seprate thread only for data transmission.            */
/* Function to generate and send the data frame periodically to client's   	*/
/* destination address or to PDC client.                                        */
/* ----------------------------------------------------------------------------	*/

void* tcp_send_data(void * newfd)
{
	int new_fd = (int) newfd, i=1;

     /* Calculate the waiting time during sending data frames */
	int tcp_data_waiting = 1000000/df_data_rate;

	tcp_data_flag = 1;
	tcp_data_trans_off = 1;
	tcp_send_thrd_id = pthread_self();

	while(1)
	{
//		printf("%d. ", i); 
		if ( i == df_data_rate) i=0;

		/* Call the function generate_data_frame() to create a fresh new Data Frame */
		generate_data_frame();

          /* Send the newly created data frame to connected PDC address */
		if (send(new_fd,data_frm, df_data_frm_size, 0) == 0)
		{
			perror("send");
			tcp_data_flag = 0;
			tcp_data_trans_off = 0;
			tcp_send_thrd_id = 0;
			pthread_exit(0);
		}
		i++;
		usleep(tcp_data_waiting);			
	}
} /* end of tcp_send_data() */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION void* new_pmu_tcp(void * nfd):					               */
/* This is a TCP Server of PMU and it will continuously on listening mode.      */
/* Function for receives frames from authentic PDC & reply back the 	     	*/
/* requested frame (if available) to PDC.					               */
/* It will create separate thread only for data transmission.                   */
/* ----------------------------------------------------------------------------	*/

void* new_pmu_tcp(void * nfd)
{
	/* local variables */
	unsigned char c;
	int new_fd,n,sin_size,start_t=0,ind,id_pdc,start_u=0;
	char tcp_command[18], filename1[200];

	FILE *fp1;
	pthread_t tcp_send;

	sin_size = sizeof(struct sockaddr_in);
	new_fd = (int)nfd;

	strcpy(filename1, pmuFilePath);

     /* This will wait until CFG has not been set by user. */
	while (1)	
	{
		ind = 2;
		memset(tcp_command,36,'\0');	

          /* TCP data stream Received */
		errno = recv(new_fd,tcp_command,36,0);

		if(errno == 0)
		{
			/* Close the thread when connection will broke */
			printf("\nConnection Closed by PDC-TCP Client.\n\nPMU-TCP Server on Listening Mode!\n\n");
			pthread_cancel(tcp_send_thrd_id);
			tcp_data_flag = 0;
			tcp_data_trans_off = 0;
			tcp_send_thrd_id = 0;
			pthread_exit(0);
		}
		else		/* New dat has been received */
		{
			if (start_u == 0)
			{ 
	               /* Call the function frame_size() to initialized all globals as Configuration frame  */
				frame_size();
				start_u = 1;
			}

			c = tcp_command[1];
			c <<= 1;
			c >>= 5;

			if(c  == 0x04) 		/* Check if it is a command frame from PDC */ 
			{	
				df_temp[0] = tcp_command[4];
				df_temp[1] = tcp_command[5];
				id_pdc = c2i (df_temp);

				if(id_pdc == df_pmu_id)		/* Check if it is coming from authentic PDC/iPDC */ 
				{	
					c = tcp_command[15];
					if((c & 0x05) == 0x05)		/* Command frame for Configuration Frame-2 request from PDC */
					{ 
						printf("\nConfiguration Frame request is received.\n"); 
						fp1 = fopen (filename1,"rb");

						if (fp1 == NULL)
						{
							perror (filename1); 
							printf("\nConfiguration Frame is not present in PMU Setup File.\n");
							exit(1);
						}
						else 
						{ 
							fclose(fp1);

							/* Get the CFG size & store in global variable */
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind];
							cfg_size = c2i(df_temp);

							/* Send Configuration frame to PDC Device */
							if (send(new_fd,cline, cfg_size, 0)== -1)
							{
								perror("sendto");
							}

							tcp_cfg_flag = 1;
							cfg_change = 0;
							cfg_pmu_error = 0;
							ind = cfg_size - 4;
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind];
							df_data_rate = c2i(df_temp);

							if (start_t > 0)		
							{
								if(old_data_rate != df_data_rate)
								{
									/* Cancle the data sending thread if data rate has changed */
									n = pthread_cancel(tcp_send_thrd_id);
									if (n == 0)
									{
										if( (err = pthread_create(&tcp_send,NULL,tcp_send_data,(void *)new_fd)))
										{ 
											perror(strerror(err));
											exit(1);
										}
									}
								}
							}

							printf("\nThe Configuration frame [%d Bytes] is sent to requested PDC.\n", cfg_size);
							old_data_rate = df_data_rate;			
							start_t = 1;
						} 
					}

					else if((c & 0x03) == 0x03)		/* Command frame for Header frame request from PDC */
					{
						printf("\nHeader Frame request is received.\n"); 
						fp1 = fopen(filename1,"rb");

						if (fp1 == NULL)
						{
							printf("\nHeader Frame is not present in PMU Setup File.\n");
							exit(1);
						}
						else 
						{ 
							fclose(fp1);

							if(get_header_frame() == 1)
							{
								/* Get the CFG size & store in global variable */
								df_temp[0] = hdrline[2];
								df_temp[1] = hdrline[3];
								hdr_size = c2i(df_temp);
								hdrline[hdr_size] = '\0';

								/* Send Header frame to PDC Device */
								if (send(new_fd,hdrline, hdr_size, 0)== -1)
								{
									perror("sendto");
								}

								printf("\nThe Headed frame is sent to requested PDC.\n");
							}
						} 
					}

					else if((c & 0x01) == 0x01)		/* Command frame for Turn off transmission request from PDC */
					{
						printf("\nRequest received for data transmission OFF.\n"); 

						if (tcp_data_flag == 1)
						{
							n = pthread_cancel(tcp_send_thrd_id);	
							if (n == 0)
							{
								tcp_data_flag = 0;
								tcp_data_trans_off = 0;
								tcp_send_thrd_id = 0;
								pmuse = 1;
								printf("\nData Transmission OFF.\n"); 
							}
							else
							{
								printf("\n  Data sending thread unable to kill \n"); 
							}								
						}
						else if (tcp_cfg_flag == 0)
						{
							printf("\nData Transmission not started yet!\n");
						}
						else
						{
							printf("\nData Transmission is already in OFF mode!\n"); 
						}								
					}				  

					else if((c & 0x02) == 0x02)		/* Command frame for Turn ON transmission request from PDC */
					{ 				
						printf("\nRequest received for data transmission ON.\n"); 

						/* Send data frames if and Only if cfg is sent to PDC */
						if ((tcp_data_trans_off == 0) || (tcp_cfg_flag == 1 && tcp_data_flag == 0)) 	 
						{
							if (tcp_send_thrd_id == 0)
							{
								/* Create a Send data thread */
								if( (err = pthread_create(&tcp_send,NULL,tcp_send_data,(void *)new_fd)))
								{	 
									perror(strerror(err));
									exit(1);
								}	
                                        printf("\nData Transmission ON.\n"); 
							}
							else
							{ 
								printf("\nData sending already in ON mode!\n");
							} 
						}
						else if(tcp_data_flag == 1)
						{
							printf("\nData sending already in ON mode!\n");
						}
						else
						{
							printf("\nData can not be send as CFG is not sent!\n");
						}
					} 

					else if((c & 0x04) == 0x04)		/* Command frame for Configuration frame-1 request from PDC */
					{
						printf("\nConfiguration frame-1 request is Received.\n"); 
						fp1 = fopen (filename1,"rb");
						if (fp1 == NULL)
						{
							printf("\nConfiguration Frame-1 is not present in PMU Setup File.\n");
						}
						else 
						{ 
							fclose(fp1);

							/* Get the CFG size & store in global variable */
							df_temp[0] = cline[ind++];
							df_temp[1] = cline[ind++];
							cfg_size = c2i(df_temp);

							if (send(new_fd,cline, cfg_size, 0)== -1)
							{
								perror("sendto");
							}

							printf("\nThe Configuration Frame-1[%d Bytes] is sent to request PDC.\n", cfg_size);
						} 
					}
				}

				else		/* If Received Command Frame Id code not matched */		
				{  
					printf("\nReceived Command Frame not from authentic PDC, ID code not matched in command frame from PDC!\n");
					continue;
				}
			} /* end of processing with received Command frame */

			else		/* If it is other than command frame */				
			{ 
				printf("\nReceived Frame is not a command frame!\n");						
				continue;				
			}
		} /* end of if-else-if */
	} /*end of While */

	pthread_exit(0);
} /* end of void* pmu_tcp(); */


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  pmu_tcp():			                    					*/
/* Function for accept the TCP connection and create a new thread to run        */
/* PMU TCP Server. And add the client as requested PDC.                         */
/* ----------------------------------------------------------------------------	*/

void* pmu_tcp()
{
	/* local variables */
	int new_tcp_fd, sin_size, err;
	pthread_t new_pmu;
	sin_size = sizeof(struct sockaddr_in);

	/* A new thread is created for TCP connection in 'detached' mode. */ 
	pthread_attr_t attr;
	pthread_attr_init(&attr);

     /* In  the detached state, the thread resources are immediately freed when it terminates, but */
     /* pthread_join cannot be used to synchronize on the thread termination. */
	if( (err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)))	
	{ 										
		perror(strerror(err));
		exit(1);							
	}																																		    
     /* Shed policy = SCHED_FIFO (realtime, first-in first-out) */
	if( (err = pthread_attr_setschedpolicy(&attr,SCHED_FIFO)))	
	{ 
		perror(strerror(err));		     
		exit(1);
	}  

	while(1)
	{
          int i = 1;
	     /* New TCP connection has been received*/ 
		if (((new_tcp_fd = accept(TCP_sockfd, (struct sockaddr *)&TCP_addr, (socklen_t *)&sin_size)) == -1))
		{ 
			perror("accept");
		}
		else	 
		{		
			if( (err = pthread_create(&new_pmu,&attr,new_pmu_tcp,(void *)new_tcp_fd)))
			{ 
				perror(strerror(err));
				exit(1);
			}	
		}
          printf("My I == %d\n", i);
          i++;
	}

	close(new_tcp_fd);
}


/* ----------------------------------------------------------------------------	*/
/* FUNCTION  start_server():							                    */
/* Function for Start PMU Server as per user given Port number and Protocol.    */
/* ----------------------------------------------------------------------------	*/

void start_server()
{
	/* Initialy create the shared memory ID */
	int ShmID;
     char *ptr1;

     cfg_pmu_trigger = 0;
     dataFileVar = 0; 
     cfg_change = 0;
     cfg_invalid_data = 0; 
     cfg_pmu_error = 0;
     cfg_sorting_data = 0; 
     cfg_crc_error = 0;

	p1.pid = getpid();
	key_t MyKey;

	if (signal(SIGUSR1, SIGUSR1_handler) == SIG_ERR) 
	{
		printf("SIGUSR-1 install error\n");
		exit(1);
	}

	if (signal(SIGUSR2, SIGUSR2_handler) == SIG_ERR) 
	{
		printf("SIGUSR-2 install error\n");
		exit(1);
	}

	MyKey   = 12345;                     /* obtain the shared memory */
	ShmID   = shmget(MyKey, sizeof(struct P_id), IPC_CREAT | 0666);
	ShmPTR  = (struct P_id *) shmat(ShmID, NULL, 0);
	*ShmPTR = p1;                /* save my pid there             */

     /* This will wait until Port and Protocol have not been set by user.  */
	while(tmp_wait)
	{
		usleep(1000);
	}

	fp_DataFile = NULL;

     /* Get the user's name for storing the PMU Setup File */
	ptr1 = getenv ("HOME");

	if (ptr1 == NULL)
	{
		printf("user not found\n");
		exit(1);
	}
	strcat(pmuFolderPath, ptr1);
	strcat(pmuFolderPath, "/iPDC/PMU");

	if(protocol_choice == 1)
	{     
		printf("\n\t\t|-------------------------------------------------------|\n");      
		printf("\t\t|\t\tWelcome to PMU SERVER - UDP\t\t|\n");      
		printf("\t\t|-------------------------------------------------------|\n");      

		if ((UDP_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)	/* Create UDP socket */
		{
			perror("socket");
			exit(1);
		}
		else
		{
			printf("\nUDP Socket : Sucessfully created\n");
		} 	

		if (setsockopt(UDP_sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)	/* UDP socket */
		{
			perror("setsockopt");
			exit(1);
		}

		UDP_my_addr.sin_family = AF_INET;         		   	/* host byte order */
		UDP_my_addr.sin_port = htons(PORT);       			/* short, network byte order */
		UDP_my_addr.sin_addr.s_addr = INADDR_ANY;			/* inet_addr("127.0.0.1")/ INADDR_ANY-automatically fill with my IP */
		memset(&(UDP_my_addr.sin_zero),'\0', 8);     		/* zero the rest of the struct */

		if (bind(UDP_sockfd, (struct sockaddr *)&UDP_my_addr, sizeof(struct sockaddr)) == -1)	/* bind UDP socket to port */
		{
			perror("bind");
			exit(1);
		}
		else
		{
			printf("\nUDP Socket Bind : Sucessfull\n");
		}  

		printf("\nPMU UDP SERVER Listening on port: %d\n",PORT);
		UDP_addr_len = sizeof(struct sockaddr);

		/* Create Thread for UDP on given port default mode */
		if( (err = pthread_create(&UDP_thread,NULL,pmu_udp,NULL)))
		{ 
			perror(strerror(err));
			exit(1);	
		}
		pthread_join(UDP_thread, NULL);
		close(UDP_sockfd);
	} /* end of UDP protocol */

	else 
	{ 
		printf("\n\t\t|-------------------------------------------------------|\n");      
		printf("\t\t|\t\tWelcome to PMU SERVER - TCP\t\t|\n");      
		printf("\t\t|-------------------------------------------------------|\n");      

		if ((TCP_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	/* Create TCP socket */
		{
			perror("socket");
			exit(1);
		}
		else
		{
			printf("\nTCP Socket : Sucessfully created\n");
		}

		if (setsockopt(TCP_sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)	/* TCP socket */
		{
			perror("setsockopt");
			exit(1);
		}

		TCP_my_addr.sin_family = AF_INET;         				 /* host byte order */
		TCP_my_addr.sin_port = htons(PORT);    				         /* short, network byte order */
		TCP_my_addr.sin_addr.s_addr = INADDR_ANY;				 /* ("127.0.0.1")/ INADDR_ANY-automatically fill with my IP */
		memset(&(TCP_my_addr.sin_zero), '\0', 8);  				 /* zero the rest of the struct */

		if (bind(TCP_sockfd, (struct sockaddr *)&TCP_my_addr, sizeof(struct sockaddr)) == -1)	/* bind TCP socket to port */
		{
			perror("bind");
			exit(1);
		}
		else
		{
			printf("\nTCP Socket Bind : Sucessfull\n");
		}

		if (listen(TCP_sockfd, BACKLOG) == -1)		/* Listen on port on TCP socket */
		{
			perror("listen");
			exit(1);
		}

		/* Signal handling for TCP Connection */
		sa.sa_handler = sigchld_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1)
		{
			perror("sigaction");
			exit(1);
		}

		printf("\nPMU TCP SERVER Listening on port: %d\n",PORT);

		/* Create Thread for TCP on given port default mode */
		if( (err = pthread_create(&TCP_thread,NULL,pmu_tcp,NULL)))
		{
			perror(strerror(err));
			exit(1);	
		}
		pthread_join(TCP_thread, NULL);
		close(TCP_sockfd);
	}
} /* end of start_server() */


/* ------------------------------------------------------------------ */
/* FUNCTION  SIGUSR1_handler(int sig):                                */
/* SIGUSR1 signal handler will give the user's choice regarding data  */
/* data source. Give also the file path when data measurements from   */
/* file.                                                              */
/* ------------------------------------------------------------------ */

void  SIGUSR1_handler(int sig)
{
	signal(sig, SIG_IGN);
	printf("SIGUSR-1 signal received\n");

	if(ShmPTR->dataFileVar == 1)
	{
		fp_DataFile = fopen (ShmPTR->filePath, "r");

		if (fp_DataFile == NULL)
		{
			perror (ShmPTR->filePath);
		}
		else
		{
			dataFileVar = ShmPTR->dataFileVar;
		}
	}
	else if(ShmPTR->dataFileVar == 0)
	{
		dataFileVar = ShmPTR->dataFileVar;

		if(fp_DataFile != NULL)			
			fclose(fp_DataFile);
	}
	else if(ShmPTR->dataFileVar == 2)
	{
		strcpy(pmuFilePath, ShmPTR->cfgPath);
	}

	signal(sig, SIGUSR1_handler);
}


/* ------------------------------------------------------------------ */
/* FUNCTION  SIGUSR2_handler(int sig):                                */
/* SIGUSR2 signal handler will give the user's choice regarding data  */
/* STAT Word change. Gives what kind of error has been introduced and */
/* which bit should be change from 0 to 1 in data frame's STAT Word.  */
/* ------------------------------------------------------------------ */

void  SIGUSR2_handler(int sig)
{
	signal(sig, SIG_IGN);
	printf("SIGUSR-2 signal received\n");

	if(ShmPTR->cfg_bit_change_info == 0)	
	{
		PORT = ShmPTR->serverPort;
		protocol_choice = ShmPTR->serverProtocol;
		tmp_wait = 0;
	}

	else if(ShmPTR->cfg_bit_change_info == 1)	/* for configuration change bit */
	{
		cfg_change = 1; 
		printf("SIGUSR2 signal received for CFG changed\n");
	}

	else if(ShmPTR->cfg_bit_change_info == 2)	/* for invalid data bit */
	{
		cfg_invalid_data = 1; 
		printf("SIGUSR2 signal received for invalid data.\n");

	}
	else if(ShmPTR->cfg_bit_change_info == 3)	/* for PMU error bit */
	{
		cfg_pmu_error = 1; 
		printf("SIGUSR2 signal received for PMU error.\n");
	}
	else if(ShmPTR->cfg_bit_change_info == 4)	/* for data sorting bit */
	{
		cfg_sorting_data = 1; 
		printf("SIGUSR2 signal received for Data Sorting errror.\n");
	}
	else if(ShmPTR->cfg_bit_change_info == 5)	/* for PMU trigger bit */
	{
		cfg_pmu_trigger = 1; 
		printf("SIGUSR2 signal received for PMU Trigger.\n");
	}
	else if(ShmPTR->cfg_bit_change_info == 6)	/* for PMU trigger bit */
	{
		cfg_crc_error = 1; 
		printf("SIGUSR2 signal received for CheckSum Error.\n");
	}

	signal(sig, SIGUSR2_handler);
}

/**************************************** End of File *******************************************************/
