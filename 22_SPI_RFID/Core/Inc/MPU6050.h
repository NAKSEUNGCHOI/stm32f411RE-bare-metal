/**
 * MPU6050.h
 *	@brief header file for MPU6050
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */


#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "i2c.h"
#include <stdint.h>

#define DEVID_R					0x00
#define SMPLRT_DIV_R				(0x19)	//Data output rate or sample rate
#define DEVICE_ADDR				(0xD0) // MPU6050 default i2c address w/ 0xD0 high
#define GYRO_CONFIG_R			(0x1B)
#define ACCEL_CONFIG_R			(0x1C)
#define ACCEL_XOUT_H_REG 		(0x3B)
#define TEMP_OUT_H_REG 			(0x41)
#define GYRO_XOUT_H_REG 			(0x43)
#define PWR_MGMT_1_R				(0x6B)	//this wakes the sensor up by writing 0x00 to the power management 1 register.

#define GYRO_XOUT_H				(0x43)
#define GYRO_XOUT_L				(0x44)
#define GYRO_YOUT_H					(0x45)
#define GYRO_YOUT_L					(0x46)
#define GYRO_ZOUT_H					(0x47)
#define GYRO_ZOUT_L					(0x48)

/*Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV) 8MHz / 1+7kHz = 1kHz */
/*For example, use SMPLRT_DIV as 7 to get the sample rate of 1khz. */
#define WHO_AM_I_R				(0x75)

typedef enum {
  MPU6050_RANGE_2_G = 0b00,  ///< +/- 2g (default value)
  MPU6050_RANGE_4_G = 0b01,  ///< +/- 4g
  MPU6050_RANGE_8_G = 0b10,  ///< +/- 8g
  MPU6050_RANGE_16_G = 0b11, ///< +/- 16g
} mpu6050_accel_range_t;

void MPU6050_init(void);
void MPU6050_read_values(uint8_t reg);


#endif /* INC_MPU6050_H_ */
