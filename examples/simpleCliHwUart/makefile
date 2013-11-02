CC=msp430-gcc
CFLAGS=-Wall -Os -g -I ../../common/inc -g
CPPFLAGS=-D TIME430_CLOCK_FREQ=$(FREQ)
FREQ=1
TARGET=msp430g2553
OUTPUT=simpleCliHw.elf
all: clean $(OUTPUT)

$(OUTPUT):
	$(CC) $(CFLAGS) $(CPPFLAGS) -mmcu=$(TARGET) -o $(OUTPUT) \
	  											main.c \
		  										commandFunctions.c \
                                                ../../common/src/uart_hw.c \
                                                ../../common/lib/stringFunctions.a
	@echo "Made with Frequency $(FREQ) MHz"

clean:
	-rm $(OUTPUT)
