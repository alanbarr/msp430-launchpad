/* 
** Author: Alan Barr
** Created: February 2012
** Compiler: mspgcc
*
* This project is still in development stage. The ultimate goal is a device
* which can act as a reminder for TV shows, and will change the channel just
* before a show comes on.
* 
* Thanks to the following sites, which helped to get the IR part of this 
* project working.
* http://www.hifi-remote.com/sony/
* http://www.arcfn.com/2010/03/understanding-sony-ir-remote-codes-lirc.html
*/

#include "tvTimer.h"

/***************************** TimeKeeping Globals*****************************/ 

const unsigned char daysInMonth[] = { 31, 28, 31, 30, 30, 31, 
                                      31, 31, 30, 31, 30, 31 };
volatile tTimeStructure gCurrentTime;
tReminderStructure gReminderList[10];

/********************************* IR GLOBALS *********************************/

volatile unsigned int gNumberOfInterruptsToStateChange = 0;
volatile eIrTransmissionState gIrState;
volatile unsigned int irTransmitData = 0;

/* Using the preprocessor to create array of command values. */
#undef COMMAND_NAME_CODE
#define COMMAND_NAME_CODE(NAME, CODE) CODE,
const unsigned int sonyTvIrCommand[] = { COMMAND_LIST };

/* Using the preprocessor to create enum of indexes to array. */
#undef COMMAND_NAME_CODE
#define COMMAND_NAME_CODE(NAME, CODE) NAME,
enum { COMMAND_LIST };

/********************************* Parser GLOBALS *********************************/
#define NUMBER_OF_COMMANDS 7
const commandPair commands[NUMBER_OF_COMMANDS] = {{'a', &(addNewReminder)},
                                                  {'d', &(removeAnEntry)},
                                                  {'r', &(readEntries)},
                                                  {'t', &(transmitReceivedCommand)},
                                                  {'s', &(timeSet)},
                                                  {'b', &(block)},
                                                  {'h', &(printHelp)}
                                                 };

/********************************* Functions **********************************/

int main(void)
{
    volatile char uartRxBuffer[BUFFERSIZE];
    unsigned char bufferIndex = 0;
    
    WDTCTL = WDTPW | WDTHOLD;
    TIME430_CALIBRATE_CLOCK();

    memset((char *)uartRxBuffer, 0, BUFFERSIZE);
    memset((void*)&gCurrentTime, 0, sizeof(tTimeStructure));
    memset(gReminderList, 0, sizeof(tReminderStructure) * REMINDER_LIST_SIZE);

    P1DIR = BIT0 | BIT6;
    P1OUT = 0x00;
    P2OUT = 0x00;

    /* Configuring WDT for second interval to measure time. Using 6 pF cap for 
     * crystal oscillator. */
    WDTCTL  = WDT_ADLY_1000;         
    BCSCTL3 = XCAP_1;  
    IE1     = WDTIE; 
    
    uartSetupPins();
    uartInit(uartRxBuffer, BUFFERSIZE);
    uartPrint("Starting\n\r");
    
    /* Main loop. Wakes from sleep each minute or when the uart has received a
     * character. The UART is checked to see if it contains a valid setting 
     * string. The event list is check against the current time. */
    while (1)
    { 
        /* Check to see if uart buffer index has increased i.e. a character has 
         * been received. */
        if (uartRxBufferIndex)
        {
            /* Unless we have a newline character not interested. bufferIndex
             * is one less as uartRxBufferIndex is incremented after each
             * reception. */
            for (bufferIndex = 0; bufferIndex <= uartRxBufferIndex; bufferIndex++)
            {
                if (uartRxBuffer[bufferIndex] == '\n' && 
                    uartRxBuffer[bufferIndex - 1] == '\r')
                {
                    /* Set the index to the buffer to the end. This is an unused
                     * character and any incoming data will overwrite this 
                     * value */
                    uartRxBufferIndex = UART_INDEX_UNUSED;

                    /* Overwrite the newline with a string terminator */
                    uartRxBuffer[bufferIndex] = '\0';

                    /* Pass the length of the buffer upto and including the
                     * string terminator for parsing.*/
                    parseUartBufferForInfo((char *)uartRxBuffer, bufferIndex + 1);

                    /* Set the uart buffer back to the start ready for a new 
                     * command */
                    memset((char*)uartRxBuffer, 0, BUFFERSIZE);
                    uartRxBufferIndex = 0;
                    uartPrint("\r\n");
                    break;
                }
            }
            
            /* Filled the buffer, reset */
            if (uartRxBufferIndex == BUFFERSIZE - 1)
            {
                uartRxBufferIndex = 0;
                memset((char*)uartRxBuffer, 0, BUFFERSIZE);
            }
        }

        checkForEvent();

        _BIS_SR(LPM0_bits | GIE); /* TODO */
    }

    return 0;
}


