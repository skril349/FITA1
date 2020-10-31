all: mailTerminal
	

mailTerminal: client.c client.o client
	gcc client.c -o client
	./client -s 172.20.0.21 -o mailfals@euss.es -d 1458570@campus.euss.org

clean:
	rm -rf *o  mailTerminal
