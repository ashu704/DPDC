/* ----------------------------------------------------------------------------- 
 * db.c & DPDC_DB_Queries
 * 
 *
 * ----------------------------------------------------------------------------- */


/**************************************** Begin of db.c *******************************************************/

#include <mysql.h>
#include <stdio.h>

main() {	/* Simple C program that connects to MySQL Database server*/

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *server = "localhost";
	char *user = "root";
	char *password = "root"; /* set me first */
	char *database = "test";

	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
			user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	char cmd[40];
	int i=2;
	sprintf(cmd, "select * from student");

	/* send SQL query */
	if (mysql_query(conn, cmd)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* output table name */
	printf("Data in student:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);
	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
}


/**************************************** Begins of DPDC_DB_Queries *******************************************************/

/*
 
 ANALOG                 
 ANALOG_MEASUREMENTS     
 DIGITAL                 
 DIGITAL_MEASUREMENTS    
 FREQUENCY_MEASUREMENTS  
 MAIN_CFG_TABLE          
 PHASOR                  
 PHASOR_MEASUREMENTS
 REVEIVED_FRAME_TIME     
 SUB_CFG_TABLE  


----CFG

SELECT * FROM MAIN_CFG_TABLE;
SELECT * FROM SUB_CFG_TABLE;
SELECT * FROM PHASOR;
SELECT * FROM ANALOG;
SELECT * FROM DIGITAL;

SELECT * from PHASOR_MEASUREMENTS WHERE SOC = (SELECT MAX(SOC) FROM PHASOR_MEASUREMENTS);

delete from  MAIN_CFG_TABLE;          
delete from  SUB_CFG_TABLE;
delete from  PHASOR;
delete from  ANALOG;
delete from  DIGITAL;
delete from  PHASOR_MEASUREMENTS;
delete from  FREQUENCY_MEASUREMENTS;
delete from  ANALOG_MEASUREMENTS;
delete from  DIGITAL_MEASUREMENTS;
delete from  REVEIVED_FRAME_TIME;

----DATA

SELECT * FROM  PHASOR_MEASUREMENTS ;
SELECT * FROM  FREQUENCY_MEASUREMENTS;
SELECT * FROM  ANALOG_MEASUREMENTS;
SELECT * FROM  DIGITAL_MEASUREMENTS;
SELECT * FROM  REVEIVED_FRAME_TIME;

SELECT COUNT(*) FROM  PHASOR_MEASUREMENTS ;
SELECT COUNT(*) FROM  FREQUENCY_MEASUREMENTS;
SELECT COUNT(*) FROM  ANALOG_MEASUREMENTS;
SELECT COUNT(*) FROM  DIGITAL_MEASUREMENTS;

// To verify the number of frames received per second from PMU Simulator.
SELECT COUNT(*) from PHASOR_MEASUREMENTS WHERE SOC = (SELECT MAX(SOC)-n FROM PHASOR_MEASUREMENTS);
SELECT COUNT(*) from ANALOG_MEASUREMENTS WHERE SOC = (SELECT MAX(SOC)-n FROM ANALOG_MEASUREMENTS);

----DROP TABLE

drop table MAIN_CFG_TABLE;
drop table SUB_CFG_TABLE;
drop table PHASOR;
drop table ANALOG;
drop table DIGITAL;
drop table PHASOR_MEASUREMENTS;
drop table FREQUENCY_MEASUREMENTS;
drop table ANALOG_MEASUREMENTS;
drop table DIGITAL_MEASUREMENTS;
drop table REVEIVED_FRAME_TIME;
*/

/**************************************** End of File *******************************************************/
