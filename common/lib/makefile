CC=msp430-gcc
AR=msp430-ar

CCFLAGS=-Os -Wall -g -I ../inc
ARFLAGS=-rcs

LIBPATH=stringFunctions.a

strings:    floatToString.o \
            stringCompare.o \
            unsignedIntToString.o \
            intToString.o \
            memClear.o \
            stringToUnsignedInt.o 

	$(AR) $(ARFLAGS) $(LIBPATH) $^
	rm $^

%.o:
	$(CC) $(CCFLAGS) -c src/$*.c 

clean:
	-rm strings.a
