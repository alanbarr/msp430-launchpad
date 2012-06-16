CC=msp430-gcc
CFLAGS= -Wall -g -Os -I ../../common/inc -I ../../common/inc/devices
CPPFLAGS=-D TIME430_CLOCK_FREQ=1 -D LCD_READING_FUNCTIONALITY=1
TARGET=msp430g2553
OUTPUT=lcd.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) $(CPPFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) main.c ../../common/src/lcd_st7066.c

clean:
	-rm $(OUTPUT)
