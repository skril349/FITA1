all: sensor
	

sensor: adc_spi_mcp3008.c adc_spi_mcp3008.o spi
	gcc adc_spi_mcp3008.c -o spi
	./spi

clean:
	rm -rf *o  sensor
