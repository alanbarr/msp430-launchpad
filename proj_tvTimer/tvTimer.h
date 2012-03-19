#ifndef _TV_TIMER_H_
#define _TV_TIMER_H_

#include "string.h"
#include "stdlib.h"
#include "time430.h"
#include "msp430g2553.h"
#include "legacymsp430.h"
#include "uart.h"
/*********************************** Macros ***********************************/

#define IR_LED                      (BIT1)

/* Deinations used for carrier wave */
#define CW_PERIOD_US                (25)
#define CW_PERIOD_CYCLES            (TIME430_US_CYCLES * CW_PERIOD_US)    
#define CW_HALF_PERIOD_CYCLES       (CW_PERIOD_CYCLES / 2) 

#define TIMER_VALUE_CLEARING        (0xFFFF)

#define ACLK_1_SECOND_COUNT         (0x7FFF)
/* Timings for transmission states */
#define CLEARING_TIME_US            (45000)
#define HEADER_STATE_TIME_US        (2400)
#define HIGH_STATE_TIME_US          (1200)
#define PAUSE_LOW_STATE_TIME_US     (600)
#define CLEARING_INT_INTERVAL_US    (TIMER_VALUE_CLEARING / TIME430_US_CYCLES)

/* Interrupts in various states */
#define INTS_IN_CLEARING_STATE      ((CLEARING_TIME_US / \
                                    CLEARING_INT_INTERVAL_US)+1) 
#define INTS_IN_HEADER_STATE        (HEADER_STATE_TIME_US / CW_PERIOD_US)
#define INTS_IN_PAUSE_LOW_STATE     (PAUSE_LOW_STATE_TIME_US  / CW_PERIOD_US)
#define INTS_IN_HIGH_STATE          (HIGH_STATE_TIME_US   / CW_PERIOD_US)

#define START_TX_INT_NUM            1

/* Used to insure index to command array and command always match */
#define COMMAND_NAME_CODE(NAME, CODE)       /*Defined when required*/

/* These codes are in format: 4 Unused bits + Device + Command, 
 * i.e. BIT0 is LSB. The device is 1 i.e tv. */
#define COMMAND_LIST                            /*Command */  /* Index */\
    COMMAND_NAME_CODE(ZERO,             0x0089) /*    0   */  /*  00   */\
    COMMAND_NAME_CODE(ONE,              0x0080) /*    1   */  /*  01   */\
    COMMAND_NAME_CODE(TWO,              0x0081) /*    2   */  /*  02   */\
    COMMAND_NAME_CODE(THREE,            0x0082) /*    3   */  /*  03   */\
    COMMAND_NAME_CODE(FOUR,             0x0083) /*    4   */  /*  04   */\
    COMMAND_NAME_CODE(FIVE,             0x0084) /*    5   */  /*  05   */\
    COMMAND_NAME_CODE(SIX,              0x0085) /*    6   */  /*  06   */\
    COMMAND_NAME_CODE(SEVEN,            0x0086) /*    7   */  /*  07   */\
    COMMAND_NAME_CODE(EIGHT,            0x0087) /*    8   */  /*  08   */\
    COMMAND_NAME_CODE(NINE,             0x0088) /*    9   */  /*  09   */\
    COMMAND_NAME_CODE(CHANNEL_UP,       0x0090) /*   16   */  /*  10   */\
    COMMAND_NAME_CODE(CHANNEL_DOWN,     0x0091) /*   17   */  /*  11   */\
    COMMAND_NAME_CODE(VOLUME_UP,        0x0092) /*   18   */  /*  12   */\
    COMMAND_NAME_CODE(VOLUME_DOWN,      0x0093) /*   19   */  /*  13   */\
    COMMAND_NAME_CODE(COMMAND_MUTE,     0x0094) /*   20   */  /*  14   */\
    COMMAND_NAME_CODE(POWER,            0x0095) /*   21   */  /*  15   */\
    COMMAND_NAME_CODE(BRIGHT_UP,        0x009E) /*   30   */  /*  16   */\
    COMMAND_NAME_CODE(BRIGHT_DOWN,      0x009F) /*   31   */  /*  17   */\
    COMMAND_NAME_CODE(SPEAKERS_TOGGLE,  0x00C0) /*   80   */  /*  18   */\
    COMMAND_NAME_CODE(MAX_IR_COMMAND,   0x0000) /* Should always be last */

