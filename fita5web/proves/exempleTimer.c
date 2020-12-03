/*
 * (c) EUSS 2013
 *
 * 
 * Exemple d'utilització dels timers de la biblioteca librt
 * Crea dos timers que es disparen cada segon de forma alternada
 * Cada cop que es disparen imprimeixen per pantalla un missatge
 * 
 * Per compilar: gcc -Wall exempleTimer.c -lrt -lpthread -o timer
 */
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


#define DEV_ID 0x48
#define DEV_PATH "/dev/i2c-1"
#define CNF_REG 0x01
#define CNV_REG 0x00

#define SHTDWN_REG 0x00
#define CTRL_REG1 0x13
#define CTRL_REG2 0x14
#define CTRL_REG3 0x15
#define PWM_UPDATE_REG 0x16
#define RST_REG 0x17
  #define DEV_ID_I2C 0x54

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
    int fd = 0;
    uint8_t adc_l=0, adc_h=0;
    uint16_t adc_out = 0;
    int adc_ok=0;
    double adc_v = 0.0;
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
	
    //--------------------------------------------------------------------------------------------------
}

int main(int argc, char ** argv)
{

    timer_t tick;
    set_timer(&tick, 1, 1, callbacksensor, (void *) "tick" );
    getchar();

    return 0;
}
