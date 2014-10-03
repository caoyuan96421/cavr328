
#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "mcuconf.h"
#include "Channel.h"
#include "FIFO.h"

#if MCUCONF_USE_SERIAL
typedef struct {
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t reserved1;
	volatile uint8_t UBRRL;
	volatile uint8_t UBRRH;
	volatile uint8_t UDR;
} USART_struct;

typedef struct Se{
	_interface_output_channel;
	_interface_input_channel;
	
	uint32_t baud;
#ifdef MCUCONF_USE_SERIAL_ASYNC
	FIFO tx_buffer;
#endif
	
} Serial;

extern Serial Serial0;
#define SERIAL0_OFFSET	0xC0
#define SERIAL0 ((USART_struct *)SERIAL0_OFFSET)

void serialInit(Serial *p, uint32_t baud);
void serialWriteBlocking(Serial *p, uint16_t n, uint8_t buffer[]);

#ifdef MCUCONF_USE_SERIAL_ASYNC
void serialWrite(Serial *p, uint16_t n, const uint8_t buffer[]);
uint8_t serialWriteI(Serial *p, uint16_t n, const uint8_t buffer[]);	/*Called inside ISR, no waiting if buffer is full*/
#endif


#endif /*MCUCONF_USE_SERIAL*/

#endif /*__SERIAL_H__*/