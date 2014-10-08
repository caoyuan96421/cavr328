
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Serial.h"
#include "FIFO.h"

#if MCUCONF_USE_SERIAL

void serialInit(Serial *pser, uint32_t baud){
	uint32_t divide = (((uint32_t)F_CPU) >> 4) / baud - 1;
	SERIAL0->UBRRL = divide & 0xFF;
	SERIAL0->UBRRH = divide >> 8;
	//SERIAL0->UCSR0B = (1<<RXCIE0) | (1<<RXEN0);	/*Only enable RX*/
	pser->baud = baud;
#ifdef MCUCONF_USE_SERIAL_ASYNC
	fifoInit(&pser->tx_buffer, MCUCONF_SERIAL_TX_BUFFER_SIZE);
#endif
}

void serialWriteBlocking(Serial *pser, uint16_t n, uint8_t buffer[]){
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

#ifdef MCUCONF_USE_SERIAL_ASYNC

void serialWrite(Serial *pser, uint16_t n, const uint8_t buffer[]){
	int i;
	if(n<=0)
		return;	/*sanity check*/
	while(fifoPush(&pser->tx_buffer, buffer[0]));
	SERIAL0->UCSRB |= (1<<TXEN0) | (1<<UDRIE0);
	for(i=1;i<n;i++){
		while(fifoPush(&pser->tx_buffer, buffer[i]));
	}
}

uint8_t serialWriteI(Serial *pser, uint16_t n, const uint8_t buffer[]){
	int i;
	uint8_t result=0;
	if(n<=0)
		return 0;	/*sanity check*/
	result |= fifoPush(&pser->tx_buffer, buffer[0]);
	SERIAL0->UCSRB |= (1<<TXEN0) | (1<<UDRIE0);
	for(i=1;i<n;i++){
		result |= fifoPush(&pser->tx_buffer, buffer[i]);
	}
	return result;
}

ISR(USART_UDRE_vect){
	if(!fifoIsEmpty(&(Serial0.tx_buffer))){
		SERIAL0->UDR = fifoPop(&Serial0.tx_buffer);
	}
	else{
		SERIAL0->UCSRB &= ~((1<<TXEN0) | (1<<UDRIE0));
	}
}

#endif


ISR(USART_RX_vect){
}

static void put(char c){
#ifdef MCUCONF_USE_SERIAL_ASYNC
	fifoPush(&Serial0.tx_buffer, c);
	SERIAL0->UCSRB |= (1<<TXEN0) | (1<<UDRIE0);
#else
	SERIAL0->UDR = c;
	SERIAL0->UCSRA |= (1<<TXC0);	/*Clear flag*/
	SERIAL0->UCSRB = (1<<TXEN0);
	while(!(SERIAL0->UCSRA & (1<<TXC0)));	/*Wait until transmission finishes*/
#endif
}

static void reg(InputListener *il){
}

Serial Serial0 = {
	.put = put,
	.registerListener = reg,
	.baud = 9600,
};
#endif