#define DAYS_FEB_LEAP_YEAR          29

/* Defines for uart input */
#define REMINDER_LIST_SIZE          12
#define IDENTIFIER_INDEX            0
#define HOUR_INDEX                  1
#define MINUTE_INDEX                3
#define DATE_INDEX                  5
#define MONTH_INDEX                 7
#define CHANNEL_INDEX               9
#define NAME_INDEX                  12
#define NAME_LEN                    10
#define BUFFERSIZE                  23
#define UART_INDEX_UNUSED           22

#define TX_LIST_INDEX               0
#define TX_HOUR_INDEX               (TX_LIST_INDEX    + 2)
#define TX_MIN_INDEX                (TX_HOUR_INDEX    + 2)
#define TX_DATE_INDEX               (TX_MIN_INDEX     + 2)
#define TX_MONTH_INDEX              (TX_DATE_INDEX    + 2)
#define TX_CHANNEL_INDEX            (TX_MONTH_INDEX   + 2)
#define TX_NAME_INDEX               (TX_CHANNEL_INDEX + 3)

#define EVENT_NAME_LEN              7       /* Includes NULL */
#define BUFFER_TIME_SET_LEN         15
#define MINIMUM_REMINDER_LEN        13      /* Null Term, no event name */

/********************************* Prototypes *********************************/

void startTransmit(const unsigned int data);
void parseUartBufferForInfo(const char * buffer, const int bufferLength);
int timeSet(const char * buffer, const int bufferLength);
int addNewReminder(const char * buffer, const int bufferLength);
int removeAnEntry(const char * buffer, const int bufferLength);
int readEntries(void);
int getTensFromStr(const char * buffer,  unsigned char * destination);
int getHundredsFromStr(const char * buffer,  unsigned char * destination);
int transmitReceivedCommand(const char * buffer, const int bufferLength);
void checkForEvent(void);
int putTensToStr(const unsigned char number, char * destination);
int putHundredsToStr(const unsigned char number, char * destination);
void block(void);
/********************************* Data Types *********************************/
typedef enum { false = 0,
               true  = 1,
             } bool;

enum { January = 1, February, March, April, May, June, 
       July, August, September, October, November, December 
     } eMonth;

enum { success = 0, 
       errorNotChanged,
       errorBufferIncorrectLength,
       errorUnrecognisedCommand,
       errorNoFreeEntries,
       errorNotAsciiDigit = 5,
       errorNullTermMissing,
       errorNumberTooLarge,
       errorIndexOutOfRange,
     } eErrorCodes;

typedef struct { bool used;
                 unsigned char hour; 
                 unsigned char minute; 
                 unsigned char date; 
                 unsigned char month;
                 unsigned char year;
                 unsigned char channel;
                 char eventName[EVENT_NAME_LEN];         /*TODO dependant on channel length*/
                } tReminderStructure;

typedef struct { unsigned char seconds;
                 unsigned char minutes;
                 unsigned char hours;
                 unsigned char date;
                 unsigned char month;
                 unsigned char year;
                 bool leapYear;
                } tTimeStructure;

typedef enum { irEntry    = 0x01,
               irHeader   = 0x02,
               irPause    = 0x04, 
               irActive   = 0x08,
               irClearing = 0x10,
               ir1sTxWait = 0x20,
               irInactive = 0x40,
            } eIrTransmissionState;

#endif /* _TV_TIMER_H_ */
