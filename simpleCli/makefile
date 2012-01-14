CC=msp430-gcc
CFLAGS=-Wall -Os -g -I ../common 
TARGET=msp430g2553
OUTPUT=simpleCli.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) main.c commandFunctions.c ../common/uart.c ../common/usefulFunctions.a

clean:
	-rm $(OUTPUT)
