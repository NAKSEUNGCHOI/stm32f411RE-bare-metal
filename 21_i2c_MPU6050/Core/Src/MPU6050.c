/*
 * MPU6050.c
 *	@brief source file for i2c
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */

#include "stm32f4xx.h"
#include "MPU6050.h"
#define GPIOAEN 			(1U << 0)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5

char data;

uint8_t data_rec[6];

void MPU6050_read_address (uint8_t reg){
	//GPIOA->ODR = LED_PIN;
	I2C1_byteRead(DEVICE_ADDR, reg, &data);


}
void MPU6050_write(uint8_t reg, char value){
	char data[1];
	data[0] = value;

	I2C1_burstWrite(DEVICE_ADDR, reg, 1, data);

}
void MPU6050_read_values(uint8_t reg){
	I2C1_burstRead(DEVICE_ADDR, reg, 6, (char*)data_rec);
}

void MPU6050_init(void){
	/*Enable I2C*/
	I2C1_init();

	/*Testing */
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER &= ~(1U << 11);
	GPIOA->MODER |= (1U << 10);

	/*Read the DEVID, this should return 0x68 or 104 in decimal something?*/
	MPU6050_read_address(WHO_AM_I_R);
	//GPIOA->ODR = LED_PIN;
	if(data == 104){
		GPIOA->ODR = LED_PIN;
	}
	//////////////////////////////////////////////////////////////////
	// 7-28-2022 Debugging done until here.
	//////////////////////////////////////////////////////////////////

	/*Set DATA RATE of 1KHz by writing SMPLRT_DIV register*/
	MPU6050_write(SMPLRT_DIV_R, 0x07);

	/*Set data format range to +-4g*/
	MPU6050_write(ACCEL_CONFIG_R, 0b00);

	/*Set DATA RATE of 1KHz by writing SMPLRT_DIV register*/
	//MPU6050_write(GYRO_CONFIG_R, 0x00);

	/*Reset all bits*/

	/*Configure power control measure bit*/
}
