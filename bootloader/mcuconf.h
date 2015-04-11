
#ifndef __MCUCONF_H__
#define __MCUCONF_H__

#include <stdint.h>
#include <stdbool.h>

/*Serial Config*/
#define MCUCONF_USE_SERIAL				true
#define MCUCONF_USE_SERIAL_ASYNC 		true
#define MCUCONF_SERIAL_TX_BUFFER_SIZE	64

/*LCD Driver Config*/
#define MCUCONF_USE_LCD					true
#define MCUCONF_LCD_USE_BUSY_CHECK		true			/*Note: If you have connected RW to the MCU, you must say true here*/
#define MCUCONF_LCD_E_PORT				GPIOB
#define MCUCONF_LCD_E_PIN				1
#define MCUCONF_LCD_RS_PORT				GPIOB
#define MCUCONF_LCD_RS_PIN				2
#define MCUCONF_LCD_RW_PORT				GPIOD
#define MCUCONF_LCD_RW_PIN				5
#define MCUCONF_LCD_DATA_PORT			GPIOC
#define MCUCONF_LCD_DATA_OFFSET			0

/*Printf utilites*/
#define MCUCONF_USE_PRINTF				true
#define MCUCONF_PRINTF_USE_FLOAT		false

#endif /*__MCUCONF_H__*/