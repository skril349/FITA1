


	






#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <time.h> 
#include <sqlite3.h>
#include <string.h>

#include "func.h"

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



typedef void (timer_callback) (union sigval);

/* Funció set_timer
 * 
 * Crear un timer
 * 
 * Paràmetres:
 * timer_id: punter a una estructura de tipus timer_t
 * delay: retard disparament timer (segons)
 * interval: periode disparament timer  (segons)
 * func: funció que s'executarà al disparar el timer
 * data: informació que es passarà a la funció func
 * 
 * */
int set_timer(timer_t * timer_id, float delay, float interval, timer_callback * func, void * data) 
{
    int status =0;
    struct itimerspec ts;
    struct sigevent se;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = data;
    se.sigev_notify_function = func;
    se.sigev_notify_attributes = NULL;

    status = timer_create(CLOCK_REALTIME, &se, timer_id);

    ts.it_value.tv_sec = abs(delay);
    ts.it_value.tv_nsec = (delay-abs(delay)) * 1e09;
    ts.it_interval.tv_sec = abs(interval);
    ts.it_interval.tv_nsec = (interval-abs(interval)) * 1e09;

    status = timer_settime(*timer_id, 0, &ts, 0);
    return 0;
}

void callbacksensor(union sigval si)
{
    char * msg = (char *) si.sival_ptr;
    
    printf("%s\n",msg);
    
    //------------------------------------------------------------------------------------------------
    char nom_servidor[32] = "192.168.11.185";
	char cadena_URI[32] = "iotlab.euss.es";
	char resposta_header[4256];
	char resposta_data[5256];
	char missatge_dades[5256];
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
		
	   /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd1, RST_REG, 0x00);

    /* Turn on the device in normal operation  */
    i2c_smbus_write_byte_data(fd1, SHTDWN_REG, 0x01);

    /* Activate LEDs 1-3 */
  i2c_smbus_write_byte_data(fd1, CTRL_REG1, 0x07);

   i2c_smbus_write_byte_data(fd1, 0x01, 0xff); // LED1 -> 015/255
   i2c_smbus_write_byte_data(fd1, 0x02, 0xff); // LED2 -> 002/255
   i2c_smbus_write_byte_data(fd1, 0x03, 0xff); // LED3 -> 128/255
	
	i2c_smbus_write_byte_data(fd1, PWM_UPDATE_REG, 0x00); 
	
    i2c_smbus_write_word_data(fd, CNF_REG, 0x83D3);

	usleep(700);		
	adc_out = i2c_smbus_read_word_data(fd, CNV_REG);
	adc_l=adc_out >> 8;
	adc_h=adc_out;
	adc_ok=(adc_h<<8)|adc_l;
	adc_ok=(adc_ok>>4);

    adc_v = 4.095 * (adc_ok / 2047.0);	
	i2c_smbus_write_byte_data(fd1, RST_REG, 0x00);
    i2c_smbus_write_byte_data(fd1, SHTDWN_REG, 0x00);
	printf("Value ADC in V = %.2fV\n", adc_v);
    printf("Value input in V = %.2fV\n", adc_v*47/6);
    printf("Value degrees(ºC) = %.2fºC\n", adc_v*4700/6);
    sprintf(missatge_dades,"%.2f",adc_v*4700/6);
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
    //missatge_dades="/cloud/guardar_dades.php?id_sensor=1&valor=1234567& HTTP/1.1\r\nHost: iotlab.euss.es\r\n\r\n";
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data,missatge_dades );

    //--------------------------------------------------------------------------------------------------
}




int main(void) {
   
 //----------------------------------------------------------------------------------------------------------------------------  
   
    timer_t tick;
    set_timer(&tick, 1, 3, callbacksensor, (void *) "tick" );
    getchar();
    return 0;
}