/* Initiates a transmission of an IR code by configuring timer A correctly,
 * then entering low power mode until transmission finished. This is repeated
 * 10 times. A sleep is then entered to allow the transmitted command to be 
 * processed by the TV. */
void startTransmit(const unsigned int data)
{
    int ctr;

    for (ctr = 0; ctr < 10; ctr++)
    {
        gIrState = irEntry;
        gNumberOfInterruptsToStateChange = START_TX_INT_NUM;
        irTransmitData = data;
        TA1CCR0  = CW_PERIOD_CYCLES;                        // ~25 uS            
        TA1CCR1  = CW_HALF_PERIOD_CYCLES;                   // ~12.5 uS
        TA1CTL   = TASSEL_2 | MC_1;                         // SMCLK, Up Mod,
        P2DIR   |= IR_LED;                                  // IR led pin output
        TA1CCTL1 = OUTMOD_7;                                // O/P Reset/Set
        TA1CCTL0 = CCIE;                                    // Enable CC0 interrupt
        _BIS_SR(GIE | LPM0_bits);                           // Enter LPM for TX
    }
    
    /* We want a 1s wait between each command transmit. This is handled by
     * configuring T1A to source the crystal and setting the gIrState to the
     * appropriate state */
    gIrState = ir1sTxWait;
    gNumberOfInterruptsToStateChange = START_TX_INT_NUM;
    TA1CCR0  = ACLK_1_SECOND_COUNT;                          
    TA1CTL   = TASSEL_1 | MC_1;                         // SMCLK, Up Mod,
    TA1CCTL0 = CCIE;                                    // Enable CC0 interrupt
    _BIS_SR(GIE | LPM0_bits);                           // Enter LPM for TX
}


/* Handles the transmission of an IR code. The transmission is split into 
 * various "states". During each state the output led will set to either output
 * the carrier wave for IR or remain low. These states last for a calculated 
 * number of interrupts. The MCU will remain in a low power state during the 
 * transmission of an IR code.*/
interrupt(TIMER1_A0_VECTOR) TA1_0_ISR(void)
{
    static int bitsToTransmit = 16;
     
    /* If number of interrupts remaining is zero, process what to do, otherwise 
     * get out of here! */
    if (!(--gNumberOfInterruptsToStateChange))
    {   
        P2SEL &= ~IR_LED;                               // Stop LED output
        
        /* If there was a 600 uS wait previously, check next bit to TX and set 
         * active time appropriately.*/
        if (gIrState == irPause)
        {
            if (irTransmitData & 0x01)
            {
                gNumberOfInterruptsToStateChange = INTS_IN_HIGH_STATE;
            }

            else
            {
                gNumberOfInterruptsToStateChange = INTS_IN_PAUSE_LOW_STATE;
            }
            
            /* Now that the on duration is set, turn on led, and count bit as 
             * transmitted. */
            gIrState = irActive;
            irTransmitData >>= 1;   
            bitsToTransmit  -= 1;
            P2SEL |= IR_LED;                                                    
        }

        /* Otherwise the state to enter is 600 uS pause that precedes high and 
         * low transitions. */
        else if (gIrState == irActive || gIrState == irHeader)
        {
            gIrState = irPause;
            gNumberOfInterruptsToStateChange = INTS_IN_PAUSE_LOW_STATE;
        }

        /* If entering this function for the first time set variables to 
         * default. */
        else if (gIrState == irEntry)
        {
            bitsToTransmit = 16;
            gIrState = irHeader;
            gNumberOfInterruptsToStateChange = INTS_IN_HEADER_STATE;
            P2SEL |= IR_LED;
        }
        
        if (!bitsToTransmit)
        {
            /* If in fact we have transmitted the command, enter clearing 
             * state which is a 45mS delay. This is handled in a TimerA count 
             * of 40,000 * the appropriate number of interrupts for clock 
             * freq.*/
            if (gIrState != irClearing && gIrState != ir1sTxWait)
            {
                gIrState = irClearing;
                TA1CCR0  = TIMER_VALUE_CLEARING;   
                TA1CCR1  = 0x00;
                TA1CCTL1 = 0x00;
                TA1R     = 0x00;
                gNumberOfInterruptsToStateChange = INTS_IN_CLEARING_STATE;
            }

            /* Clearing period has passed, reset timer disabling interrupts. 
             * Exit low power mode upon leaving ISR. */
            else
            {
                gIrState = irInactive;
                TA1CTL   = 0x00;                                        
                TA1CCTL0 = 0x00;                                        
                _BIC_SR_IRQ(LPM0_bits);          
            }
        }
    }
}


