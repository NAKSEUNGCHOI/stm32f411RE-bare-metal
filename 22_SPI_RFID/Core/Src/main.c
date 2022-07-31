/**
 *	main.c
 *	@brief running MPU6050 i2c bare-metal
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "MPU6050.h"
#include "i2c.h"
#include "spi.h"

int16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW, Gyro_X_RAW, Gyro_Y_RAW, Gyro_Z_RAW;
float Ax, Ay, Az, Gx, Gy, Gz;

extern uint8_t data_rec[6]; //buffer to store data

int main(void){
	/*1. initializes MPU6050*/
 	MPU6050_init();

	while(1){
		/*2. read accel values.*/
		MPU6050_read_values(ACCEL_XOUT_H_REG);

		Accel_X_RAW = (int16_t)(data_rec[0] << 8 | data_rec[1]);
		Accel_Y_RAW = (int16_t)(data_rec[2] << 8 | data_rec[3]);
		Accel_Z_RAW = (int16_t)(data_rec[4] << 8 | data_rec[5]);

		Ax = (Accel_X_RAW/16384.0);
		Ay = (Accel_Y_RAW/16384.0);
		Az = (Accel_Z_RAW/16384.0);

		/*3. read gyro values.*/
		MPU6050_read_values(GYRO_XOUT_H_REG);

		Gyro_X_RAW = (int16_t)(data_rec[0] << 8 | data_rec[1]);
		Gyro_Y_RAW = (int16_t)(data_rec[2] << 8 | data_rec[3]);
		Gyro_Z_RAW = (int16_t)(data_rec[4] << 8 | data_rec[5]);

		Gx = (Gyro_X_RAW/131.0);
		Gy = (Gyro_Y_RAW/131.0);
		Gz = (Gyro_Z_RAW/131.0);
	}


}


