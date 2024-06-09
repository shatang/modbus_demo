#ifndef __SPI1_H
#define __SPI1_H
#include "stm32f4xx_hal.h"


#define	W5500_CS_LOW		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)
#define	W5500_CS_HIGH	 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)


void spi1_init(void);
uint8_t spi1_readByte(void);
void spi1_writeByte(uint8_t fu8_buf);

#endif
