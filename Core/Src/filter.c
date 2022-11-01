/*
 * fir_filter.c
 *
 *  Created on: 1. 11. 2022
 *      Author: jakub
 */

#include <filter.h>

void Filter_Init(Filter *fis, float start_value)
{
	// Set filter buffer
	for (uint8_t n = 0; n < FILTER_LENGTH; n++)
	{
		fis->buff[n] = start_value;
	}
	// Reset buffer index
	fis->buff_index = 0;
	// Clear filter output
	fis->out = 0.0;
}

float Filter_Update(Filter *fis, float input)
{
	// Store latest sample in buffer
	fis->buff[fis->buff_index++] = input;
	// Check buffer index and wrap around if necessary
	if (fis->buff_index >= FILTER_LENGTH)
	{
		fis->buff_index = 0;
	}
	// Compute new output sample
	fis->out = 0.0;
	float sum = 0.0;
	for (uint8_t i = 0; i < FILTER_LENGTH; i++)
	{
		sum += fis->buff[i];
	}
	sum /= FILTER_LENGTH;
	fis->out = sum;
	// Return new output
	return fis->out;
}
