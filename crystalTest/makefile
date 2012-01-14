CC=msp430-gcc
CFLAGS=-Wall -g -I./inc -L/usr/msp430/lib/ldscripts/msp430g2553/ -I/usr/msp430/include
TARGET=msp430g2553
OUTPUT=crystalTest.elf

all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) main.c

clean:
	-rm $(OUTPUT)
