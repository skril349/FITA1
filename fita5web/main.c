
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <time.h> 
#include <sqlite3.h>
#include <string.h>

#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/types.h>

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
//SPI
int verbose = 1;

//Variables Globals
char basedades[REPLY_MSG_SIZE] = "/home/pi/Desktop/FITA1/fita5web/temperatures.db"; //Nom de la base de dades
char nom_servidor[REPLY_MSG_SIZE] = "192.168.11.185"; //Nom del servidor


static char *cntdevice = "/dev/spidev0.0";

//ADC configurations segons manual MCP3008
#define SINGLE_ENDED_CH0 8
#define SINGLE_ENDED_CH1 9
#define SINGLE_ENDED_CH2 10
#define SINGLE_ENDED_CH3 11
#define SINGLE_ENDED_CH4 12
#define SINGLE_ENDED_CH5 13
#define SINGLE_ENDED_CH6 14
#define SINGLE_ENDED_CH7 15
#define DIFERENTIAL_CH0_CH1 0 //Chanel CH0 = IN+ CH1 = IN-
#define DIFERENTIAL_CH1_CH0 1 //Chanel CH0 = IN- CH1 = IN+
#define DIFERENTIAL_CH2_CH3 2 //Chanel CH2 = IN+ CH3 = IN-
#define DIFERENTIAL_CH3_CH2 3 //Chanel CH2 = IN- CH3 = IN+
#define DIFERENTIAL_CH4_CH5 4 //Chanel CH4 = IN+ CH5 = IN-
#define DIFERENTIAL_CH5_CH4 5 //Chanel CH4 = IN- CH5 = IN+
#define DIFERENTIAL_CH6_CH7 6 //Chanel CH6 = IN+ CH7 = IN-
#define DIFERENTIAL_CH7_CH6 7 //Chanel CH6 = IN- CH7 = IN+



//--------------------------------------------------------------------------------------------------------------------

static void pabort(const char *s)
{
	perror(s);
	abort();
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

// -----------------------------------------------------------------------------------------------

static void spiadc_config_tx( int conf, uint8_t tx[3] )
{
	int i;

	uint8_t tx_dac[3] = { 0x01, 0x00, 0x00 };
	uint8_t n_tx_dac = 3;
	
	for (i=0; i < n_tx_dac; i++) {
		tx[i] = tx_dac[i];
	}
	
// Estableix el mode de comunicació en la parta alta del 2n byte
	tx[1]=conf<<4;
	
	if( verbose ) {
		for (i=0; i < n_tx_dac; i++) {
			printf("spi tx dac byte:(%02d)=0x%02x\n",i,tx[i] );
		}
	}
		
}

// -----------------------------------------------------------------------------------------------
static int spiadc_transfer(int fd, uint8_t bits, uint32_t speed, uint16_t delay, uint8_t tx[3], uint8_t *rx, int len )
{
	int ret, value, i;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len*sizeof(uint8_t),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	if( verbose ) {

		for (i = 0; i < len; i++) {
			printf("0x%02x ", rx[i]);
		}
		value = ((rx[1] & 0x0F) << 8) + rx[2];
		printf("-->  %d\n", value);
	
	}

	return ret;

}



// -----------------------------------------------------------------------------------------------

static int spiadc_config_transfer( int conf, int *value )
{
	int ret = 0;
	int fd;
	uint8_t rx[3];
	char buffer[255];
	
	/* SPI parameters */
	char *device = cntdevice;
	//uint8_t mode = SPI_CPOL; //No va bé amb aquesta configuació, ha de ser CPHA
	uint8_t mode = SPI_CPHA;
	uint8_t bits = 8;
	uint32_t speed = 500000; //max 1500KHz
	uint16_t delay = 0;
	
	/* Transmission buffer */
	uint8_t tx[3];

	/* open device */
	fd = open(device, O_RDWR);
	if (fd < 0) {
		sprintf( buffer, "can't open device (%s)", device );
		pabort( buffer );
	}

	/* spi mode 	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/* bits per word 	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/* max speed hz  */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	/* build data to transfer */
	spiadc_config_tx( conf, tx );
		
	/* spi adc transfer */
	ret = spiadc_transfer( fd, bits, speed, delay, tx, rx, 3 );
	if (ret == 1)
		pabort("can't send spi message");

	close(fd);

	*value = ((rx[1] & 0x03) << 8) + rx[2];

	return ret;
}

//--------------------------------------------------------------------------------------------------------------------
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
//    char nom_servidor[32] = "192.168.11.185";
	char cadena_URI[32] = "iotlab.euss.es";
	char resposta_header[4256];
	char resposta_data[5256];
	char missatge_dades[5256];
	char valorSensor[5256];
	char idSensor[5256]="81";
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
   // sprintf(missatge_dades,"%.2f",adc_v*4700/6);
   // sprintf(cadenaSensor,"/cloud/guardar_dades.php?id_sensor=%s&valor=%s&",idSensor,missatge_dades);
    
    
    sprintf(valorSensor,"%.2f",adc_v*4700/6);
    sprintf(missatge_dades,"/cloud/guardar_dades.php?id_sensor=%s&valor=%s&",idSensor,valorSensor);
    
	  int rc = sqlite3_open(basedades, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        //return 1;
    }
   char sql [2056] = "DROP TABLE IF EXISTS Lectures;" 
                "CREATE TABLE Lectures(Id INT, Sensor TEXT, Temperatura FLOAT, Temps TEXT);" ;
                //"CREATE TABLE Lectures(Id INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Temperatura FLOAT, Temps TIMESTAMP DEFAULT CURRENT_TIMESTAMP);" ;        
     char texto [2056];
     snprintf(texto, sizeof(texto), "INSERT INTO Lectures(id,Sensor,Temperatura, Temps) VALUES(%s, 'sensor temperatura', %.2f, DateTime('now'));",idSensor, adc_v*4700/6);
     
     counter++;
     rc = sqlite3_exec(db, texto, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        //return 1;
    } 
    n++;
        
    sqlite3_close(db);
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data, missatge_dades );

    //--------------------------------------------------------------------------------------------------
}

