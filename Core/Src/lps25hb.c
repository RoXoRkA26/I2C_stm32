/*
 * lps25hb.c
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#include "lps25hb.h"

uint8_t address = LPS25HB_DEVICE_ADDRESS0;

uint8_t LPS25HB_Read_Byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address, 0));
}

void LPS25HB_Write_Byte(uint8_t data, uint8_t register_addr, uint8_t slave_addr, uint8_t flag_read)
{
	i2c_master_write(data, register_addr, slave_addr, flag_read);
}

uint8_t LPS25HB_Init()
{
	uint8_t status = 1;

	//LPS25hb_ACC_ON;
	LL_mDelay(100);

	uint8_t val = LPS25HB_Read_Byte(LPS25HB_WHO_AM_I_ADDRESS);

	if (val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		address = LPS25HB_DEVICE_ADDRESS1;
		val = LPS25HB_Read_Byte(LPS25HB_WHO_AM_I_ADDRESS);
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