/* The watch dog timer is used as an interval timer. It interrupts at 1 sec 
 * intervals. This handles all of the time updating. Each time minutes are 
 * increased, we wake from sleep allowing programs to be checked. */
interrupt(WDT_VECTOR) wdtInterrupt(void)
{
    if (++gCurrentTime.seconds > 59)
    {
        gCurrentTime.seconds = 0;
        _BIC_SR_IRQ(LPM0_bits);
        
        if (++gCurrentTime.minutes > 59) 
        {
            gCurrentTime.minutes = 0;

            if (++gCurrentTime.hours > 23)
            {
                gCurrentTime.hours = 0;
                
                if (++gCurrentTime.date > daysInMonth[gCurrentTime.month - 1])
                {
                    /* Handles extra day in Feb due to leap year */
                    if (gCurrentTime.month == February && gCurrentTime.leapYear)
                    {
                        /* If past the 29th feb, move to next month, otherwise
                         * come back here in 1 day */
                        if (gCurrentTime.date > DAYS_FEB_LEAP_YEAR)
                        {
                            gCurrentTime.date = 1;
                            gCurrentTime.month++;
                        }
                    }

                    /* Not Feb or not a leap year, increment months. */
                    else
                    {
                        gCurrentTime.date = 1;
                        gCurrentTime.month++;
                    }
                  
                    if (gCurrentTime.month > December)
                    {
                        gCurrentTime.month = January;

                        /* Handles leap year boolean */
                        if (++gCurrentTime.year % 4 == 0)
                        {
                            gCurrentTime.leapYear = true;
                        }

                        else
                        {
                            gCurrentTime.leapYear = false;
                        }
                    }
                }
            }
        }
    }
}


/* Checks the first character in the received string to see if it is valid. If
 * it is, it calls the appropriate function. It handles errors raised when 
 * parsing the command string. */
void parseUartBufferForInfo(const char * buffer, const int bufferLength)
{
    int errorReturned = errorNotChanged;
    char errorReportString[] = "ERROR:0\r\n\0";
    int functionIndex;
    
    errorReturned = errorUnrecognisedCommand;
    for (functionIndex = 0; functionIndex < NUMBER_OF_COMMANDS; functionIndex++)
    {
        if (buffer[IDENTIFIER_INDEX] == commands[functionIndex].identifier)
        {
            errorReturned = commands[functionIndex].bufferFunction(buffer, bufferLength);
        }
    }

    errorReportString[6] = errorReturned + 0x30;
    uartPrint(errorReportString);
}


/* Adds a new reminder from UART to the global array. 
 * Expected format: aHhMmDdMmCccNnnnnn0 */
