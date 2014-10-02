

#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>

typedef struct {
	uint8_t *buffer;
	uint16_t head, tail, r, size;
} FIFO;

#define fifoIsEmpty(fifo) ((fifo)->head == (fifo)->tail && (fifo)->r == 0)
#define fifoIsFull(fifl) ((fifo)->head == (fifo)->tail && (fifo)->r == 1)

void fifoInit(FIFO *,uint16_t size);
uint8_t fifoPush(FIFO *,uint8_t data);
uint8_t fifoPop(FIFO *);	/*Note: pop operations don't check if the buffer is empty. Please do that manually.*/
uint8_t fifoPopBack(FIFO *);

#endif /*__FIFO_H__*/