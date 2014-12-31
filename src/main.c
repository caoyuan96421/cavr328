#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GPIO.h"
#include "Serial.h"
#include "usbdrv.h"
#include "LCD.h"
#include "avrprintf.h"
#include <stdio.h>
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
	GPIOC->DDR |= (1<<4);
	GPIOC->PORT |= (1<<4);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR0);
	uint8_t counter = 0;
	while(1){
		usbPoll();
		
		//GPIOC->PORT &= ~(1<<5);// Parallel load
		//GPIOC->PORT |= (1<<5);
		
		SPDR = (out_data >> 8);	
		while((SPSR & (1<<SPIF)) == 0);
		
		GPIOC->PORT &= ~(1<<4);
		GPIOC->PORT |= (1<<4);// Update Output
		
		if(counter++ % 10==0){
			lcdClear();
			avrprintf(&LCD, "Out=%04x", out_data);
		}
		_delay_ms(10);
		GPIOB->PORT &= ~0x01;
	}
	return 0;
}
