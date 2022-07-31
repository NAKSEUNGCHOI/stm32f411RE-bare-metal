/**
 * spi.c
 *	@brief source file for spi
 *  @author Nakseung Choi
 *  @date 07-30-2022
 *
 * PA5 -> CLK
 * PA6 -> MISO
 * PA7 -> MOSI
 * PA9 -> slave select
 */


#include "spi.h"
#include "stm32f4xx.h"

/**
 * void spi1_gpio_init(void)
 * @brief initialize GPIO pins for spi1
 * @step followed:
 *
 * 1. Enable Clock access to GPIOA
 * 2. Set PA5, PA6, and PA7 modes to alternate function
 * 3. Set PA 9 (SS) as a general output pin
 * 4. Set PA5, PA6, and PA7 alternate function type to SPI1
 */
void spi1_gpio_init(void){

	/*1. Enable Clock access to GPIOA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*2. Set PA5, PA6, and PA7 modes to alternate function*/
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;

	/*3. Set PA 9 (SS) as a general output pin*/
	GPIOA->MODER |= GPIO_MODER_MODE9_0;

	/*4. Set PA5, PA6, and PA7 alternate function type to SPI1*/
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2;

}
/**
 * void spi1_config(void)
 * @brief configures spi1
 * @step followed:
 *
 * 1. Enable Clock access to SPI1
 * 2. Set clock to fPCLK/4
 * 3. Set Clock Polarity and Clock phase
 * 4. Enable full-duplex.
 * 5. Set MSB first
 * 6. Set the MCU to MASTER mode.
 * 7. Set data format to 8 bit
 * 8. Select Software slave management by setting SSM to 1 and SSI to 1
 * 9. Enable SPI
 */
void spi1_config(void){

	/*1. Enable Clock access to SPI1*/
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/*2. Set clock to fPCLK/4 */
	SPI1->CR1 |= SPI_CR1_BR_0;

	/*3. Set Clock Polarity and Clock phase*/
	SPI1->CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL;

	/*4. Enable full-duplex. */
	SPI1->CR1 |= SPI_CR1_RXONLY;

	/*5. Set MSB first*/
	SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);

	/*6. Set the MCU to MASTER mode. */
	SPI1->CR1 |= SPI_CR1_MSTR;

	/*7. Set data format to 8 bit*/
	SPI1->CR1 &= ~(SPI_CR1_DFF);

	/*8. Select Software slave management by setting SSM to 1 and SSI to 1*/
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

	/*9. Enable SPI*/
	SPI1->CR1 |= SPI_CR1_SPE;

}
/**
 * void spi1_transmit(uint8_t *data, uint32_t size)
 * @brief
 * @step followed:
 *
 * 1. wait until TXE flag is set
 * 2. Write the data to the data register
 * 3. wait until TXE flag is set (Tx buffer is empty)
 * 4. wait for BUSY flag to reset
 * 5. Clear OVR flag
 *
 */
void spi1_transmit(uint8_t *data, uint32_t size){
	uint32_t i = 0;
	uint8_t temp;

	while(i < size){

		/*1. wait until TXE flag is set */
		while(!(SPI1->SR & SPI_SR_TXE)){}

		/*2. Write the data to the data register */
		SPI1->DR = data[i];
		i++;

	}
	/*3. wait until TXE flag is set (Tx buffer is empty)*/
	while(!(SPI1->SR & SPI_SR_TXE)){}

	/*4. wait for BUSY flag to reset */
	while(SPI1->SR & SPI_SR_BUSY){}

	/*5. Clear OVR flag (both DR and SR; refer to reference manual)  */
	temp = SPI1->DR;
	temp = SPI1->SR;

}
/**
 * void spi1_receive(uint8_t *data, uinit32_t size)
 * @brief init receive function
 * @param data pointer to the data buffer
 * @param size size of the data
 * @step followed:
 *
 * 1. Send dummy data
 * 2. wait for RXNE flag to be set
 * 3. Read data from data register
 * 4. Decrement size
 */
void spi1_receive(uint8_t *data, uinit32_t size){
	while(size){

		/*1. Send dummy data */
		SPI1->DR = 0;

		/*2. wait for RXNE flag to be set*/
		while(!(SPI1->SR & SPI_SR_RXNE)){}

		/*3. Read data from data register */
		*data++ = (SPI1->DR);

		/*4. Decrement size */
		size--;

	}
}
/**
 * void cs_enable(void)
 * @brief enable cs
 * @note cs is active low, so set to LOW to enable.
 */
void cs_enable(void){

	/* 1. set cs to LOW to enable (active low) */
	GPIOA->ODR &= ~(GPIO_ODR_OD9);

}
/**
 * void cs_disable(void)
 * @brief disable cs
 * @note cs is active low, so set to HIGH to disable.
 */
void cs_disable(void){

	/* 1. set cs to HIGH to disable (active low) */
	GPIOA->ODR |= GPIO_ODR_OD9;
}

