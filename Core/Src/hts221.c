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
void HTS221_read_array(uint8_t *data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, hts221_address, 1);
}

float HTS221_get_temperature()
{
	int16_t t0_degC_x8 = 0;
	int16_t t1_degC_x8 = 0;
	uint8_t t0_t1_msb = 0;
	int16_t t0_out = 0;
	int16_t t1_out = 0;

	// T0 temperature
	uint8_t t0_tmp = HTS221_read_byte(HTS221_T0_DEG_C_x8);
	t0_degC_x8 |= t0_tmp;

	uint8_t t0_out_tmp[2] = { 0 };
	HTS221_read_array(t0_out_tmp, HTS221_T0_OUT_L, 2);
	t0_out |= t0_out_tmp[0];
	t0_out |= (t0_out_tmp[1] << 8);

	// T1 temperature
	uint8_t t1_temp = HTS221_read_byte(HTS221_T1_DEG_C_x8);
	t1_degC_x8 |= t1_temp;

	uint8_t t1_out_tmp[2] = { 0 };
	HTS221_read_array(t1_out_tmp, HTS221_T1_OUT_L, 2);
	t1_out |= t1_out_tmp[0];
	t1_out |= (t1_out_tmp[1] << 8);

	// T_OUT temperature
	uint8_t t_out_tmp[2] = { 0 };
	HTS221_read_array(t_out_tmp, HTS221_TEMPERATURE_OUT_L, 2);
	int16_t t_out = 0;
	t_out |= t_out_tmp[0];
	t_out |= (t_out_tmp[1] << 8);

	// Temperature interpolation
	t0_t1_msb = HTS221_read_byte(HTS221_T1_T0_MSB);

	float T_DegC;
	float T0_degC = (t0_degC_x8 + (1 << 8) * (t0_t1_msb & 0x03)) / 8.0;
	float T1_degC = (t1_degC_x8 + (1 << 6) * (t0_t1_msb & 0x0C)) / 8.0; // Value is in 3rd and fourth bit, so we only need to shift this value 6 more bits.
	T_DegC = (T0_degC + (t_out - t0_out) * (T1_degC - T0_degC) / (t1_out - t0_out));

	return T_DegC;
}

float HTS221_get_humidity()
{
	uint8_t h0_rh_x2 = 0;
	uint8_t h1_rh_x2 = 0;
	int16_t h0_t0_out = 0;
	int16_t h1_t0_out = 0;
	int16_t h_out = 0;

	// H0_T0
	uint8_t h0_t0_temp = HTS221_read_byte(HTS221_H0_T0_OUT_L);
	h0_t0_out |= h0_t0_temp;
	h0_t0_temp = HTS221_read_byte(HTS221_H0_T0_OUT_H);
	h0_t0_out |= (h0_t0_temp << 8);

	// H1_T0
	uint8_t h1_t0_temp = HTS221_read_byte(HTS221_H1_T0_OUT_L);
	h1_t0_temp |= h1_t0_temp;
	h1_t0_temp = HTS221_read_byte(HTS221_H1_T0_OUT_H);
	h1_t0_out |= (h1_t0_temp << 8);

	// H0_rH_x2
	h0_rh_x2 = HTS221_read_byte(HTS221_H0_RH_x2);

	// H1_rH_x2
	h1_rh_x2 = HTS221_read_byte(HTS221_H1_RH_x2);

	// H_OUT
	uint8_t h0_out_temp = HTS221_read_byte(HTS221_HUMIDITY_OUT_L);
	h_out |= h0_out_temp;
	h0_out_temp = HTS221_read_byte(HTS221_HUMIDITY_OUT_H);
	h_out |= (h0_out_temp << 8);

	float humidity = 0.0;
	float h0 = h0_rh_x2 / 2.0;
	float h1 = h1_rh_x2 / 2.0;
	humidity = (h0 + (h_out - h0_t0_out) * (h1 - h0) / (h1_t0_out - h0_t0_out));

	return humidity;
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

	uint8_t reg_status = HTS221_read_byte(HTS221_CONTROL_REG);
	// PD mode ON (power on)
	reg_status |= (1 << 7);
	HTS221_write_byte(reg_status, HTS221_CONTROL_REG, hts221_address);
	// Output data rate setup
	reg_status = HTS221_read_byte(HTS221_CONTROL_REG);
	reg_status |= (1 << 0);
	HTS221_write_byte(reg_status, HTS221_CONTROL_REG, hts221_address);

	reg_status = HTS221_read_byte(HTS221_CONTROL_REG);

	return status;
}
