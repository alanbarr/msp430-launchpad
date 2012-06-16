CC=msp430-gcc

CFLAGS=-Wall -Os -g -I ../../common/inc -I ../../common/inc/devices
TARGET=msp430g2553
OUTPUT=expander4Leds.elf

all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) -o $(OUTPUT) -mmcu=$(TARGET) ../../common/src/i2c.c main.c ../../common/lib/usefulFunctions.a

clean:
	-rm $(OUTPUT)
