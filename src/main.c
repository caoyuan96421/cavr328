#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Serial.h"
#include "usbdrv.h"
const char str[] = "Hello World!\r\n";
extern void updateUSB();

int main(){
#ifdef BOOTLOADER
	MCUCR |= (1<<IVSEL);	/*Move vector table to the Bootloader region.*/
#endif
	serialInit(&Serial0, 9600);
	//usbInit();
	//sbDeviceDisconnect();
	//_delay_ms(100);
	//usbDeviceConnect();
	//sei();
	while(1){
		serialWriteBlock(&Serial0, sizeof(str), str);
		//updateUSB();
		_delay_ms(100);
	}
	return 0;
}
