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
void LPS25HB_write_byte(uint8_t data, uint8_t register_addr, uint8_t slave_addr)
{
	i2c_master_write(data, register_addr, slave_addr, 0);
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
	LPS25HB_read_array(pressure, LPS25HB_PRESSURE_OUT_XL, 3);

	float pressure_real = ((pressure[2] * 65536) + (pressure[1] * 256) + pressure[0]) / 4096.0;

	return pressure_real;
}

// LPS25HB_get_reference_pressure
float LPS25HB_get_reference_pressure()
{
	return 0.0;
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

	// Set power-down mode to turn ON -> set PD to 1 to power on
	uint8_t reg_setup = LPS25HB_read_byte(LPS25HB_CONTROL_REG1);
	reg_setup |= (1 << 7);
	LPS25HB_write_byte(reg_setup, LPS25HB_CONTROL_REG1, lps25hb_address);
	// Set Output data rate register to 1 Hz -> 0b001
	reg_setup = LPS25HB_read_byte(LPS25HB_CONTROL_REG1);
	reg_setup |= (1 << 4);
	LPS25HB_write_byte(reg_setup, LPS25HB_CONTROL_REG1, lps25hb_address);

	reg_setup = LPS25HB_read_byte(LPS25HB_CONTROL_REG1);

	return status;
}
