/*
** Author: Alan Barr 
** Created: 2011
*/
#include "stringFunctions.h"

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

