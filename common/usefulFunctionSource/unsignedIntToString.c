/*
 * Copyright (C)
 * Alan Barr 2011
 */
#include "usefulFunctions.h"

void unsignedIntToString(unsigned int integer, char * integerString)
{
	char tempIntegerString[6];
	int tempIndex = 0;
	int index = 0;
	
	do
	{
		tempIntegerString[tempIndex++] = integer%10 + '0';	
	}
    while( (integer /= 10) != 0);
	
	tempIndex--;
	
	while(tempIndex > -1)
	{
		integerString[index++] = tempIntegerString[tempIndex--];	
	}
	integerString[index] = '\0';
}

