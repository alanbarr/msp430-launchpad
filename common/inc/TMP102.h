/* Author: Alan Barr
** Created: 15 Jan 2012
**
** Header file for TMP102 Digital Temperature Sensor.
*/
#ifndef TMP102_H_
#define TMP102_H_

/* 7 bit i2c Addressing */
#define TMP102_ADDRESS                  0x48    
#define TMP102_ADDRESS_0                0x48    
#define TMP102_ADDRESS_1                0x49    

/* 8 bit i2c Addressing (Left shifted) */
#define TMP102_8B_ADDRESS               0x90    
#define TMP102_8B_ADDRESS_0             0x90    
#define TMP102_8B_ADDRESS_1             0x92    

/* Address pins */
#define TMP102_ADDR_PIN0                0x01    /*A0*/

/* Register Addresses - 2 Bytes */
#define TMP102_TEMPERATURE_REGISTER     0x00    /*Read Only*/
#define TMP102_CONFIGURATION_REGISTER   0x01    /*Read/Write*/
#define TMP102_TLOW_REGISTER            0x02    /*Read/Write*/
#define TMP102_THIGH_REGISTER           0x03    /*Read/Write*/

/* Configuration Register Bits */
/* Byte 1 */
#define TMP102_OS                       0x80    /*One Shot temperature measurement mode*/
#define TMP102_R1                       0x40    /*Converstion Resolution bit 1 (Read Only)*/
#define TMP102_R0                       0x20    /*Converstion Resolution bit 0 (Read Only)*/
#define TMP102_F1                       0x10    /*Fault queue bit 1*/
#define TMP102_F0                       0x08    /*Fault queue bit 0*/
#define TMP102_POL                      0x04    /*Polarity of Alert pin*/
#define TMP102_TM                       0x02    /*Thermostat mode*/
#define TMP102_SD                       0x01    /*Shut Down mode*/

/* Byte 2 */ 
#define TMP102_CR1                      0x80    /*Conversion Rate Bit 1*/
#define TMP102_CR0                      0x40    /*Conversion Rate Bit 0*/
#define TMP102_AL                       0x20    /*Alert Bit (Read Only)*/
#define TMP102_EM                       0x10    /*Extended mode*/

/* Conversion Rates */
#define TMP102_CR_0_25Hz                CR0
#define TMP102_CR_1Hz                   CR1
#define TMP102_CR_4Hz                   CR1           /*default*/
#define TMP102_CR_8Hz                   (CR1 + CR0)

/* Temp = Temp. Register Value * TMP102_CONVERSION */
#define TMP102_CONVERSION               0.0625

#endif /*TMP102_H_*/