void callbacksensorSPI(union sigval si)
{
	
//	char nom_servidor[32] = "192.168.11.185";
	char cadena_URI[32] = "iotlab.euss.es";
	char resposta_header[4256];
	char resposta_data[5256];
	char missatge_dades[5256];
	char valorSensor[5256];
	char idSensor[32]="82";
	int ret = 0, value_int;
    sqlite3 *db;
    char *err_msg = 0;
    int fd = 0;
    int fd1=0;
	
	
	
	
	
	ret = spiadc_config_transfer( SINGLE_ENDED_CH0, &value_int );

	printf("valor llegit (0-1023) %d\n", value_int);
	//value_volts=3.3*value_int/1023;
	
	printf("voltatge %.3f V\n", 3.3*value_int/1023);
	
	 
    
    sprintf(valorSensor,"%.2f",3.3*value_int/1023);
    sprintf(missatge_dades,"/cloud/guardar_dades.php?id_sensor=%s&valor=%s&",idSensor,valorSensor);
    
	  int rc = sqlite3_open(basedades, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        //return 1;
    }
   char sql [2056] = "DROP TABLE IF EXISTS Lectures;" 
                "CREATE TABLE Lectures(Id INT, Sensor TEXT, Temperatura FLOAT, Temps TEXT);" ;
                //"CREATE TABLE Lectures(Id INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Temperatura FLOAT, Temps TIMESTAMP DEFAULT CURRENT_TIMESTAMP);" ;        
     char texto [2056];
     snprintf(texto, sizeof(texto), "INSERT INTO Lectures(id,Sensor,Temperatura, Temps) VALUES(%s, 'sensor voltatge', %.2f, DateTime('now'));",idSensor, 3.3*value_int/1023);
     
     rc = sqlite3_exec(db, texto, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        //return 1;
    } 
    
        
    sqlite3_close(db);
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data, missatge_dades );

	
	return ret;

}



int main(int argc, char *argv[]) {
   
 //----------------------------------------------------------------------------------------------------------------------------  

	
	int opt= 0;
    static struct option long_options[] = 
    {
        {"servidor", required_argument, 0, 's'},

        {"basedades", required_argument, 0, 'b'},
        {0, 0, 0, 0}
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"s:b:", long_options, &long_index )) != -1) 
    {
        switch (opt) 
        {

             case 's' : strcpy(nom_servidor, optarg); 
                 break;
             case 'b' : strcpy(basedades, optarg);
                 break;
             default: 	error(); 
                 exit(EXIT_FAILURE);
        }
    }
    
    printf(" ./main -s 192.168.11.185 -b /home/pi/Desktop/FITA1/fita5web/temperatures.db\n");

    timer_t tick;
	timer_t tock;

    set_timer(&tick, 1, 2, callbacksensor, (void *) "tick" );
    set_timer(&tock, 1, 3, callbacksensorSPI, (void *) "tock" );

    getchar();
    return 0;
}
