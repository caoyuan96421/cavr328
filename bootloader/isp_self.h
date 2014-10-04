
#ifndef __ISP_SELF_H__
#define __ISP_SELF_H__

/* Implementation of ISP functions with self-programming*/


/* Prepare connection to target device */
void ispConnect();

/* Close connection to target device */
void ispDisconnect();

/* read an write a byte from isp using hardware (fast) */
uint8_t ispTransmit(uint8_t send_byte);

/* enter programming mode */
uint8_t ispEnterProgrammingMode();

/* read byte from eeprom at given address */
uint8_t ispReadEEPROM(unsigned int address);

/* write byte to flash at given address */
uint8_t ispWriteFlash(unsigned long address, uint8_t data, uint8_t pollmode);

uint8_t ispFlushPage(unsigned long address, uint8_t pollvalue);

/* read byte from flash at given address */
uint8_t ispReadFlash(unsigned long address);

/* write byte to eeprom at given address */
uint8_t ispWriteEEPROM(unsigned int address, uint8_t data);

/* load extended address byte */
void ispLoadExtendedAddressByte(unsigned long address);

#endif /*__ISP_SELF_H__*/