/* Author: Alan Barr
** Created: 15 Jan 2012
**
** Header file for TMP102 Digital Temperature Sensor.
*/
#ifndef TMP102_H_
#define TMP102_H_

/* Register Addresses - 2 Bytes */
#define TEMPERATURE_REGISTER    0x00    /*Read Only*/
#define CONFIGURATION_REGISTER  0x01    /*Read/Write*/
#define TLOW_REGISTER           0x02    /*Read/Write*/
#define THIGH_REGISTER          0x03    /*Read/Write*/

/* Configuration Register Bits */
/* Byte 1 */
#define OS  0x80    /*One Shot temperature measurement mode*/
#define R1  0x40    /*Converstion Resolution bit 1 (Read Only)*/
#define R0  0x20    /*Converstion Resolution bit 0 (Read Only)*/
#define F1  0x10    /*Fault queue bit 1*/
#define F0  0x08    /*Fault queue bit 0*/
#define POL 0x04    /*Polarity of Alert pin*/
#define TM  0x02    /*Thermostat mode*/
#define SD  0x01    /*Shut Down mode*/

/* Byte 2 */ 
#define CR1 0x80    /*Conversion Rate Bit 1*/
#define CR0 0x40    /*Conversion Rate Bit 0*/
#define AL  0x20    /*Alert Bit (Read Only)*/
#define EM  0x10    /*Extended mode*/

/* Conversion Rates */
#define CR_0_25Hz CR0
#define CR_1Hz    CR1
#define CR_4Hz    CR1           /*default*/
#define CR_8Hz    (CR1 + CR0)


#endif /*TMP102_H_*/
