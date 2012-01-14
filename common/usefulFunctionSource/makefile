CC=msp430-gcc
AR=msp430-ar

CCFLAGS=-Os -Wall -g -I ../
ARFLAGS=-rcs

all: floatToString.o \
     power.o \
     stringCompare.o \
     unsignedIntToString.o \
     intToString.o \
     memClear.o \
     setupDefaultPins.o \
     stringToUnsignedInt.o 

	$(AR) $(ARFLAGS) ../usefulFunctions.a $^
	rm $^

%.o:
	$(CC) $(CCFLAGS) -c $*.c 

clean:
	-rm *.o
	-rm ../usefulFunctions.a
