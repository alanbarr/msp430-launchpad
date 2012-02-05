CC=msp430-gcc
CFLAGS=-Wall -g -Os -I ../common/inc -I ../common/inc/devices
OUTPUT=lcdClock.elf
TARGET=msp430g2553

all: clean $(OUTPUT)

$(OUTPUT): main.c
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $@ $< ../common/src/lcd.c

clean:
	-rm $(OUTPUT)
