CC=msp430-gcc
CFLAGS= -Wall -g -Os
CPPFLAGS=
TARGET=msp430g2553
OUTPUT=simpleSwitch.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) $(CPPFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) main.c

clean:
	-rm $(OUTPUT)
