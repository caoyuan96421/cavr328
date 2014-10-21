#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GPIO.h"
#include "Serial.h"
#include "usbdrv.h"
#include "LCD.h"
#include "avrprintf.h"
#include <stdio.h>
#include <avr/cpufunc.h>
char str[128]=__DATE__""__TIME__"\r\n";
//extern void updateUSB();

uint16_t in_data, out_data = 0x0F0F;
int main(){
//	serialInit(&Serial0, 9600);
//	usbInit();
	lcdInit();
//	lcdSetCursor(1,0);
/*	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();*/
	sei();
	GPIOB->DDR |= (1<<0) | (1<<5) | (1<<3);
	//GPIOB->DDR &= ~(1<<4);
	GPIOC->DDR |= (1<<4) | (1<<5);
	GPIOC->PORT |= (1<<4) | (1<<5);
	//uint16_t length = sprintf(str,"MCUCR=%02x\r\n",MCUCR);
	//serialWriteBlocking(&Serial0, length, str);
//	serialWriteBlocking(&Serial0, 7, "Init\r\n");
	SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR0);
	uint8_t counter = 0;
	while(1){
		//uint16_t length = sprintf(str,"%d\r\n",counter++);
		//serialWrite(&Serial0, strlen(str), str);
		//serialWrite(&Serial0, length, str);
		//updateUSB();
		//_delay_ms(100);
		GPIOB->PIN |= 0x01;
		//lcdPrint("Hello World!");
		lcdClear();
		
		
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
		
		avrprintf(&LCD, "Data=%04x", in_data);
		_delay_ms(100);
	}
	return 0;
}
