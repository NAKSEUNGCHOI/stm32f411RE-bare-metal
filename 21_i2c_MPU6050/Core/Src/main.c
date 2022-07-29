#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "MPU6050.h"
#include "i2c.h"

int16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW;
float Ax, Ay, Az;

extern uint8_t data_rec[6];

int main(void){
	MPU6050_init();

	while(1){
		MPU6050_read_values(ACCEL_XOUT_H_REG);

		Accel_X_RAW = (int16_t)(data_rec[0] << 8 | data_rec[1]);
		Accel_Y_RAW = (int16_t)(data_rec[2] << 8 | data_rec[3]);
		Accel_Z_RAW = (int16_t)(data_rec[4] << 8 | data_rec[5]);

		Ax = (Accel_X_RAW/16384.0);
		Ay = (Accel_Y_RAW/16384.0);
		Az = (Accel_Z_RAW/16384.0);
	}


}


