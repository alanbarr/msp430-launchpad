/*
** Author: Alan Barr 
** Created: 2011
*/
#include "stringFunctions.h"
 
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

