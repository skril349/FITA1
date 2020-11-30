#include <stdio.h>
#include "func.h"


int main(int argc, char *argv[])
{
	char nom_servidor[32] = "192.168.11.185";
	char cadena_URI[32] = "iotlab.euss.es";
	char resposta_header[4256];
	char resposta_data[5256];
	
		
	http_get(nom_servidor, cadena_URI, resposta_header, resposta_data );
}
