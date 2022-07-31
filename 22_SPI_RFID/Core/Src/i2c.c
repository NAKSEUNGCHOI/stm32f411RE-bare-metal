/**
 * i2c.c
 *	@brief source file for i2c
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */


#include "stm32f4xx.h"
#include "i2c.h"
#include <stdio.h>

#define I2C_100KHZ 						80; // I2C to standard mode; refer to the reference manual for calculation.
#define SD_MODE_MAX_RISE_TIME				17; // same as above.
#define GPIOAEN 			(1U << 0)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5

/**
 * void I2C1_init(void)
 * @brief Initialize I2C1
 * @step followed:
 *
 * 1. Enable clock access to GPIOB
 * 2. Set PB8 and PB9 mode to alternate function
 * 3. Set PB8 and PB9 output type to open drain
 * 4. Enable pull-up register for PB8 and PB9
 * 5. Enable clock access to I2C1
 * 6. Enter the reset mode
 * 7. Come out of the reset mode
 * 8. Set the peripheral clock frequency
 * 9. Set I2C to standard mode, 100kHz clock. refer to the reference manual for calculation.
 * 10. Set rise time
 * 11. Enable I2C1 module.
 * ***************
 * Pin-out       *
 * PB8 ----- SCL *
 * PB9 ----- SDA *
 * ***************
 */
void I2C1_init(void){
	/*1. Enable clock access to GPIOB */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	/*2. Set PB8 and PB9 mode to alternate function */
	GPIOB->MODER |= (1U << 19) | (1U << 17);
	GPIOB->MODER &= ~(1U << 18);
	GPIOB->MODER &= ~(1U << 16);

	/*3. Set PB8 and PB9 output type to open drain */
	GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;

	/*4. Enable pull-up register for PB8 and PB9 */
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0;
	GPIOB->PUPDR &= ~(1U << 19); // try ~GPIO_PUPDR_PUPDR8_1
	GPIOB->PUPDR &= ~(1U << 17);

	/*5. Enable clock access to I2C1 */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	/*Bits (3:2:1:0) = 0:1:0:0 --> AF4 for pin PB8;  Bits (7:6:5:4) = 0:1:0:0 --> AF4 for pin PB9*/
	GPIOB->AFR[1] |= (4<<0) | (4<<4);

	/*6. Enter the reset mode */
	I2C1->CR1 |= I2C_CR1_SWRST;

	/*7. Come out of the reset mode*/
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	/*8. Set the peripheral clock frequency */
	I2C1->CR2 |= I2C_CR2_FREQ_4;

	/*9. Set I2C to standard mode, 100kHz clock. refer to the reference manual for calculation. */
	I2C1->CCR = I2C_100KHZ; // 100kHz

	/*10. Set rise time*/
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/*11. Enable I2C1 module. */
	I2C1->CR1 |= I2C_CR1_PE;

}
/**
 * void I2C1_byteRead(char saddr, char maddr, char* data)
 * @brief initialize I2C read function
 * @step followed:
 *
 * 1. Wait until bus is not busy
 * 2. Enable Start bit
 * 3. Wait until start flag is set.
 * 4. Transmit the slave address + Write 0 at bit 0
 * 5. wait until address flag is set
 * 6. Clear status registers by reading them
 * 7. send memory address
 * 8. wait until transmitter gets empty
 * 9. Enable re-start bit
 * 10. wait until start flag is set
 * 11. transmit slave address + Read 1 at bit 0
 * 12. wait until address flag is set
 * 13. Disable Acknowledge
 * 14. Clear address flag
 * 15. generate stop condition
 * 16. wait until stop bit is set
 * 17 Read data from DR
 *
 */

void I2C1_byteRead(char saddr, char maddr, char* data){

	volatile int temp = 0; // This is going to be used to clear status registers by reading them.

	/*1. Wait until bus is not busy (while it is busy, get stuck in the while loop.)*/
	while(I2C1->SR2 & I2C_SR2_BUSY){}

	/*2. Enable Start bit*/
	I2C1->CR1 |= I2C_CR1_START;

	/*3. Wait until start flag is set. */
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*4. Transmit the slave address + Write 0 at bit 0 */
	I2C1->DR = saddr;

	/*5. wait until address flag is set */
	while(!(I2C1->SR1 & (I2C_SR1_ADDR))){}

	/*6. Clear status registers by reading them*/
	temp = I2C1->SR2;

	/*7. send memory address */
	I2C1->DR = maddr;

	/*8. wait until transmitter gets empty*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*9. Enable re-start bit */
	I2C1->CR1 |= I2C_CR1_START;

	/*10. wait until start flag is set */
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*11. transmit slave address + Read 1 at bit 0 */
	I2C1->DR = saddr + 0x01;

	/*12. wait until address flag is set */
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*13. Disable Acknowledge */
	I2C1->CR1 &= ~I2C_CR1_ACK;

	/*14. Clear address flag*/
	temp = I2C1->SR2 | I2C1->SR1;

	/*15. generate stop condition */
	I2C1->CR1 |= I2C_CR1_STOP;

	/*16. wait until stop bit is set*/
	while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

	/*17 Read data from DR*/
	*data++ = I2C1->DR;

}
/**
 * void I2C1_burstRead(char saddr, char maddr, int n, char* data)
 * @brief intializes burst read
 * @param saddr slave address
 * @param maddr memory address
 * @param n number of byte
 * @param data pointer to store to data that you want to read.
 * @step followed:
 *
 * 1. Enable Start bit
 * 2. Wait until start flag is set.
 * 3. Transmit the slave address + Write 0 at bit 0
 * 4. wait until address flag is set
 * 5. Clear status registers by reading them
 * 6. wait until transmitter gets empty
 * 7. send memory address
 * 8. wait until transmitter gets empty
 * 9. Enable re-start bit
 * 10. wait until start flag is set
 * 11. transmit slave address + Read 1 at bit 0
 * 12. wait until address flag is set
 * 13. Clear address flag
 * 14. Enable Acknowledge bit
 * 15. If one byte is left,
 *
 *     1. disable Acknowledge bit
 *     2. Enable stop bit
 *     3. wait for RXNE flag to be set
 *     4. read data from DR
 *     5. break when n = 1
 *
 * 16. if data is more than one byte, keep reading.
 *
 * 	   1. wait for RXNE flag to be set
 * 	   2. Read data from DR
 * 	   3. decrement until n = 1 such that codes in (n = 1U) runs and breaks out of the loop.
 */
