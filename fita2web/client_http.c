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


int http_get(char *nom_servidor, char *cadena_URI, char *resposta_header, char *resposta_data);


int main(int argc, char **argv)
{
    int opc;
    char nom_servidor[32];
	char cadena_URI[32];
	char resposta_header[4256];
	char resposta_data[4256];
    
   while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"servidor",  	required_argument, 0, 's'},
            {"recurs",  	required_argument, 0, 'r'},
            {0,         0,                 0,  0 }
        };

       opc = getopt_long(argc, argv, "s:r:",
                 long_options, &option_index);
        if (opc == -1)
            break;

       switch (opc) {
        
            case 's':
            strcpy(nom_servidor,optarg);
            break;

       case 'r':
            strcpy(cadena_URI,optarg);
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
int http_get (char *nom_servidor, char *cadena_URI, char *resposta_header, char *resposta_data){
	
	struct sockaddr_in	serverAddr;
	char	    serverName[32];
	int			sockAddrSize;
	int			sFd;
	int 		result;
	char		buffer[8512];
	char		missatge[64];
	
	sprintf(serverName,"%s",nom_servidor);
	sprintf(missatge,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",cadena_URI,nom_servidor);

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

