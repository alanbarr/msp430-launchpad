CC=msp430-gcc
CFLAGS=-Wall -g -Os

TARGET=msp430g2553

all: clean wdtClock.elf

wdtClock.elf: main.c
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $@ $<

clean:
	-rm wdtClock.elf
