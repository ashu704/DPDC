/* ----------------------------------------------------------------------------- 
 * function.c
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

/* Function copies Bytes in a main array */ 
void B_copy(unsigned char main[], unsigned char tmp[], int ind, int n);

/* Function to read the measurement file and return measurements for a singel timestamp */ 
char* measurement_Return ();

/* Function for Hexa to String Conversion */ 
void H2S(char a[], unsigned char temp_6[]);

/* Function for Integer to Character Conversion */ 
void i2c (int t, unsigned char temp[]);

/* Function for Long Integer to Character Conversion */ 
void li2c (long int t1, unsigned char temp_1[]);

/* Function for float to Character Conversion */ 
void f2c (float f, unsigned char temp_4[]);

/* Function for Character to Integer Conversion */ 
int c2i (unsigned char temp_2[]);

/* Function for Character to Long Integer Conversion */ 
long int c2li (unsigned char temp_3[]);

/* Function for calculation of CHECKSUM CRC-CCITT (0xffff) */
uint16_t compute_CRC(unsigned char *message,int length);

/* Function for TCP connection signal handling */ 
void sigchld_handler(int s);

/**************************************** End of File *******************************************************/
