CC=msp430-gcc
CFLAGS= -Wall -g -Os -I ../common/inc -I ../common/inc/devices
TARGET=msp430g2553
OUTPUT=lcd.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) \
    main.c ../common/src/lcd.c

clean:
	-rm $(OUTPUT)
