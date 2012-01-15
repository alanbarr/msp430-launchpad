/*
** Author: Alan Barr
** Created: 2011
*/
#include "usefulFunctions.h"

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
