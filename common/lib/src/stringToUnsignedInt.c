/*
** Author: Alan Barr
** Created: 2011
*/
#include "stringFunctions.h"
#include "math.h"

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
        value += integerString[index++] * pow(10,length);
    }
    return value;
}

