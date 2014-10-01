#include <avr/io.h>
#include "Serial.h"
const char str[] = "Hello World!\r\n";
int main(){
	serialInit(&Serial0, 9600);
	while(1){
		serialWriteBlock(&Serial0, sizeof(str), str);
	}
	return 0;
}