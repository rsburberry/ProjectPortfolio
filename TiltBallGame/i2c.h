#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

// Initalise the i2c Port.
void i2c_init(void);

// Read a register from the gyroscope over i2c.
uint8_t gyro_readRegister(uint8_t register_number);

// Write a value to a register on the gyroscope over i2c.
void gyro_writeRegister(uint8_t register_number, uint8_t value);

#endif