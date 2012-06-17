/*
** Author: Alan Barr 
** Created: 2011
*/
#include "stringFunctions.h"
#include "math.h"
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
    tempValue = (int)(floatingNumber * (float)pow(10,decimals));
    
    tempIndex = index;
    
    index += decimals;
    floatString[index + 1] = '\0';
    
    while(index > tempIndex)
    {
        floatString[index--] = tempValue % 10 + '0';
        tempValue /= 10;
    }
    
}

