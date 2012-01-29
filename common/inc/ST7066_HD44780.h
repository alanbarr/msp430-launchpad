/* Author: Alan Barr
** Created: Jan 2012
** This header is the registers and bits of ST7066/HD44780.
** http://www.sparkfun.com/products/9053
*/

/*LCD MODULE PINS*/
/*  1 = VSS
    2 = VDD
    3 = Contrast Adjust
    4 = RS
    5 = RW
    6 = E
    7 = DB0
    8 = DB1     
    9 = DB2 
   10 = DB3 
   11 = DB4     
   12 = DB5     
   13 = DB6     
   14 = DB7 
   15 = LED+
   16 = LED- 
*/
#ifndef ST7066_H_
#define ST7066_H_   
                                                        
#define ST7066_RS                           0x01        
#define ST7066_RW                           0x02        
#define ST7066_E                            0x04        

#define ST7066_DB0                          0x01          
#define ST7066_DB1                          0x02                   
#define ST7066_DB2                          0x04                          
#define ST7066_DB3                          0x80         
#define ST7066_DB4                          0x10               
#define ST7066_DB5                          0x20                     
#define ST7066_DB6                          0x40         
#define ST7066_DB7                          0x80                     

/* Command Identifiers */
#define ST7066_INSTRUCTION_WRITE            0x00        /*RS and RW low*/
#define ST7066_INSTRUCTION_READ             ST7066_RW
#define ST7066_DATA_READ                    (ST7066_RS | ST7066_RW)
#define ST7066_DATA_WRITE                   (ST7066_RS)

/* Clear Display Settings */
#define ST7066_CLEAR_DISPLAY_BITS           0x01

/* Return Home Settings */
#define ST7066_RETURN_HOME_BITS             0x02

/*Entry Mode Settings */
#define ST7066_ENTRY_MODE_BITS              0x04
#define ST7066_MOVE_CURSOR_RIGHT            0x02
#define ST7066_MOVE_CURSOR_LEFT             0x00

#define ST7066_SHIFT_DISPLAY                0x01
#define ST7066_SHIFT_LEFT                   0x02
#define ST7066_SHIFT_RIGHT                  0x00

/* Display On Off Settings */
#define ST7066_DISPLAY_ON_OFF_BITS          0x08
#define ST7066_DISPLAY_ON                   0x04
#define ST7066_DISPLAY_OFF                  0x00
#define ST7066_CURSOR_ON                    0x02
#define ST7066_CURSOR_OFF                   0x00
#define ST7066_CURSOR_BLINK_ON              0x01
#define ST7066_CURSOR_BLINK_OFF             0x00

/* Cursor or Display Shift */
#define ST7066_CUR_DISP_SHIFT_BITS          0x10
#define ST7066_SHIFT_CURSOR_LEFT            0x00
#define ST7066_SHIFT_CURSOR_RIGHT           0x01
#define ST7066_SHIFT_DISPLAY_LEFT           0x02
#define ST7066_SHIFT_DISPLAY_RIGHT          0x03

/* Function Set Settings */
#define ST7066_FUNCTION_SET_BITS            0x20
#define ST7066_BIT_MODE_4                   0x00
#define ST7066_BIT_MODE_8                   0x10
#define ST7066_LINE_MODE_1                  0x00
#define ST7066_LINE_MODE_2                  0x08
#define ST7066_FONT_MODE_5x8                0x00
#define ST7066_FONT_MODE_5x11               0x40

/*Set CGRAM Address Settings */
#define ST7066_CGRAM_BITS                   0x40

/*Set DDRAM Address Settings */
#define ST7066_DDRAM_BITS                   0x80
/*StartUpCommand*/
#define ST7066_START_UP_COMMAND             0x30

/* Read Data Settings */
#define ST7066_BUSY_BIT                     ST7066_DB7

#define ST7066_LINE0_POS0                   1
#define ST7066_LINE0_POS1                   2
#define ST7066_LINE0_POS2                   3
#define ST7066_LINE0_POS3                   4
#define ST7066_LINE0_POS4                   5
#define ST7066_LINE0_POS5                   6
#define ST7066_LINE0_POS7                   7
#define ST7066_LINE0_POS8                   8
#define ST7066_LINE0_POS9                   9
#define ST7066_LINE0_POS10                  10
#define ST7066_LINE0_POS11                  11
#define ST7066_LINE0_POS12                  12
#define ST7066_LINE0_POS13                  13
#define ST7066_LINE0_POS14                  14
#define ST7066_LINE0_POS15                  15
#define ST7066_LINE0_POS16                  16
#define ST7066_LINE0_POS17                  17
#define ST7066_LINE0_POS18                  18
#define ST7066_LINE0_POS19                  19

#define ST7066_LINE1_POS0                   64
#define ST7066_LINE1_POS1                   65
#define ST7066_LINE1_POS2                   66
#define ST7066_LINE1_POS3                   67
#define ST7066_LINE1_POS4                   68
#define ST7066_LINE1_POS5                   69
#define ST7066_LINE1_POS7                   70
#define ST7066_LINE1_POS8                   71
#define ST7066_LINE1_POS9                   72
#define ST7066_LINE1_POS10                  73
#define ST7066_LINE1_POS11                  74
#define ST7066_LINE1_POS12                  75  
#define ST7066_LINE1_POS13                  76  
#define ST7066_LINE1_POS14                  77  
#define ST7066_LINE1_POS15                  78  
#define ST7066_LINE1_POS16                  79  
#define ST7066_LINE1_POS17                  80  
#define ST7066_LINE1_POS18                  81  
#define ST7066_LINE1_POS19                  82  

#endif /*ST7066_H_*/
