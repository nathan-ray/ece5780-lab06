/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  HAL_Init();
  SystemClock_Config();
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
	// GREEN  -> 9
	// ORANGE -> 8
	// BLUE		-> 7
	// RED		-> 6
	// Set up a configuration struct to pass to the initialization function
	GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7,
	GPIO_MODE_OUTPUT_PP,
	GPIO_SPEED_FREQ_LOW,
	GPIO_NOPULL};
	HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC6, PC7, PC8 & PC9
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // Start PC9 reset
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET); // Start PC8 reset
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // Start PC7 reset
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // Start PC6 reset
	
	/*
	// enable ADC in RCC
	__HAL_RCC_ADC1_CLK_ENABLE();
	// initialize PC0 to analog mode
	GPIOC->MODER |= (1 << 0);
	GPIOC->MODER |= (1 << 1);
	
	// resolution to 8 bits
	ADC1->CFGR1 |= (1 << 4);
	ADC1->CFGR1 &= ~(1 << 3);
	// continuous conversion
	ADC1->CFGR1 |= (1 << 13);
	// disable hardware trigger
	ADC1->CFGR1 &= ~(1 << 11);
	ADC1->CFGR1 &= ~(1 << 10);
	// select channel PC0 -> ADC_IN10
	ADC1->CHSELR |= (1 << 10);
	// makes sure ADC is first disabled and DMAEN is 0
	ADC1->CR &= ~(1 << 0);
	ADC1->CFGR1 &= ~(1 << 0);
	ADC1->CR |= (1 << 31);	// calibrate ADC
	
	// wait until calibration is done (ADCAL = 0)
	while ((ADC1->CR >> 31) & 1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		HAL_Delay(100);
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	
	// enable ADC
	ADC1->CR |= (1 << 0);
	// start ADC
	ADC1->CR |= (1 << 2);
	
	uint16_t data;
  while (1)
  {
		data = ADC1->DR;
		if (data > 1) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		}
		if (data > 75) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		}
		if (data > 175) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		}
		if (data > 250) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		}
  }
	*/
	
	// ###########################################
	// PART 2
	// ###########################################
	// enable DAC1 RCC
	__HAL_RCC_DAC1_CLK_ENABLE();
	
	// enable PA4
	GPIOA->MODER |= (1 << 9);
	GPIOA->MODER |= (1 << 8);
	
	// DAC software trigger mode
	DAC1->CR |= (1 << 5);
	DAC1->CR |= (1 << 4);
	DAC1->CR |= (1 << 3);
	
	// enable DAC channel 0
	DAC1->CR |= (1 << 0);
	
	// Sine Wave: 8-bit, 32 samples/cycle
	const uint8_t sine_table[32] = {127,151,175,197,216,232,244,251,254,251,244,
	232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};
	// Triangle Wave: 8-bit, 32 samples/cycle
	const uint8_t triangle_table[32] = {0,15,31,47,63,79,95,111,127,142,158,174,
	190,206,222,238,254,238,222,206,190,174,158,142,127,111,95,79,63,47,31,15};
	// Sawtooth Wave: 8-bit, 32 samples/cycle
	const uint8_t sawtooth_table[32] = {0,7,15,23,31,39,47,55,63,71,79,87,95,103,
	111,119,127,134,142,150,158,166,174,182,190,198,206,214,222,230,238,246};
	// Square Wave: 8-bit, 32 samples/cycle
	const uint8_t square_table[32] = {254,254,254,254,254,254,254,254,254,254,
	254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		
	int i = 0;
		
	while (1) {
		DAC1->DHR8R1 = sine_table[i];
		i++;
		if (i > 31) i = 0;
		HAL_Delay(1);
	}
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
