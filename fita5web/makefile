all: i2c
	

i2c: main.c
	gcc -fPIC -c -o func.o func.c
	gcc -shared -fPIC -o libfunc.so func.o
	gcc -c -o main.o main.c -I.
	gcc -o main main.o -L. -lfunc -lsqlite3 -Wall -lrt -lpthread
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/pi/Desktop/FITA1/fita5web
	./main

clean:
	rm -rf *o  i2c
