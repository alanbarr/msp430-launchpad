/* Author: Alan Barr
   Created: January 2012

   This header file is used to try and provide basic time configuration
   for the launchpad. Define your TIME430_CLOCK_FREQ in your project and
   this header file will use this preprocessor to give you the appropriate
   DCO setup as well correct millisecond and microsecond delays.
   The correct defines for TIME430_CLOCK_FREQ are:
   1,8,12 and 16. These correspond to the desired cpu speed in Mhz.
*/

#ifndef TIME430_H_
#define TIME430_H_

#ifndef TIME430_CLOCK_FREQ
    #error "TIME430_CLOCK_FREQ has not been defined. Please define or don't include time430.h"
#endif

#if TIME430_CLOCK_FREQ == 1
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;
    #define TIME430_US_CYCLES           (1)
    #define TIME430_MS_CYCLES           (1000)
    #define TIME430_CLOCK_HZ            (1000000L)

#elif TIME430_CLOCK_FREQ == 2
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_2MHZ; DCOCTL = CALDCO_2MHZ;
    #define TIME430_US_CYCLES           (2)
    #define TIME430_MS_CYCLES           (2000)
    #define TIME430_CLOCK_HZ            (2000000L)

#elif TIME430_CLOCK_FREQ == 8
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_8MHZ; DCOCTL = CALDCO_8MHZ;
    #define TIME430_US_CYCLES           (8)
    #define TIME430_MS_CYCLES           (8000)
    #define TIME430_CLOCK_HZ            (8000000L)

#elif TIME430_CLOCK_FREQ == 16
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_16MHZ; DCOCTL = CALDCO_16MHZ;
    #define TIME430_US_CYCLES           (16)
    #define TIME430_MS_CYCLES           (16000)
    #define TIME430_CLOCK_HZ            (16000000L)
#else
    #error "TIME430_CLOCK_FREQ was not 1,2,8 or 16. Please ensure it is set correctly"
#endif 


#define TIME430_DELAY_US(x)     __delay_cycles((x) * TIME430_US_CYCLES)
#define TIME430_DELAY_MS(x)     __delay_cycles((x) * TIME430_MS_CYCLES)

#endif /*TIME430_H_*/
