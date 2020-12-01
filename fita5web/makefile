all: i2c
	

i2c: main.c
	gcc -shared -fPIC -c -o func.o func.c
	gcc -fPIC -c -o func.o func.c
	gcc -c -o main.o main.c -I.
	gcc -o main main.o -L. -lfunc -lsqlite3 -Wall
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/pi/Desktop/FITA1/fita5web
	./main

clean:
	rm -rf *o  i2c
