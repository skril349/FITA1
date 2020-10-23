all: http

http: client_http.o
	gcc client_http.o -o http

client_hhtp.o: client_http.c
	gcc -c client_http.c

clean:
	rm -rf *o http
