
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usbdrv.h"

#define BAUD 9600
#define DIVIDE ((((uint32_t)F_CPU) >> 4) / BAUD - 1)
extern void updateUSB();


FUSEMEM __fuse_t fusedata = {
	0xFF,
	0xD8,		// BOOTRST = 0 (enabled)
	0xFF
}; 

void usart0_init(){
	UBRR0L = DIVIDE & 0xFF;
	UBRR0L = DIVIDE >> 8;
}

void usart0_write(char *s){
	UCSR0B |= (1<<TXEN0);
	UCSR0A |= (1<<TXC0);
	for(;*s;s++){
		UDR0 = *s;
		while(!(UCSR0A & (1<<UDRE0)));
	}
	UCSR0B &= ~(1<<TXEN0);
	while((UCSR0B & (1<<TXEN0)));
}

int main(){

	usbInit();
	usart0_init();

	MCUCR = (1<<IVCE);	/*Move vector table to the Bootloader region.*/
	MCUCR = (1<<IVSEL);	/*Move vector table to the Bootloader region.*/
	
	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();
	sei();
	
	while(1){
		usart0_write(__DATE__""__TIME__"\r\n");
		updateUSB();
		_delay_ms(20);
	}
	
	return 0;
}