int addNewReminder(const char * buffer, const int bufferLength)
{
    int errorReturn = errorNotChanged;
    int reminderListIndex = 0;
    tReminderStructure * pRem = 0;

    if (bufferLength > MINIMUM_REMINDER_LEN)
    {
        /* Find unused record */
        while (reminderListIndex < REMINDER_LIST_SIZE)
        {
            if (gReminderList[reminderListIndex].used == false)
            {
                break;
            }

            reminderListIndex++;
        }
        
        /* Ensure we left the loop with an empty record */
        if (gReminderList[reminderListIndex].used == false)
        {   
            pRem = &gReminderList[reminderListIndex];

            errorReturn = errorNotAsciiDigit;

            if (getTensFromStr(&buffer[HOUR_INDEX], &(pRem->hour)) == success     &&
                getTensFromStr(&buffer[MINUTE_INDEX], &(pRem->minute)) == success && 
                getTensFromStr(&buffer[DATE_INDEX],  &(pRem->date)) == success    &&
                getTensFromStr(&buffer[MONTH_INDEX],  &(pRem->month)) == success)
            {
                if ((errorReturn = getHundredsFromStr(&buffer[CHANNEL_INDEX], 
                                                      &(pRem->channel))) == success)
                {
                    strcpy(pRem->eventName, &buffer[NAME_INDEX]);
                    pRem->used = true;
                    errorReturn = success;
                }
            }
        }

        else
        {
            errorReturn = errorNoFreeEntries;
        }
    }
    
    else
    {
        errorReturn = errorBufferIncorrectLength;
    }
    return errorReturn;
}


/* Parses the received uart string for the index to the reminder
 * to delete. */
int removeAnEntry(const char * buffer, const int bufferLength)
{
    unsigned char entryToRemove = 0;
    int errorReturn = errorNotChanged;
    
    if ((errorReturn = getTensFromStr(&buffer[1], &entryToRemove)) == success)
    {
        gReminderList[entryToRemove].used = false;
    }
    return errorReturn;
}
 
    
/* Reads all active reminders and transmits them over UART.
 * Format IiHhMmDdMmCccEeeeeeNR0 = 20 chars. Replace NULL w/ newline chars = 22*/
int readEntries(const char * buffer, const int bufferLength)
{
    int errorReturn = errorNotChanged;
    tReminderStructure * pRem;
    int remIndex = 0;
    int strIndex = 0;
    char stringToTx[BUFFERSIZE];

    for (remIndex = 0; remIndex < REMINDER_LIST_SIZE; remIndex++)
    {
        pRem = &gReminderList[remIndex];

        if (pRem->used == true)
        {
            putTensToStr(remIndex, &stringToTx[TX_LIST_INDEX]);
            putTensToStr(pRem->hour, &stringToTx[TX_HOUR_INDEX]);
            putTensToStr(pRem->minute, &stringToTx[TX_MIN_INDEX]);
            putTensToStr(pRem->date, &stringToTx[TX_DATE_INDEX]);
            putTensToStr(pRem->month, &stringToTx[TX_MONTH_INDEX]);
            putHundredsToStr(pRem->channel, &stringToTx[TX_CHANNEL_INDEX]);
            strcpy(&stringToTx[TX_NAME_INDEX], pRem->eventName);
            
            /* Find string terminator, replace with newline, carriage return
             * and null */
            errorReturn = errorNullTermMissing;
            for (strIndex = 0; strIndex < BUFFERSIZE; strIndex++)
            {
                if (stringToTx[strIndex] == '\0')
                {
                    stringToTx[strIndex]     = '\r';
                    stringToTx[strIndex + 1] = '\n';
                    stringToTx[strIndex + 2] = '\0';
                    uartPrint(stringToTx);
                    errorReturn = success;
                    break;
                }
            }
        }
    }

    /* Finally send the current time */
    putTensToStr(gCurrentTime.hours, &stringToTx[0]);
    putTensToStr(gCurrentTime.minutes, &stringToTx[2]);
    putTensToStr(gCurrentTime.seconds, &stringToTx[4]);
    putTensToStr(gCurrentTime.date, &stringToTx[6]);
    putTensToStr(gCurrentTime.month, &stringToTx[8]);
    putTensToStr(gCurrentTime.year, &stringToTx[10]);
    stringToTx[12] = '\r';
    stringToTx[13] = '\n';
    stringToTx[14] = '\0';
    uartPrint(stringToTx);
    errorReturn = success;  /* TODO */
    return errorReturn;
}


