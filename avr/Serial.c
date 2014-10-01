
#include <avr/io.h>
#include "Serial.h"

#ifdef MCUCONF_USE_SERIAL

void serialInit(Serial *pser, uint32_t baud){
	uint32_t divide = (((uint32_t)F_CPU) >> 4) / baud - 1;
	SERIAL0->UBBRL = divide & 0xFF;
	SERIAL0->UBBRH = divide >> 8;
	//SERIAL0->UCSR0B = (1<<RXCIE0) | (1<<RXEN0);	/*Only enable RX*/
	pser->baud = baud;
}

void serialWriteBlock(Serial *pser, uint16_t n, uint8_t buffer[]){
	SERIAL0->UCSRB |= (1<<TXEN0);	/*Enable TX*/
	int i;
	SERIAL0->UCSRA |= (1<<TXC0);
	for(i=0;i<n;i++){
		SERIAL0->UDR = buffer[i];
		while(!(SERIAL0->UCSRA & (1<<UDRE0)));
	}
	while(!(SERIAL0->UCSRA & (1<<TXC0)));	/*Wait until transmission finishes*/
	
	SERIAL0->UCSRB &= ~(1<<TXEN0);	 /*Disable TX*/
}

static void put(char c){
}

static void reg(InputListener il){
}

Serial Serial0 = {
	.put = put,
	.registerListener = reg,
	.baud = 9600,
};
#endif