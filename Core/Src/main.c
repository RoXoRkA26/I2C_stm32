/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "lsm6ds0.h"
#include "lps25hb.h"
#include "hts221.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include <math.h>

#define PRESSURE_0 1013.25
float reference_pressure = 0.0;

void SystemClock_Config(void);

int main(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();

//  lsm6ds0_init();
	LPS25HB_Init();
	HTS221_Init();

	char message_pressure[128];
	memset(message_pressure, 0, sizeof(message_pressure));

	// Calculate reference pressure
	uint8_t n_samples = 20;
	for (uint8_t sample = 0; sample < n_samples; sample++)
	{
		reference_pressure += LPS25HB_get_pressure();
		LL_mDelay(500);
	}
	reference_pressure /= n_samples;

	while (1)
	{
		memset(message_pressure, '\0', sizeof(message_pressure));

		// Pressure
		float pressure = LPS25HB_get_pressure();

		// Temperature
		float temperature = HTS221_get_temperature();

		// Humidity
		float humidity = HTS221_get_humidity();

		// Absolute height calculation
//		float press_ratio = PRESSURE_0 / pressure;
//		float press_pw = powf(press_ratio, (1 / 5.257));
//		float abs_height = ((press_pw - 1) * (temperature + 273.15)) / 0.0065;

		// Relative height calculation
		float press_ratio = reference_pressure / pressure;
		float press_pw = powf(press_ratio, (1 / 5.257));
		float rel_height = ((press_pw - 1) * (temperature + 273.15)) / 0.0065;

		// Format string
		sprintf(message_pressure, "%7.3f, %3.1f, %d, %5.2f\r", pressure, temperature, (int) humidity, rel_height);
		USART2_PutBuffer((uint8_t*) message_pressure, strlen(message_pressure));

		// Delay
		LL_mDelay(25);

	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
	{
		Error_Handler();
	}
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while (LL_RCC_HSI_IsReady() != 1)
	{

	}
	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
	{

	}
	LL_Init1msTick(8000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	LL_SetSystemCoreClock(8000000);
	LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