void I2C1_burstRead(char saddr, char maddr, int n, char* data){
	volatile int temp = 0;

	/*1. Wait until bus is not busy (while it is busy, get stuck in the while loop.)*/
	//while(I2C1->SR2 & I2C_SR2_BUSY){}

	/*1. Enable Start bit*/
	I2C1->CR1 |= I2C_CR1_START;

	/*2. Wait until start flag is set. */
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*3. Transmit the slave address + Write 0 at bit 0 */
	I2C1->DR = saddr;

	/*4. wait until address flag is set */
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*5. Clear status registers by reading them*/
	temp = I2C1->SR2 | I2C1->SR1;

	/*6. wait until transmitter gets empty*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*7. send memory address */
	I2C1->DR = maddr;

	/*8. wait until transmitter gets empty*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*9. Enable re-start bit */
	I2C1->CR1 |= I2C_CR1_START;

	/*10. wait until start flag is set */
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*11. transmit slave address + Read 1 at bit 0 */
	I2C1->DR = saddr + 0x01;

	/*12. wait until address flag is set */
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*13. Clear address flag*/
	temp = I2C1->SR2 | I2C1->SR1;

	/*14. Enable Acknowledge bit*/
	I2C1->CR1 |= I2C_CR1_ACK;

	while(n > 0U){

		/*15. check if one byte is left*/
		if(n == 1U){

			/*1. disable Acknowledge bit*/
			I2C1->CR1 &= ~I2C_CR1_ACK;

			/*2. Enable stop bit*/
			I2C1->CR1 |= I2C_CR1_STOP;

			/*3. wait for RXNE flag to be set*/
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

			/*4. read data from DR*/
			*data++ = I2C1->DR;

			/*5. break when n = 1*/
			break;

		/*16. if data is more than one byte, keep reading. */
		}else{
			/*1. wait for RXNE flag to be set*/
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

			/*2. Read data from DR*/
			(*data++) = I2C1->DR;

			/*3. decrement until n = 1 such that codes in (n = 1U) runs and breaks out of the loop.  */
			n--;
		}
	}
}
/**
 * void I2C1_burstWrite(char saddr, char maddr, int n, char* data)
 * @brief intializes I2C burst write
 * @param saddr slave address
 * @param maddr memory address
 * @param n number of byte
 * @param data pointer to hold the data you want to write to the slave.
 * @step followed:
 *
 * 1. Enable Start bit
 * 2. Wait until start flag is set.
 * 3. Transmit the slave address + Write 0 at bit 0
 * 4. wait until address flag is set
 * 5. Clear address flag
 * 6. wait until transmitter gets empty
 * 7. send memory address
 * 8. wait until data register is empty
 * 9. Transmit memory address
 * 10. Wait until BTF (byte transfer finished) is set
 */
void I2C1_burstWrite(char saddr, char maddr, int n, char* data){
	volatile int temp = 0;

	/*1. Enable Start bit*/
	I2C1->CR1 |= I2C_CR1_START;

	/*2. Wait until start flag is set. */
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*3. Transmit the slave address + Write 0 at bit 0 */
	I2C1->DR = saddr;

	/*4. wait until address flag is set */
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*5. Clear address flag*/
	temp = I2C1->SR2 | I2C1->SR1;

	/*6. wait until transmitter gets empty*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*7. send memory address */
	I2C1->DR = maddr;

	/*8. Wait until BTF (byte transfer finished) is set */
	while(!(I2C1->SR1 & I2C_SR1_BTF)){}

	for(int i = 0; i < n; i++){

		/*9. wait until data register is empty*/
		while(!(I2C1->SR1 & I2C_SR1_TXE)){}

		/*10. Transmit memory address */
		I2C1->DR = *data++;

		/*11. Wait until BTF (byte transfer finished) is set */
		while(!(I2C1->SR1 & I2C_SR1_BTF)){}

	}
	//count++;
}
