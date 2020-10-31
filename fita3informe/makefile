all: sqlite
	

sqlite: informe.c informe.o
	arm-linux-gnueabihf-gcc informe.c -o sql -L. -lsqlite3
	./sql basedades_adstr.db "select * from mesures

clean:
	rm -rf *o client