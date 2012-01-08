CC=msp430-gcc
CFLAGS=-Os -Wall -g 

TARGET=msp430g2553

all: clean ldr.elf

ldr.elf: main.c
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $@ $<

clean:
	-rm ldr.elf