/* Gets unit and tens digit from an ASCII string and converts them
 * into an unsigned char */
int getTensFromStr(const char * buffer,  unsigned char * destination)
{
    int errorReturn = errorNotChanged;
    
    if ((buffer[0] >= '0' && buffer[0] <= '9') &&
        (buffer[1] >= '0' && buffer[1] <= '9'))
    {
        *destination  = (buffer[0] - 0x30) * 10;
        *destination += buffer[1] - 0x30;
        errorReturn = success;
    }

    else
    {
        errorReturn = errorNotAsciiDigit;
    }
    
    return errorReturn;
}

int getHundredsFromStr(const char * buffer,  unsigned char * destination)
{
    int errorReturn = errorNotChanged;
    
    if (buffer[0] > '2' || (buffer[0] == '2' && buffer[1] > '5') ||
       (buffer[0] == '2' && buffer[1] == '5' && buffer[2] > '5'))
    {
        errorReturn = errorNumberTooLarge;
    }
    
    else if ((buffer[0] >= '0' && buffer[0] <= '9') &&
             (buffer[1] >= '0' && buffer[1] <= '9') &&
             (buffer[2] >= '0' && buffer[2] <= '9'))
    {
        *destination  = (buffer[0] - 0x30) * 100;
        *destination += (buffer[1] - 0x30) * 10;
        *destination += buffer[2] - 0x30;
        errorReturn   = success;
    }

    else
    {
        errorReturn = errorNotAsciiDigit;
    }
    
    return errorReturn;
}

/* Converts number into two ASCII digits to place in destination array */
int putTensToStr(const unsigned char number, char * destination)
{
    int errorReturn = errorNotChanged;

    if (number < 100)
    {
        destination[1] = (number % 10) + 0x30;
        destination[0] = (number / 10) + 0x30;
        errorReturn = success;
    }

    else
    {
        errorReturn = errorNumberTooLarge;
    }
    
    return errorReturn;
}


/* Converts number into three ASCII digits to place in destination array */
int putHundredsToStr(const unsigned char number, char * destination)
{
    int errorReturn = errorNotChanged;

    if (number < 1000)
    {
        destination[2] = (number % 10) + 0x30;
        destination[1] = ((number / 10) % 10) + 0x30;
        destination[0] = (number / 100) + 0x30;
        errorReturn = success;
    }

    else
    {
        errorReturn = errorNumberTooLarge;
    }

    return errorReturn;
}


/* Gets two digits from the received string and sends the matching command
 * from the sonyTvIRCommand array. Expected tNn */
int transmitReceivedCommand(const char * buffer, const int bufferLength)
{
    int errorReturn = errorNotChanged;
    unsigned char rxCommand = 0;

    if ((errorReturn = getTensFromStr(&buffer[1], &rxCommand)) == success)
    {
        if (rxCommand < MAX_IR_COMMAND)
        {
            startTransmit(sonyTvIrCommand[rxCommand]);
        }

        else
        {
            errorReturn = errorIndexOutOfRange;
        }
    }

    return errorReturn;
}


/* Function checks entries in reminder list against current time. If a reminder
 * event is scheduled, the appropriate channel is transmitted, and the event is
 * marked as inactive. */
