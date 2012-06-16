CC=msp430-gcc
CFLAGS=-Wall -g -Os -I ../../common/inc -I ../../common/inc/devices
CPPFLAGS=-D TIME430_CLOCK_FREQ="1"
OUTPUT=lcdClock.elf
TARGET=msp430g2553

all: clean $(OUTPUT)

$(OUTPUT): main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -mmcu=$(TARGET) -o $@ $< ../../common/src/lcd_st7066.c

clean:
	-rm $(OUTPUT)
