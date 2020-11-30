/**
 *@brief Progama Client HTTP
 *
 *@code
 *~$gcc client_http.c -o http
 *~$./http --servidor 192.168.11.185 --recurs /hola.txt
 *@endcode
 *
 *@file client_http.c
 *@author Daniel Gonzalez
 *@version 1.1
 *@date 24.10.2020
 *
 *@param Adreça del servidor i nom de la pàgina
 *@return 0 si ok
 *
 *@todo Següent Fita.
 *
 *@section LICENSE
 *License GNU/GPL, see COPYING
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/


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
#include <getopt.h>


#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		80


int http_get(char *nom_servidor, char *valorSensor, char *resposta_header, char *resposta_data);


int main(int argc, char **argv)
{
    int opc;
    char nom_servidor[32];
	char cadena_URI[32];
	char resposta_header[4256];
	char resposta_data[4256];
	char valorSensor[32];
    
   while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"servidor",  	required_argument, 0, 's'},
            {"recurs",  	required_argument, 0, 'v'},
            {0,         0,                 0,  0 }
        };

       opc = getopt_long(argc, argv, "s:v:t:",
                 long_options, &option_index);
        if (opc == -1)
            break;

       switch (opc) {
        
            case 's':
            strcpy(nom_servidor,optarg);
            break;

       case 'v':
            strcpy(valorSensor,optarg);
            break;
            
		case 't':
            
            break;


       default:
            printf("?? getopt returned character code 0%o ??\n", opc);
            break;
        }
    }

   if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data);
   exit(EXIT_SUCCESS);
}
int http_get (char *nom_servidor, char *valorSensor, char *resposta_header, char *resposta_data){
	
	struct sockaddr_in	serverAddr;
	char	    serverName[32];
	char		sensors[32];
	int			sockAddrSize;
	int			sFd;
	int 		result;
	char		buffer[8512];
	char		missatge[2000];
	char 		temps[8512]="2020-11-27+12%3A34%3A56";
	sprintf(serverName,"%s",nom_servidor);
	sprintf(sensors,"%s",valorSensor);
	sprintf(missatge,"GET /cloud/guardar_dades.php?id_sensor=1&valor=1929&temps=%s HTTP/1.1\r\nHost: iotlab.euss.cat\r\n\r\n",temps);

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
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
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge);

	/*Rebre*/
	result = read(sFd, buffer, 4256);
	for (int i1 = 0; i1 < strlen(buffer); i1++){
		if (buffer [i1-1] == '\n' && buffer [i1] == '\r'){
			for (int i2 = 0; i2<i1; i2++){
				resposta_header [i2] = buffer[i2];
			}
			int i3=0;
			for (int i2 = i1+2; i2 <= strlen (buffer); i2++){
				resposta_data[i3]=buffer[i2];
				i3++;
			}
		}
	}
	printf("Missatge rebut del servidor(bytes %d): \nResposta:\n%s\n\nData:\n%s\n", result, resposta_header, resposta_data);

	/*Tancar el socket*/
	close(sFd);

	return 0;
	}

