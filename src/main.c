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
	lcdInit();
	sei();
	GPIOB->DDR |= (1<<0) | (1<<5) | (1<<3);
	//GPIOB->DDR &= ~(1<<4);
//	GPIOC->DDR |= (1<<4) | (1<<5);
//	GPIOC->PORT |= (1<<4) | (1<<5);
	//uint16_t length = sprintf(str,"MCUCR=%02x\r\n",MCUCR);
	//serialWriteBlocking(&Serial0, length, str);
//	serialWriteBlocking(&Serial0, 7, "Init\r\n");
//	SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR0);
	_delay_ms(200);
	uint16_t counter = 0;
	while(1){
		GPIOB->PIN |= 0x01;
		lcdClear();
		//lcdPrint("a");
		avrprintf(&LCD, "%d", counter++);
		_delay_ms(200);
	}
	return 0;
}
