#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "bootloader.h"

#define BAUD 9600
#define DIVIDE ((((uint32_t)F_CPU) >> 4) / BAUD - 1)


FUSEMEM __fuse_t fusedata = {
	0xFF,
	0xD8,		// BOOTRST = 0 (enabled)
	0xFF
}; 

void usart0_init(){
	UBRR0L = DIVIDE & 0xFF;
	UBRR0H = DIVIDE >> 8;
}

void usart0_write(const char *s){
	UCSR0B |= (1<<TXEN0);
	UCSR0A |= (1<<TXC0);
	for(;*s;s++){
		UDR0 = *s;
		while(!(UCSR0A & (1<<UDRE0)));
	}
	UCSR0B &= ~(1<<TXEN0);
	while((UCSR0B & (1<<TXEN0)));
}

#define CONV_HEX(x) {if((x) > 57) (x) += ('A' - '9' - 1);}
void usart0_write_hex(uint8_t data){
	char buf[3];
	buf[0]=(data>>4) + '0';
	CONV_HEX(buf[0]);
	buf[1]=(data&0x0F) + '0';
	CONV_HEX(buf[1]);
	buf[2]='\0';
	usart0_write(buf);
}

void usart0_write_hex_word(uint16_t data){
	char buf[5],i;
	for(i=3;i>=0;i--){
		buf[i]=(data&0x0F) + '0';data>>=4;
		CONV_HEX(buf[i]);
	}
	buf[4]='\0';
	usart0_write(buf);
}

int main(){

	usbInit();
	usart0_init();
	DDRB |= 0x01; /*Enable LED*/

	MCUCR = (1<<IVCE);	/*Move vector table to the Bootloader region.*/
	MCUCR = (1<<IVSEL);	/*Move vector table to the Bootloader region.*/
	
	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();
	usart0_write("\r\nBootloader\r\n");
	sei();
	
	while(1){
		usbPoll();
	}
	
	return 0;
}