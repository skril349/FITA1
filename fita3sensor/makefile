all: i2c
	

i2c: sensor.c sensor.o sensor
	gcc sensor.c -o sensor
	./sensor

clean:
	rm -rf *o  i2c
