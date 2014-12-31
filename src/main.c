#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GPIO.h"
#include "Serial.h"
#include "usbdrv.h"
#include "LCD.h"
#include "avrprintf.h"
#include <stdio.h>
char s[] = "AB";
uint16_t in_data;
uint16_t out_data = 0x0000;
int main(){
	serialInit(&Serial0, 38400);
	usbInit();
	lcdInit();
	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();
	sei();
	GPIOB->DDR |= (1<<0) | (1<<5) | (1<<3);
	GPIOC->DDR |= (1<<4) | (1<<5);
	GPIOC->PORT |= (1<<4) | (1<<5);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR0);
	uint8_t counter = 0;
	while(1){
		usbPoll();
		
		GPIOC->PORT &= ~(1<<5);// Parallel load
		GPIOC->PORT |= (1<<5);
		
		SPDR = (out_data >> 8);	
		while((SPSR & (1<<SPIF)) == 0);
		in_data = SPDR;
		SPDR = (out_data & 0xFF);	
		while((SPSR & (1<<SPIF)) == 0);
		in_data = (in_data << 8) | SPDR;
		
		GPIOC->PORT &= ~(1<<4);
		GPIOC->PORT |= (1<<4);// Update Output
		
		if(counter++ % 10==0){
			lcdClear();
			avrprintf(&LCD, "In=%04x,Out=%04x", in_data, out_data);
		}
		serialWriteBlocking(&Serial0,2,(uint8_t *)s);
		_delay_ms(10);
		GPIOB->PORT &= ~0x01;
	}
	return 0;
}
