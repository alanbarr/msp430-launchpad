CC=msp430-gcc
AR=msp430-ar

CCFLAGS=-Os -Wall -g -I ../../inc
ARFLAGS=-rcs

LIBPATH=../../lib/usefulFunctions.a
usefulFunctions:    floatToString.o \
                    power.o \
                    stringCompare.o \
                    unsignedIntToString.o \
                    intToString.o \
                    memClear.o \
                    setupDefaultPins.o \
                    stringToUnsignedInt.o \

	$(AR) $(ARFLAGS) $(LIBPATH) $^
	rm $^

%.o:
	$(CC) $(CCFLAGS) -c $*.c 

clean:
	-rm *.o
	-rm ../../lib/usefulFunctions.a
