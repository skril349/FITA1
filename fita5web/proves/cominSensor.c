
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		80


int http_get( char *nom_servidor, char *cadena_URI, char *resposta_header, char *resposta_data );


int main(int argc, char *argv[])
{
	char nom_servidor[32] = "192.168.11.185";
	char cadena_URI[32] = "iotlab.euss.es";
	char resposta_header[4256];
	char resposta_data[5256];
	
		
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data );
}
    
int http_get( char *nom_servidor, char *cadena_URI, char *resposta_header, char *resposta_data )
{
	struct sockaddr_in	serverAddr;
	char		serverName[32]="192.168.11.185";					//Adreça IP on està el client
	int			sockAddrSize;
	int			sFd;
	int 		result;
	char		buffer[4256];
	char		missatge[REQUEST_MSG_SIZE]="GET /cloud/guardar_dades.php?id_sensor=1&valor=1234567&temps=2020-11-27+12%3A34%3A56 HTTP/1.1\r\nHost: iotlab.euss.es\r\n\r\n";
	;
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	printf("%s", asctime(localtime(&t)));
	

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); 	//Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(serverName);

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",	inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	/*Enviar*/
	strcpy(buffer,missatge); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge);	//Imprimim per pantalla el que hem enviat al servidor.
       
	/*Rebre*/
	result = read(sFd, buffer, 4256);
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);	//Imprimim per pantalla tot el que hem rebut del servidor.

	/*Tancar el socket*/
	close(sFd);
	
	
	
	for(int i = 0; i < strlen(buffer); i ++)
	{
		if (buffer [i - 1] == '\n' && buffer [i] == '\r')
		{
			for (int j = 0; j < i + 1; j ++)
			{
				resposta_header[j] = buffer[j];
			}
			int index = 0;		
			for (int k = i + 2; k <= strlen(buffer); k ++)
			{
				resposta_data[index] = buffer[k];
				index ++;
			}
		}
	}
	
	printf("CAPÇALERA: \n%s\n\n", resposta_header);
	printf("HTML: \n%s\n", resposta_data);
	

	return 0;
}
