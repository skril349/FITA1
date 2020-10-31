
Per a executar el codi s'han de posar aquestes dues comandes:

arm-linux-gnueabihf-gcc informe.c -o sql -L. -lsqlite3
./sql basedades_adstr.db "select * from mesures"

la carpeta que hi ha sqlite-amalgamation he llegit que servia per introduir sqlite al makefile però no m'ha funcionat.
amb els dos codis de la part superior heu de tenir suficient.
