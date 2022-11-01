/*
 * fir_filter.c
 *
 *  Created on: 1. 11. 2022
 *      Author: jakub
 */

#include "fir_filter.h"

static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {};

void FIRFilter_Init(FIRFilter* fir)
{
	// Clear filter buffer
	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
	{
		fir->buff[n] = 0.0;
	}
	// Reset buffer index
	fir->buff_index = 0;
	// Clear filter output
	fir->out = 0.0;
}

float FIRFilter_Update(FIRFilter* fir, float input)
{
	// Store latest sample in buffer

	// Increment buffer index and wrap around if necessary

	// Compute new output sample
	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
	{
		// Decrement index and wrap if necessary

		// Multiply impulse response with shifted input sample and add to output
	}
}
