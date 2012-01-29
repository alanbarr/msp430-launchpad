CC=msp430-gcc
CFLAGS= -Wall -g -Os -I ../common/inc
TARGET=msp430g2553
OUTPUT=i2cBitExpLcd.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) \
    main.c \
    ../common/src/i2c.c ../common/src/i2cBitExpanderLcd.c ../common/lib/usefulFunctions.a\

clean:
	-rm $(OUTPUT)
