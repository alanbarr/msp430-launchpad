/* Author: Alan Barr
   Created: January 2012
   Compiler: mspgcc

   This header file is used to try and provide basic time configuration
   for the launchpad. Define TIME430_CLOCK_FREQ in your project before including
   this header file will use this preprocessor to give you the appropriate
   DCO setup as well correct millisecond and microsecond delays.
   The correct defines for TIME430_CLOCK_FREQ are:
   1,8,12 and 16. These correspond to the desired cpu speed in Mhz.
*/

#ifndef TIME430_H_
#define TIME430_H_

#ifndef TIME430_CLOCK_FREQ
    #error "TIME430_CLOCK_FREQ has not been defined."
#endif

#if TIME430_CLOCK_FREQ == 1
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;
    #define TIME430_US_CYCLES           (1)
    #define TIME430_MS_CYCLES           (1000)
    #define TIME430_CLOCK_HZ            (1000000UL)

#elif TIME430_CLOCK_FREQ == 2
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_2MHZ; DCOCTL = CALDCO_2MHZ;
    #define TIME430_US_CYCLES           (2)
    #define TIME430_MS_CYCLES           (2000)
    #define TIME430_CLOCK_HZ            (2000000UL)

#elif TIME430_CLOCK_FREQ == 8
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_8MHZ; DCOCTL = CALDCO_8MHZ;
    #define TIME430_US_CYCLES           (8)
    #define TIME430_MS_CYCLES           (8000)
    #define TIME430_CLOCK_HZ            (8000000UL)

#elif TIME430_CLOCK_FREQ == 16
    #define TIME430_CALIBRATE_CLOCK()   BCSCTL1 = CALBC1_16MHZ; DCOCTL = CALDCO_16MHZ;
    #define TIME430_US_CYCLES           (16)
    #define TIME430_MS_CYCLES           (16000)
    #define TIME430_CLOCK_HZ            (16000000UL)
#else
    #error "TIME430_CLOCK_FREQ was not 1,2,8 or 16. Please ensure it is defined correctly"
#endif 

/*The largest value which __delay_cycles can take (mspgcc ver. 20110612) is 106609*/
#define TIME430_DELAY_US(x)     __delay_cycles((x) * TIME430_US_CYCLES)
#define TIME430_DELAY_MS(x)     __delay_cycles((x) * TIME430_MS_CYCLES)

#endif /*TIME430_H_*/
