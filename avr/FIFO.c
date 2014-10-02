
#include "FIFO.h"
#include <stdlib.h>

void fifoInit(FIFO *fifo, uint16_t size){
	if(fifo->buffer){
		free(fifo->buffer);
	}
	fifo->buffer = (uint8_t *)malloc(size);
	fifo->head = fifo->tail = fifo->r = 0;
	fifo->size = size;
}

uint8_t fifoPush(FIFO *fifo, uint8_t data){
	if(fifoIsFull(fifo)){	/*FIFO full*/
		return -1;
	}
	
	fifo->buffer[fifo->tail] = data;
	fifo->tail++;
	if(fifo->tail == fifo->size){
		fifo->tail = 0;
		fifo->r ++;
	}
	return 0;
}

uint8_t fifoPop(FIFO *fifo){
	uint8_t data = fifo->buffer[fifo->head];
	fifo->head ++;
	if(fifo->head == fifo->size){
		fifo->head = 0;
		fifo->r --;
	}
	return data;
}

uint8_t fifoPopBack(FIFO *fifo){
	if(fifo->tail == 0){
		fifo->tail = fifo->size - 1;
		fifo->r --;
	}
	fifo->tail --;
	return fifo->buffer[fifo->tail];
}