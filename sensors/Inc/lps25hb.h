/*
 * lps25hb.h
 *
 *  Created on: 29. 10. 2022
 *      Author: jakub
 */

#ifndef INC_LPS25HB_H_
#define INC_LPS25HB_H_

// Register address definitions
#define LPS25HB_WHO_AM_I_ADDRESS			0x0f
#define LPS25HB_WHO_AM_I_VALUE				0xBD

#define LPS25HB_RESOLUTION_REG				0x10

#define LPS25HB_CONTROL_REG1				0x20
#define LPS25HB_CONTROL_REG2				0x21

#define LPS25HB_PRESSURE_OUT_XL				0x28
#define LPS25HB_PRESSURE_OUT_L				0x29
#define LPS25HB_PRESSURE_OUT_H				0x2A

#define LPS25HB_TEMPERATURE_OUT_L			0x2B
#define	LPS25HB_TEMPERATURE_OUT_H			0x2C

#define LPS25HB_DEVICE_ADDRESS0				0xB8
#define LPS25HB_DEVICE_ADDRESS1				0xBA

// Function prototypes
void LPS25HB_Init();
void LPS25HB_Write(uint8_t data, uint8_t lenght);
uint8_t* LPS25HB_Read();

#endif /* INC_LPS25HB_H_ */
