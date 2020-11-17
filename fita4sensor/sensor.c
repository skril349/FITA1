/**
 * @brief Programa de lectura d'una base de dades
 * 
 * Programa que llegeix la informació d'un sensor de temperatura i la publica
 * en la base de dades.
 * 
 * @code
 * ~$ gcc informe.c -o informe
 * ~$ ./informe
 * @endcode
 * 
 * El programa penja a la base de dades els valors obtinguts del sensor de
 * temperatura.
 * 
 * @file sensor.c
 * @author Toni Vives Cabaleiro
 * @version Fita4
 * @date 03/11/2020
 * 
 * @param - No n'hi ha
 * @return 0 Si està ok
 * 
 * @todo Pròximes fites
 * 
 * @section LICENSE
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.         
 */
 
  //gcc informe_txt.c -lsqlite3 -o informe
 //./informe



#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h> 
#include <sqlite3.h>
#include <string.h>
 
#define DEV_ID 0x48
#define DEV_PATH "/dev/i2c-1"
#define CNF_REG 0x01
#define CNV_REG 0x00

//------------------------------------------------------------------------------------------------------------------------------
/*
#define DEV_PATH "/dev/i2c-1"
*/
#define SHTDWN_REG 0x00
#define CTRL_REG1 0x13
#define CTRL_REG2 0x14
#define CTRL_REG3 0x15
#define PWM_UPDATE_REG 0x16
#define RST_REG 0x17
  #define DEV_ID_I2C 0x54



static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i]: "NULL");
	}
	printf("\n");
	return(0);
}


void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
}



int main(void) {
    
    sqlite3 *db;
    char *err_msg = 0;
    int fd = 0;
    uint8_t adc_l=0, adc_h=0;
    uint16_t adc_out = 0;
    int adc_ok=0;
    double adc_v = 0.0;
    int counter = 0;
    int n=0;
    int fd1=0;
    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
	perror("Unable to open i2c device");
	exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, DEV_ID) < 0) {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }
    
    if ((fd1 = open(DEV_PATH, O_RDWR)) < 0) {
	perror("Unable to open i2c device");
	exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd1, I2C_SLAVE, DEV_ID_I2C) < 0) {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }
 //--------------------------------------------------------------------------------------------------------------------------
  
//------------------------------------------------------------------------------------------------------------------------
	
while(n<=10){
	
	   /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd1, RST_REG, 0x00);

    /* Turn on the device in normal operation  */
    i2c_smbus_write_byte_data(fd1, SHTDWN_REG, 0x01);

    /* Activate LEDs 1-3 */
  i2c_smbus_write_byte_data(fd1, CTRL_REG1, 0x07);

    /* SET the PWM value for LEDs 1 to 3 */
   i2c_smbus_write_byte_data(fd1, 0x01, 0xff); // LED1 -> 015/255
   i2c_smbus_write_byte_data(fd1, 0x02, 0xff); // LED2 -> 002/255
   i2c_smbus_write_byte_data(fd1, 0x03, 0xff); // LED3 -> 128/255
	/* Values stored in a temporary register */
	
	/* Update values of registers*/
	i2c_smbus_write_byte_data(fd1, PWM_UPDATE_REG, 0x00); //write any value
	
	
    /* Run one-shot measurement (AIN1-gnd), FSR +-4.096V, 160 SPS, 
     * no windows, no comparation */
    // LowSByte MSByte  they are inverted
    i2c_smbus_write_word_data(fd, CNF_REG, 0x83D3);
	//printf("memoria=%d",0x83D3);
    /* Wait until the measurement is complete */
	usleep(700);		/* 700 microseconds */
	adc_out = i2c_smbus_read_word_data(fd, CNV_REG);
	// swap bytes
	adc_l=adc_out >> 8;
	adc_h=adc_out;
	adc_ok=(adc_h<<8)|adc_l;
	// dichart 4 LSB
	adc_ok=(adc_ok>>4);
	
    printf("Value ADC = %d \n",adc_out);
    printf("Value ADC = %x \n",adc_out);
    printf("Value L   = %x \n",adc_l);
    printf("Value H   = %x \n",adc_h);
    printf("Value OK  = %x \n",adc_ok);
   
    /* calculate output values */
    adc_v = 4.095 * (adc_ok / 2047.0);

    /*output */
    printf("Value ADC in V = %.2fV\n", adc_v);
    printf("Value input in V = %.2fV\n", adc_v*47/6);
    printf("Value degrees(ºC) = %.2fºC\n", adc_v*4700/6);
	
	i2c_smbus_write_byte_data(fd1, RST_REG, 0x00);
    i2c_smbus_write_byte_data(fd1, SHTDWN_REG, 0x00);
    delay(1000);	

    int rc = sqlite3_open("temperatures.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
   char sql [2056] = "DROP TABLE IF EXISTS Lectures;" 
                "CREATE TABLE Lectures(Id INT, Sensor TEXT, Temperatura FLOAT, Temps TEXT);" ;
                //"CREATE TABLE Lectures(Id INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Temperatura FLOAT, Temps TIMESTAMP DEFAULT CURRENT_TIMESTAMP);" ;        
     char texto [2056];
     snprintf(texto, sizeof(texto), "INSERT INTO Lectures(id,Sensor,Temperatura, Temps) VALUES(%d, 'sensor temperatura', %.2f, DateTime('now'));",counter, adc_v*4700/6);
     
     counter++;
     rc = sqlite3_exec(db, texto, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    n++;
    
    sqlite3_close(db);
    
      

   } 
 //----------------------------------------------------------------------------------------------------------------------------  
   
    return 0;
}
