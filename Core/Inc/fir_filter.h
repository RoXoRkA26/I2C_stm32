/*
 * fir_filter.h
 *
 *  Created on: 1. 11. 2022
 *      Author: jakub
 */

#ifndef INC_FIR_FILTER_H_
#define INC_FIR_FILTER_H_

#include <stdint.h>

#define FIR_FILTER_LENGTH 16

typedef struct
{
	float buff[FIR_FILTER_LENGTH];
	uint8_t buff_index;
	float out;
} FIRFilter;

void FIRFilter_Init(FIRFilter* fir);
float FIRFilter_Update(FIRFilter* fir, float input);

#endif /* INC_FIR_FILTER_H_ */
