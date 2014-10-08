
#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

typedef struct {
	volatile uint8_t PIN;
	volatile uint8_t DDR;
	volatile uint8_t PORT;
} GPIO_struct;

#define GPIOB ((GPIO_struct *)_SFR_ADDR(PINB))
#define GPIOC ((GPIO_struct *)_SFR_ADDR(PINC))
#define GPIOD ((GPIO_struct *)_SFR_ADDR(PIND))

#endif /*__GPIO_H__*/