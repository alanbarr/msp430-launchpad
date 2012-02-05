CC=msp430-gcc
CFLAGS=-Wall -g -Os -I ../common/inc -I ../common/inc/devices

TARGET=msp430g2553

all: clean wdtClock.elf

wdtClock.elf: main.c
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $@ $< ../common/src/lcd.c

clean:
	-rm wdtClock.elf
