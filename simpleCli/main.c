/*
** Author: Alan Barr 
** Created: 2011
*/
/* Builds with mspgcc */
/* This project demonstrates a simple command line interface on the launchpad
** Communication with the launchpad is achieved over the usb UART interface
** using a bit banged Timer A based UART. 
** A program such as HyperTerminal (Windows) or Minicom/CuteCom (Linux)
** can be used to communicate with the Launchpad. (On Linux the Launchpad shows 
** up as /dev/ACM0.
** Strings received at the MSP430 are parsed to see if they match the commands
** defined in a command structure. If they do, their counter part functions 
** are called via function pointers.
** This example requires only a launchpad, so the examples are rather basic.
** You can turn on, off or toggle the launchpad leds.
*/
#include "cli.h"

static void commandParser(char * stringToParse, tCommandStruct * commandStructPtr);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
  
    BCSCTL1 = CALBC1_1MHZ;                  // 1MHZ
    DCOCTL = CALDCO_1MHZ;

    P1DIR = LED1 + LED2;                    // LED pins outputs and high
    P1OUT = LED1 + LED2;
	
    uartSetupPins();       
	uartInit();

	memClear((char *)uartRxBuffer, UARTBUFSIZE);
	PRINT("Cli Started.\n\r");
    cliHelp();
	PRINT(PROMPT);
    
    /* Loop constantly checks UART buffer for a newline or carriage return.
     * If it finds one it calls the parser to check if it was a valid command.
     * The uartBuffer is handled here for overflow. 
     */
	while(1)
	{   
        if(uartRxBufferIndex >= UARTBUFSIZE)
		{
			uartRxBufferIndex = 0;
			memClear((char*)uartRxBuffer, sizeof(uartRxBuffer));
			PRINT(PROMPT);
		}

        /* If last char received was a newline, parse */
		else if(uartRxBuffer[uartRxBufferIndex -1] == '\n' || 
                uartRxBuffer[uartRxBufferIndex -1] == '\r')
		{
			uartRxBuffer[uartRxBufferIndex - 1] = '\0';
			commandParser((char *)uartRxBuffer, (tCommandStruct *)commandList);
			uartRxBufferIndex = 0;
			memClear((char *)uartRxBuffer, UARTBUFSIZE );
			PRINT(PROMPT);
		}
	}
    return 0;	
}

/* Function runs through all commands in argument of tCommandStruct comparing with
 * stringToParse. If a command matches, the corresponding function pointer is called
 */
static void commandParser(char * stringToParse, tCommandStruct * commandStructPtr)
{
	int ctr;
    
    for(ctr = 0; ctr < COMMANDLISTSIZE; ctr++)
    {
        if (stringCompare(commandStructPtr[ctr].commandString, stringToParse) >= 0)
        {
        	commandStructPtr[ctr].function();
        }
    }
}



