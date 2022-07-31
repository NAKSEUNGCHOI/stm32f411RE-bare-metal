/**
 * MPU6050.c
 *	@brief source file for MPU6050
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
/**
 * void MPU6050_read_address (uint8_t reg)
 * @brief read address.
 */
void MPU6050_read_address (uint8_t reg){
	//GPIOA->ODR = LED_PIN;
	I2C1_byteRead(DEVICE_ADDR, reg, &data);

}
/**
 * void MPU6050_write(uint8_t reg, char value)
 * @brief write value to address.
 */
void MPU6050_write(uint8_t reg, char value){
	char data[1];
	data[0] = value;

	I2C1_burstWrite(DEVICE_ADDR, reg, 1, data);

}
/**
 * void MPU6050_read_values(uint8_t reg)
 * @brief read data from the register
 */
void MPU6050_read_values(uint8_t reg){
	I2C1_burstRead(DEVICE_ADDR, reg, 6, (char*)data_rec);
}
/*
 * void MPU6050_init(void)
 * @brief MPU6050 init
 * @step followed:
 *
 * 1. Enable I2C
 * 2. Read WHO_AM_I, this should return 0x68 or 104 in decimal
 * 3. if the data returned is equal to 0x68 or 104 in decimal:
 * 4. Wakes up the device
 * 5. Set DATA RATE of 1KHz by writing SMPLRT_DIV register
 * 6. Set data format range to +-2g
 * 7. Set data format range to +-250
 */
void MPU6050_init(void){

	/*1. Enable I2C*/
	I2C1_init();

	/*2. Read WHO_AM_I, this should return 0x68 or 104 in decimal*/
	MPU6050_read_address(WHO_AM_I_R);

	/*3. if the data returned is equal to 0x68 or 104 in decimal: */
	if(data == 104){

		/*4. Wakes up the device*/
		MPU6050_write(PWR_MGMT_1_R, 0);

		/*5. Set DATA RATE of 1KHz by writing SMPLRT_DIV register*/
		MPU6050_write(SMPLRT_DIV_R, 0x07);
		//GPIOA->ODR = LED_PIN;

		/*6. Set data format range to +-2g*/
		MPU6050_write(ACCEL_CONFIG_R, 0b00);

		/*7. Set data format range to +-250*/
		MPU6050_write(GYRO_CONFIG_R, 0b00);
	}


}
