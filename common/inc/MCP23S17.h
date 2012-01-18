/* Author: Alan Barr
** Created: January 2012 
**
** Header file for MCP23S17 bit expander.
** Register addresses and unique bits.
*/

#ifndef MCP23S17_H_
#define MCP23S17_H_

/* Address Pins */
#define MCP23S17_ADDR_PIN0          0x01     /*A0*/
#define MCP23S17_ADDR_PIN1          0x02     /*A1*/
#define MCP23S17_ADDR_PIN2          0x04     /*A2*/

/* 7 Bit Addresses */
#define MCP23S17_ADDRESS            0x20  
#define MCP23S17_ADDRESS_0          0x20  
#define MCP23S17_ADDRESS_1          0x21  
#define MCP23S17_ADDRESS_2          0x22  
#define MCP23S17_ADDRESS_3          0x23  
#define MCP23S17_ADDRESS_4          0x24  
#define MCP23S17_ADDRESS_5          0x25  
#define MCP23S17_ADDRESS_6          0x26  
#define MCP23S17_ADDRESS_7          0x27  

/* 8 Bit Addresses */
#define MCP23S17_8B_ADDRESS         0x40   
#define MCP23S17_8B_ADDRESS_0       0x40     
#define MCP23S17_8B_ADDRESS_1       0x42     
#define MCP23S17_8B_ADDRESS_2       0x44     
#define MCP23S17_8B_ADDRESS_3       0x46     
#define MCP23S17_8B_ADDRESS_4       0x48     
#define MCP23S17_8B_ADDRESS_5       0x4A     
#define MCP23S17_8B_ADDRESS_6       0x4C     
#define MCP23S17_8B_ADDRESS_7       0x4E     

/* Register Addresses */
#define MCP23S17_IODIRA             0x00    /*I/O DIRECTION REGISTER A*/
#define MCP23S17_IODIRB             0x01    /*I/O DIRECTION REGISTER B*/
#define MCP23S17_IPOLA              0x02    /*INPUT POLARITY REGISTER A*/
#define MCP23S17_IPOLB              0x03    /*INPUT POLARITY REGISTER B*/
#define MCP23S17_GPINTENA           0x04    /*INTERRUPT-ON-CHANGE CONTROL REGISTER A*/
#define MCP23S17_GPINTENB           0x05    /*INTERRUPT-ON-CHANGE CONTROL REGISTER B*/
#define MCP23S17_DEFVALA            0x06    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE A*/
#define MCP23S17_DEFVALB            0x07    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE B*/
#define MCP23S17_INTCONA            0x08    /*INTERRUPT CONTROL REGISTER A*/
#define MCP23S17_INTCONB            0x09    /*INTERRUPT CONTROL REGISTER B*/
#define MCP23S17_IOCON              0x0A    /*CONFIGURATION REGISTER A*/
#define MCP23S17_IOCON              0x0B    /*CONFIGURATION REGISTER B*/
#define MCP23S17_GPPUA              0x0C    /*PULL-UP RESISTOR A*/
#define MCP23S17_GPPUB              0x0D    /*PULL-UP RESISTOR B*/
#define MCP23S17_INTFA              0x0E    /*INTERRUPT FLAG REGISTER A*/
#define MCP23S17_INTFB              0x0F    /*INTERRUPT FLAG REGISTER B*/
#define MCP23S17_INTCAPA            0x10    /*INTERRUPT CAPTURE REGISTER A*/
#define MCP23S17_INTCAPB            0x11    /*INTERRUPT CAPTURE REGISTER B*/
#define MCP23S17_GPIOA              0x12    /*PORT REGISTER A*/
#define MCP23S17_GPIOB              0x13    /*PORT REGISTER B*/
#define MCP23S17_OLATA              0x14    /*OUTPUT LATCH REGISTER  A*/
#define MCP23S17_OLATB              0x15    /*OUTPUT LATCH REGISTER  B*/

/*Register addresses if BANK = 1 */
#define MCP23S17_IODIRA_BANK        0x00    /*I/O DIRECTION REGISTER A*/                            
#define MCP23S17_IODIRB_BANK        0x10    /*I/O DIRECTION REGISTER B*/
#define MCP23S17_IPOLA_BANK         0x01    /*INPUT POLARITY REGISTER A*/
#define MCP23S17_IPOLB_BANK         0x11    /*INPUT POLARITY REGISTER B*/
#define MCP23S17_GPINTENA_BANK      0x02    /*INTERRUPT-ON-CHANGE CONTROL REGISTER A*/
#define MCP23S17_GPINTENB_BANK      0x12    /*INTERRUPT-ON-CHANGE CONTROL REGISTER B*/
#define MCP23S17_DEFVALA_BANK       0x03    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE A*/
#define MCP23S17_DEFVALB_BANK       0x13    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE B*/
#define MCP23S17_INTCONA_BANK       0x04    /*INTERRUPT CONTROL REGISTER A*/
#define MCP23S17_INTCONB_BANK       0x14    /*INTERRUPT CONTROL REGISTER B*/
#define MCP23S17_IOCON_BANK         0x05    /*CONFIGURATION REGISTER A*/
#define MCP23S17_IOCON_BANK         0x15    /*CONFIGURATION REGISTER B*/
#define MCP23S17_GPPUA_BANK         0x06    /*PULL-UP RESISTOR A*/
#define MCP23S17_GPPUB_BANK         0x16    /*PULL-UP RESISTOR B*/
#define MCP23S17_INTFA_BANK         0x07    /*INTERRUPT FLAG REGISTER A*/
#define MCP23S17_INTFB_BANK         0x17    /*INTERRUPT FLAG REGISTER B*/
#define MCP23S17_INTCAPA_BANK       0x08    /*INTERRUPT CAPTURE REGISTER A*/
#define MCP23S17_INTCAPB_BANK       0x18    /*INTERRUPT CAPTURE REGISTER B*/
#define MCP23S17_GPIOA_BANK         0x09    /*PORT REGISTER A*/
#define MCP23S17_GPIOB_BANK         0x19    /*PORT REGISTER B*/
#define MCP23S17_OLATA_BANK         0x0A    /*OUTPUT LATCH REGISTER A*/
#define MCP23S17_OLATB_BANK         0x1A    /*OUTPUT LATCH REGISTER B*/

/* IOCON bits */
#define MCP23S17_INTPOL             0x20    /*This bit sets the polarity of the INT output pin.*/
#define MCP23S17_ODR                0x40    /*This bit configures the INT pin as an open-drain output.*/
#define MCP23S17_HAEN               0x80    /*Hardware Address Enable bit.*/
#define MCP23S17_DISSLW             0x10    /*Slew Rate control bit for SDA output.*/
#define MCP23S17_SWQOP              0x20    /*Sequential Operation mode bit.*/
#define MCP23S17_MIRROR             0x40    /*INT Pins Mirror bit.*/
#define MCP23S17_BANK               0x80    /*Controls how the registers are addressed.*/

/* All other registers effect the associated pin. The following can be used to address these. */
#define MCP23S17_PIN0               0x01
#define MCP23S17_PIN1               0x02
#define MCP23S17_PIN2               0x04
#define MCP23S17_PIN3               0x08
#define MCP23S17_PIN4               0x10
#define MCP23S17_PIN5               0x20
#define MCP23S17_PIN6               0x40
#define MCP23S17_PIN7               0x80

#endif /*MCP23S17_H_*/

