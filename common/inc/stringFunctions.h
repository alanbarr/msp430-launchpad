/*
** Authors: Alan Barr, mspgcc project
** Created: 2011
** Tailored functionality to libc, you may be preferable to use that.
*/

#ifndef STRINGFUNCTIONS_H_
#define STRINGFUNCTIONS_H_

#include "msp430.h"

void floatToString(float floatingNumber, char * integerString, int decimals);
void intToString(int integer,char * integerString);
void unsignedIntToString(unsigned int integer, char * integerString);
unsigned int stringToUnsignedInt(char * integerString);
void memClear(char * dataPtr, unsigned int numberOfBytes);
int stringCompare(char * masterString, char * comparisonString);

#endif /*STRINGFUNCTIONS_H_*/
