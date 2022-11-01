/*
 * fir_filter.h
 *
 *  Created on: 1. 11. 2022
 *      Author: jakub
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>

#define FILTER_LENGTH 64

typedef struct
{
	float buff[FILTER_LENGTH];
	uint8_t buff_index;
	float out;
} Filter;

void Filter_Init(Filter* fis, float start_value);
float Filter_Update(Filter* fis, float input);

#endif /* INC_FILTER_H_ */
