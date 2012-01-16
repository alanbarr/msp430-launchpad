/*
** Authors: Alan Barr, mspgcc project
** Created: 2011
** Tailored functionality to libc, you may be preferable to use that.
*/

#ifndef USEFULFUNCTIONS_H_
#define USEFULFUNCTIONS_H_

#include "msp430.h"

void setupDefaultPins(void);
long power(int base, int power);
void floatToString(float floatingNumber, char * integerString, int decimals);
void intToString(int integer,char * integerString);
void unsignedIntToString(unsigned int integer, char * integerString);
unsigned int stringToUnsignedInt(char * integerString);
void memClear(char * dataPtr, unsigned int numberOfBytes);
int stringCompare(char * masterString, char * comparisonString);

/* Copied from the mspgcc souceforge page: 
** http://mspgcc.sourceforge.net/manual/c1408.html
*/
void __inline__ delay(register unsigned int n);

#endif /*USEFULFUNCTIONS_H_*/
