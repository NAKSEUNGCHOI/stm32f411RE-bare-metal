/**
 * spi.h
 *	@brief header file for spi
 *  @author Nakseung Choi
 *  @date 07-30-2022
 *
 * PA5 -> CLK
 * PA6 -> MISO
 * PA7 -> MOSI
 * PA9 -> slave select
 */
#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f4xx.h"
#include <stdint.h>

void spi1_gpio_init(void);
void spi1_config(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uinit32_t size);
void cs_enable(void);
void cs_disable(void);

#endif /* INC_SPI_H_ */
