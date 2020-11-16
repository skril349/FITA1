/*
 * ADC MC3008 example
 *
 * Cross-compile with   arm-linux-gnueabi-gcc nom.c -o nom_executable
 *
 * Copyrigth (C) EUSS 2018  ( http://www.euss.cat )
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <time.h> 
#include <sqlite3.h>

int verbose = 1;

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

// -----------------------------------------------------------------------------------------------

static void pabort(const char *s)
{
	perror(s);
	abort();
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

// -----------------------------------------------------------------------------------------------


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




int main(int argc, char *argv[])
{
	sqlite3 *db;
    char *err_msg = 0;
	int ret = 0, value_int;
	int counter = 0;
    int n=0;
	float value_volts;
	
	
	while(1){
	ret = spiadc_config_transfer( SINGLE_ENDED_CH0, &value_int );

	printf("valor llegit (0-1023) %d\n", value_int);
	value_volts=3.3*value_int/1023;
	
	printf("voltatge %.3f V\n", value_volts);
	delay(1000);
//--------------------------------------------------------------------------------------------------------------------	
	  int rc = sqlite3_open("temperatures.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
   char sql [2056] = "DROP TABLE IF EXISTS Voltatges;" 
                "CREATE TABLE Voltatges(Id INT, Sensor TEXT, Voltatge FLOAT, Temps TEXT);" ;
                //"CREATE TABLE Lectures(Id INTEGER PRIMARY KEY AUTOINCREMENT, Nom TEXT, Temperatura FLOAT, Temps TIMESTAMP DEFAULT CURRENT_TIMESTAMP);" ;        
     char texto [2056];
     snprintf(texto, sizeof(texto), "INSERT INTO Voltatges(id,Sensor,Voltatge, Temps) VALUES(%d, 'Lectura sensor', %.2f, DateTime('now'));",counter, value_volts);
     
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
	return ret;
}
