/**
 *@brief Progama Client SMTP
 *
 *@code
 *~$gcc client_mail.c -o smtp
 *~$./smtp --servidor 172.20.0.21 --origen danielgm@me.com --desti 1104921@campus.euss.org
 *@endcode
 *
 *@file client_mail.c
 *@author Daniel Gonzalez
 *@version 1.1
 *@date 26.10.2020
 *
 *@param Adreça del servidor, adreça electrònica d'origen i adreça electrònica de destí.
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
#include <time.h> 

#define REQUEST_MSG_SIZE 1024
#define REPLY_MSG_SIZE 500
#define SERVER_PORT_NUM 25

int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email);


int main(int argc, char *argv[])
{
	char nom_servidor[32];
	char email_destinatari[REPLY_MSG_SIZE]; 				//Email al que volem que arribin els correus.
	char email_remitent[REPLY_MSG_SIZE]; 					//Email remitent dels correus.
	char text_email[REPLY_MSG_SIZE] = "Hola,\nText del email de prova\n\nAtentament\n\nDaniel"; 	//Text del email.	


	int opt= 0;
    static struct option long_options[] = 
    {
        {"servidor", required_argument, 0, 's'},
        {"origen", required_argument, 0, 'o'},
        {"desti", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"s:o:d:", long_options, &long_index )) != -1) 
    {
        switch (opt) 
        {

             case 's' : strcpy(nom_servidor, optarg); 
                 break;
             case 'o' : strcpy(email_remitent, optarg);
                 break;
             case 'd' : strcpy(email_destinatari, optarg);
                 break;
             default: 
                 exit(EXIT_FAILURE);
        }
    }
    
    /*printf("Aquest codi està generant un email accedint des del terminal on les dades estàn organitzades de la següent manera:\n");
    printf("./client -s 172.20.0.21 -o mailfals@euss.es -d 1104921@campus.euss.org");
    delay(5000);
    system("cls");
    printf("Servidor: %s, Destinatari: %s, Remitent: %s.\n", nom_servidor, email_destinatari, email_remitent);*/




	email(nom_servidor, email_destinatari, email_remitent, text_email);
	return(0);
}

int email( char *nom_servidor, char *email_destinatari, char *email_remitent, char *text_email)
{
		struct sockaddr_in serverAddr;
	int sockAddrSize;
	int sFd;
	int result;
	char BufRead[REPLY_MSG_SIZE];
	char BufWrite[REPLY_MSG_SIZE];

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);
	if(sFd < 0)
	{
		perror("socket");
		exit(1);
	}

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); 				//Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM); 			//Posem el nostre port.
	serverAddr.sin_addr.s_addr = inet_addr(nom_servidor); 	//Posem l'adreça del servidor.

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));


	/*Presentació del servidor*/
	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	if (result < 0)
	{
		perror("Read presentacio:");
		exit(1);
	}
	printf ("Rebut(%d): %s\n", result, BufRead);

	/*HELO*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "HELO Rpi30.iotlab.euss.es\n"); 		//Iniciem la conversació amb HELO.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));
	if (result < 0)
	{
		perror("Write");
		exit(1);
	}

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): -> %s\n", result, BufRead);


	/*MAIL FROM*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "MAIL FROM: %s\n", email_remitent); 	//Enviem l'adreça d'email del remitent del correu.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);	
	
	
	/*RCPT TO*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "RCPT TO: %s\n", email_destinatari); //Enviem l'adreça d'email del destinatari del correu.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);


	/*DATA*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "DATA\n");							//Enviem el comandament DATA.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);
	
	
	/*Subject*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "Subject: Email de Prova\n");//Enviem el "Subject".
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));		//No esperem cap resposta de Mercuri.

	
	/*From*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "From: %s\n", email_remitent);		//Indiquem el remitent, pot ser el mateix d'abans o un altre.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));		//Tampoc esperem cap resposta de Mercuri.

	
	/*To*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "To: %s\n", email_destinatari);		//Tornem a introduïr el destinatari.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));		//Tampoc esperem cap resposta de Mercuri.

	
	/*Text*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "%s \n.\n", text_email);				//Enviem el text que forma part del cos del programa.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);

	
	/*QUIT*/
	memset(BufWrite, 0, REPLY_MSG_SIZE);
	sprintf (BufWrite, "QUIT\n");							//Finalitzem la comunicació.
	printf("Enviat: %s\n", BufWrite);
	result = write(sFd, BufWrite, strlen(BufWrite));

	memset(BufRead, 0, REPLY_MSG_SIZE);
	result = read(sFd, BufRead, REPLY_MSG_SIZE);
	printf ("Rebut(%d): %s\n", result, BufRead);


	close(sFd);

	return 0;
}