void checkForEvent(void)
{
    int remIndex;
    unsigned int firstChannelDigit = 0;
    unsigned int secondChannelDigit = 0;
    unsigned int thirdChannelDigit = 0;

    for (remIndex = 0; remIndex < REMINDER_LIST_SIZE; remIndex++)
    {
        if (gReminderList[remIndex].used == true)
        {
            if (gReminderList[remIndex].month  == gCurrentTime.month    &&
                gReminderList[remIndex].date   == gCurrentTime.date     &&
                gReminderList[remIndex].hour   == gCurrentTime.hours    &&
                gReminderList[remIndex].minute == gCurrentTime.minutes)
            {
                thirdChannelDigit = gReminderList[remIndex].channel % 10;
                gReminderList[remIndex].channel /= 10;
                secondChannelDigit = gReminderList[remIndex].channel % 10;
                gReminderList[remIndex].channel /= 10;
                firstChannelDigit = gReminderList[remIndex].channel % 10;
                
                if (firstChannelDigit)
                {
                    startTransmit(sonyTvIrCommand[firstChannelDigit]);
                }

                if (secondChannelDigit)
                {
                    startTransmit(sonyTvIrCommand[secondChannelDigit]);
                }

                startTransmit(sonyTvIrCommand[thirdChannelDigit]);
                gReminderList[remIndex].used = false;
                break;
            }
        }
    }
}

/* sHrMmSsDdMmYy */
/* Sets the current time to that which is passed in the uart string. */
int timeSet(const char * buffer, const int bufferLength)
{
    int errorReturn = errorNotChanged;
    tTimeStructure tempParsedTime;

    if (bufferLength != BUFFER_TIME_SET_LEN)
    {
        errorReturn = errorBufferIncorrectLength;
    }

    else
    {
        errorReturn = getTensFromStr(&buffer[1], &tempParsedTime.hours);

        if (errorReturn == success)
        {
            errorReturn = getTensFromStr(&buffer[3], &tempParsedTime.minutes);
        }   

        if (errorReturn == success)
        {
            errorReturn = getTensFromStr(&buffer[5], &tempParsedTime.seconds);
        }

        if (errorReturn == success)
        {
            errorReturn = getTensFromStr(&buffer[7], &tempParsedTime.date);
        }

        if (errorReturn == success)
        {
            errorReturn = getTensFromStr(&buffer[9], &tempParsedTime.month);
        }

        if (errorReturn == success)
        {
            errorReturn = getTensFromStr(&buffer[11], &tempParsedTime.year);
        }

        if (errorReturn == success)
        {
            if (tempParsedTime.year % 4)
            {
                tempParsedTime.leapYear = false;
            }

            else
            {
                tempParsedTime.leapYear = true;
            }
            
            /* Everything is correct, overwrite global time keeping struct */
            gCurrentTime = tempParsedTime;
        }
    }
    
    return errorReturn;
}

/* bS */
/* Generates an always on carrier wave to stop other signals reaching the tv. */
int block(const char * buffer, const int bufferLength)
{
    int errorReturn = errorNotChanged;
    
    if (bufferLength != BUFFER_BLOCK_LEN)
    {
        errorReturn = errorBufferIncorrectLength;
    }

    else if (buffer[1] == '1')
    {
        TA1CCR0  = CW_PERIOD_CYCLES;                        // ~25 uS            
        TA1CCR1  = CW_HALF_PERIOD_CYCLES;                   // ~12.5 uS
        TA1CTL   = TASSEL_2 | MC_1;                         // SMCLK, Up Mod,
        P2DIR   |= IR_LED;                                  // IR led pin output
        TA1CCTL1 = OUTMOD_7;                                // O/P Reset/Set
        P2SEL   |= IR_LED;    

        errorReturn = success;
    }

    else
    {
        TA1CCR0  = 0x00;                         
        TA1CCR1  = 0x00;                  
        TA1CTL   = 0x00;
        P2DIR   &= ~IR_LED;
        TA1CCTL1 = 0x00;
        P2SEL   &= ~IR_LED;    

        errorReturn = success;
    }

    return errorReturn;
}

int printHelp(const char * buffer, const int bufferLength)
{
    uartPrint("a - add a new reminder.                 aHhMmDdMmCccNnnnnn\r\n");
    uartPrint("b - blocks IR signals.                  bS\r\n");
    uartPrint("d - deletes an entry by index.          dIi\r\n");
    uartPrint("h - prints help.                        h\r\n");
    uartPrint("r - reads entries.                      r\r\n");
    uartPrint("s - sets the time.                      sHhMmSsDdMmYy\r\n");
    uartPrint("t - transmits an IR command from array. tNn\r\n");
    return success;
}
