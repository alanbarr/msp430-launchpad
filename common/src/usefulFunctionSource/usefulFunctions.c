/*
 * Copyright (C)
 * Alan Barr 2011
 */
#include "usefulFunctions.h"

void floatToString(float floatingNumber, char * floatString, int decimals)
{
	char tempFloatString[7];
	int tempIndex = 0;
	int index = 0;
	int tempValue = 0;
	
	tempValue = (int)floatingNumber;
	floatingNumber -= tempValue;
	
	if (tempValue < 0)
	{	
		tempFloatString[index++] = '-';
		tempValue = -tempValue;
	}
	do
	{
		tempFloatString[tempIndex++] = tempValue%10 + '0';	
	}
	while( (tempValue /= 10) > 0);
	
	tempIndex--;
	
	while(tempIndex > -1)
	{
		floatString[index++] = tempFloatString[tempIndex--];	
	}
	
	floatString[index] = '.';
	tempValue = (int)(floatingNumber * (float)power(10,decimals) );
	
	tempIndex = index;
	
	index += decimals;
	floatString[index + 1] = '\0';
	
	while(index > tempIndex)
	{
		floatString[index--] = tempValue % 10 + '0';
		tempValue /= 10;
	}
	
}

long power(int base, int power)
{
	int ctr;
	long result;
	result = base;
	for(ctr = 1; ctr < power; ctr++)
	{
		result *= base;	
	}
	return result;
}


void intToString(int integer, char * integerString)
{
	char tempIntegerString[6];
	int tempIndex = 0;
	int index = 0;
	
	if (integer < 0)
	{	
		integerString[index++] = '-';
		integer = -integer;
	}
	do
	{
		tempIntegerString[tempIndex++] = integer%10 + '0';	
	}
	while( (integer /= 10) > 0);
	
	tempIndex--;
	
	while(tempIndex > -1)
	{
		integerString[index++] = tempIntegerString[tempIndex--];	
	}
	integerString[index] = '\0';
	
}

     
void unsignedIntToString(unsigned int integer, char * integerString)
{
	char tempIntegerString[6];
	int tempIndex = 0;
	int index = 0;
	
	if (integer < 0)
	{	
		integerString[index++] = '-';
		integer = -integer;
	}
	do
	{
		tempIntegerString[tempIndex++] = integer%10 + '0';	
	}
	while( (integer /= 10) > 0);
	
	tempIndex--;
	
	while(tempIndex > -1)
	{
		integerString[index++] = tempIntegerString[tempIndex--];	
	}
	integerString[index] = '\0';
}


unsigned int stringToUnsignedInt(char * integerString)
{
	unsigned int index = 0;
    unsigned int length = 0;
    unsigned int value = 0;

    //Find length of string
    while (integerString[length] >= 0x30 && integerString[length] <=0x39)
    {
        length++;
    }
    
    while (length && length <= 6)
    {
        length--;
        value += integerString[index++] * power(10,length);
    }
    return value;
}
 
void setupDefaultPins(void)
{
	P1OUT = 0xFF;
    P1SEL = 0x00;
    P1DIR = 0x00;
	P2OUT = 0xFF;
    P2SEL = 0x00;
    P2DIR = 0x00;
}


/*
    Zeros out an area of memory
*/
void memClear(char * dataPtr, unsigned int numberOfBytes)
{
	int ctr;

	for(ctr=0;ctr<numberOfBytes;ctr++)
	{
		dataPtr[ctr] = 0x00;
	}
}


/*  
    Compares a string to a base or master string.
   `Returns 0 for identical, 1 for string two identical until space in place of NULL,
   -1 for not identical
*/
int stringCompare(char * masterString, char * comparisonString)
{
	int rtn = -1;
	
	while(*masterString == *comparisonString)
        {
            if(*masterString == '\0' )
            {
                rtn = 0;            
                break;
            }
            else if( *(masterString+1) == '\0' && (*(comparisonString+1) == ' ' || 
                     *(comparisonString+1) == '\n' || *(comparisonString+1) == '\r'))                          
            {
                rtn = 1;   
                break;
            }

            masterString++;
            comparisonString++;
        }
	return rtn;
}

