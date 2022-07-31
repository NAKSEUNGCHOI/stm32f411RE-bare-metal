/**
 * i2c.h
 *	@brief header file for i2c
 * 	@author Nakseung Choi
 * 	@date 07-28-2022
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

void I2C1_byteRead(char saddr, char maddr, char* data);
void I2C1_burstRead(char saddr, char maddr, int n, char* data);
void I2C1_burstWrite(char saddr, char maddr, int n, char* data);

#endif /* INC_I2C_H_ */
