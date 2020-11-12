all: i2c
	

i2c: sensor.c
	gcc sensor.c -lsqlite3 -o sensor
	./sensor

clean:
	rm -rf *o  i2c
