/*
** Author: Alan Barr 
** Created: 2011
*/
/* Builds with mspgcc */
/* This file contains the commands which can be ran over UART */
#include "cli.h"

const tCommandStruct commandList[COMMANDLISTSIZE]= {{"led one on",&(led1On)},
                                                    {"led two on",&(led2On)},
                                                    {"led one off",&(led1Off)},
                                                    {"led two off",&(led2Off)},
                                                    {"led one toggle",&(led1Toggle)},
                                                    {"led two toggle",&(led2Toggle)},
                                                    {"help", &(cliHelp)},
                                                    {"reset", &(reset)}
                                                    };   

void led1On(void)
{
	P1OUT |= LED1;
}

void led2On(void)
{
	P1OUT |= LED2;
    PRINT("TWO\n\r");
}

void led1Off(void)
{
	P1OUT &= ~LED1;
}

void led2Off(void)
{
	P1OUT &= ~LED2;
}

void led1Toggle(void)
{
	P1OUT ^= LED1;
}

void led2Toggle(void)
{
	P1OUT ^= LED2;
}

/* Resets mcu by writing to Watchdog Register without specifying password. */
void reset(void)
{
    WDTCTL = 0x00;
}

void cliHelp(void)
{
	int ctr;
	PRINT("Options:\n\r");
	
	for(ctr=0;ctr<COMMANDLISTSIZE-1;ctr++)
	{
		PRINT(commandList[ctr].commandString);
		PRINT("\n\r");
	}
}



