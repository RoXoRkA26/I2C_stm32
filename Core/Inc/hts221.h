/*
 * hts221.h
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#include "main.h"
#include "i2c.h"

#ifndef INC_HTS221_H_
#define INC_HTS221_H_

#define HTS221_DEVICE_ADDRESS			0xBE

#define HTS221_WHO_AM_I_ADDRESS			0x0F
#define HTS221_WHO_AM_I_VALUE			0xBC

#define HTS221_AVG_CONFIG				0x10
#define HTS221_CONTROL_REG				0x20

#define HTS221_HUMIDITY_OUT_L			0x28
#define HTS221_HUMIDITY_OUT_H			0x29

#define HTS221_TEMPERATURE_OUT_L		0x2A
#define HTS221_TEMPERATURE_OUT_H		0x2B

#define HTS221_CALIBRATION_REG1			0x30
#define HTS221_CALIBRATiON_REG2			0x3F

// Function prototypes
uint8_t HTS221_Init();
void HTS221_write_byte(uint8_t data, uint8_t register_addr, uint8_t slave_addr);
uint8_t HTS221_read_byte(uint8_t reg_addr);
void HTS221_read_array(uint8_t* data, uint8_t reg, uint8_t length);
float HTS221_get_temperature();
float HTS221_get_humidity();

#endif /* INC_HTS221_H_ */
