/*
 * lps25hb.c
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#include "lps25hb.h"

uint8_t lps25hb_address = LPS25HB_DEVICE_ADDRESS0;

// LPS25HB_read_byte implementation
uint8_t LPS25HB_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, lps25hb_address, 0));
}

// LPS25HB_write_byte implementation
void LPS25HB_write_byte(uint8_t data, uint8_t register_addr, uint8_t slave_addr, uint8_t flag_read)
{
	i2c_master_write(data, register_addr, slave_addr, flag_read);
}

// LPS25HB_read_array implementation
void LPS25HB_read_array(uint8_t* data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, lps25hb_address, 1);
}

// LPS25HB_get_pressure implementation
float LPS25HB_get_pressure()
{
	uint8_t pressure[3] = {0};
	LPS25HB_read_array(&pressure[0], LPS25HB_PRESSURE_OUT_H, 1);
	LPS25HB_read_array(&pressure[1], LPS25HB_PRESSURE_OUT_L, 1);
	LPS25HB_read_array(&pressure[2], LPS25HB_PRESSURE_OUT_XL, 1);

	float pressure_real = ((pressure[0] * 65536) + (pressure[1] * 256) + pressure[2]) / 4096.0;

	return pressure_real;
}

uint8_t LPS25HB_Init()
{
	uint8_t status = 1;

	//LPS25hb_ACC_ON;
	LL_mDelay(100);

	uint8_t val = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRESS);

	if (val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		lps25hb_address = LPS25HB_DEVICE_ADDRESS1;
		val = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRESS);
		if (val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}
	return status;
}
