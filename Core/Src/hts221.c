/*
 * hts221.c
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#include "hts221.h"

uint8_t hts221_address = HTS221_DEVICE_ADDRESS;

// HTS221_read_byte implementation
uint8_t HTS221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, hts221_address, 0));
}

// HTS221_write_byte implementation
void HTS221_write_byte(uint8_t data, uint8_t register_addr, uint8_t slave_addr)
{
	i2c_master_write(data, register_addr, slave_addr, 0);
}

// HTS221_read_array implementation
void HTS221_read_array(uint8_t* data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, hts221_address, 1);
}

uint8_t HTS221_Init()
{
	uint8_t status = 1;

	// HTS221__ACC_ON;
	LL_mDelay(100);

	uint8_t val = HTS221_read_byte(HTS221_WHO_AM_I_ADDRESS);

	if (val != HTS221_WHO_AM_I_VALUE)
	{
		status = 0;
		return status;
	}


	return status;
}
