CC=msp430-gcc
CCFLAGS=-Wall -g -Os -I ../common/inc -I ../common/inc/devices
CPPFLAGS=-D TIME430_CLOCK_FREQ=$(FREQ)
TARGET=msp430g2553
OUTPUT=tvTimer.elf
FREQ=1
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CCFLAGS) $(CPPFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) main.c
	echo "Made with Frequency $(FREQ) MHz"

clean:
	-rm $(OUTPUT)
