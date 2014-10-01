
#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "mcuconf.h"
#include "Channel.h"

#if MCUCONF_USE_SERIAL
typedef struct {
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t reserved1;
	volatile uint8_t UBBRL;
	volatile uint8_t UBBRH;
	volatile uint8_t UDR;
} USART_struct;

typedef struct Se{
	_interface_output_channel;
	_interface_input_channel;
	
	uint32_t baud;
	
} Serial;

extern Serial Serial0;
#define SERIAL0_OFFSET	0xC0
#define SERIAL0 ((USART_struct *)SERIAL0_OFFSET)

void serialInit(Serial *p, uint32_t baud);
void serialWriteBlock(Serial *p, uint16_t n, uint8_t buffer[]);

#endif /*MCUCONF_USE_SERIAL*/

#endif /*__SERIAL_H__*/