/*
 * Copyright (C)
 * Alan Barr 2011
 */
#include "usefulFunctions.h"

void setupDefaultPins(void)
{
	P1OUT = 0xFF;
    P1SEL = 0x00;
    P1DIR = 0x00;
	P2OUT = 0xFF;
    P2SEL = 0x00;
    P2DIR = 0x00;
}

