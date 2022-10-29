/*
 * lps25hb.c
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#include "../Inc/lps25hb.h"

uint8_t LPS25HB_Read_Byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_, LPS25HB_DEVICE_ADDRESS0, 0));
}

void LPS25HB_Init()
{
	uint8_t who_am_i = LPS25HB_Read_Byte(LPS25HB_WHO_AM_I_ADDRESS);

	if (who_am_i == LPS25HB_WHO_AM_I_VALUE)
	{
		// Device cold connect
	}
}
