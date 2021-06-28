#include "main.h"
#include "project_hal.h"
#include "max30100_for_stm32_hal.h"

// Private vars
I2C_HandleTypeDef 	hI2C; 		// I2C Handler
TIM_HandleTypeDef 	hTIMER;		// TIMER Handler
UART_HandleTypeDef 	hUART;		// UART Handler

// Private functions declaration
void Error_Handler(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);

	/***********************************************************************
	*																																			 *
	*									 BELOW THE LIST OF MY HAL FUNCTIONS	 								 * 
	*																																			 *
	***********************************************************************/

	void START_HAL(void){
		HAL_Init();
	}

	void CONFIG_CLOCK(void){
		SystemClock_Config();
	}
		
  void START_GPIO(void){
	  MX_GPIO_Init();
  }
  
	void START_I2C(void){
		MX_I2C1_Init();
	}
	
	void START_TIMER(void){
		MX_TIM2_Init();
		HAL_TIM_Base_Start_IT(&hTIMER);
	}
		
	void START_UART(void){
		MX_USART2_UART_Init();
	}
	
	void START_MAX30100(void){
		MAX30100_Init(&hI2C, &hUART);
		MAX30100_SetSpO2SampleRate(MAX30100_SPO2SR_DEFAULT);
		MAX30100_SetLEDPulseWidth(MAX30100_LEDPW_DEFAULT);
		MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_0_0, MAX30100_LEDCURRENT_0_0);
		MAX30100_SetMode(MAX30100_IDLE_MODE);
	}
	
	void MAX30100_Stop(void){
		MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_0_0, MAX30100_LEDCURRENT_0_0);
		MAX30100_SetMode(MAX30100_IDLE_MODE);
	}

	void MAX30100_Start(void){
		MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_DEFAULT, MAX30100_LEDCURRENT_DEFAULT);
		MAX30100_SetMode(MAX30100_SPO2_MODE);
	}
	
	void ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
		HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	}
	
	void WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState){
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
	}
	
	void UART_Receive(uint8_t *pData){
		HAL_UART_Receive_IT(&hUART, pData, sizeof(pData));
	}
	
	void UART_Transmit(uint8_t *pData, uint16_t timeout){
		HAL_UART_Transmit(&hUART, pData, sizeof(pData), timeout);
	}
		
	void I2C_Receive(uint16_t DevAddress, uint8_t * pData){
		HAL_I2C_Master_Receive_IT (&hI2C, DevAddress, pData, sizeof(pData));
	}
	
	void I2C_Transmit(uint16_t DevAddress, uint8_t * pData){
		HAL_I2C_Master_Transmit_IT (&hI2C, DevAddress, pData, sizeof(pData));
	}
	
	
	/***********************************************************************
	*																																			 *
	*	 										BELOW LIST OF INTERRUPTIONS	 										 * 
	*																																			 *
	***********************************************************************/
	
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) 
	{
		Interruption_TIM2();
	}

	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		Interruption_UART();
	}
	
	void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef * hi2c)
	{
		Interruption_I2C();
	}
	
	/***********************************************************************
	*																																			 *
	*	 BELOW LIST OF HAL USED IN THE PROJECT PROVIDED BY THE MANUFACTURER	 * 
	*																																			 *
	***********************************************************************/
	
// System Clock Configuration
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    Error_Handler();
	
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    Error_Handler();
}	

// GPIO Initialization Function
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7 PA8 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PIN_GPIO_Port, &GPIO_InitStruct);
}


// I2C1 Initialization Function
static void MX_I2C1_Init(void)
{
  hI2C.Instance = I2C1;
  hI2C.Init.Timing = 0x00000708;
  hI2C.Init.OwnAddress1 = 0;
  hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hI2C.Init.OwnAddress2 = 0;
  hI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	
  if (HAL_I2C_Init(&hI2C) != HAL_OK)
    Error_Handler();
	
  if (HAL_I2CEx_ConfigAnalogFilter(&hI2C, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    Error_Handler();
	
  if (HAL_I2CEx_ConfigDigitalFilter(&hI2C, 0) != HAL_OK)
    Error_Handler();  
}

// TIM2 Initialization Function
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  hTIMER.Instance = TIM2;
  hTIMER.Init.Prescaler = 0;
  hTIMER.Init.CounterMode = TIM_COUNTERMODE_UP;
  hTIMER.Init.Period = 65535;
  hTIMER.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  hTIMER.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&hTIMER) != HAL_OK)
    Error_Handler();

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&hTIMER, &sClockSourceConfig) != HAL_OK)
    Error_Handler();
	
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&hTIMER, &sSlaveConfig) != HAL_OK)
    Error_Handler();

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&hTIMER, &sMasterConfig) != HAL_OK)
    Error_Handler();

}

// USART2 Initialization Function
static void MX_USART2_UART_Init(void)
{
  hUART.Instance = USART2;
  hUART.Init.BaudRate = 9600;
  hUART.Init.WordLength = UART_WORDLENGTH_8B;
  hUART.Init.StopBits = UART_STOPBITS_1;
  hUART.Init.Parity = UART_PARITY_NONE;
  hUART.Init.Mode = UART_MODE_TX_RX;
  hUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hUART.Init.OverSampling = UART_OVERSAMPLING_16;
  hUART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hUART) != HAL_OK)
    Error_Handler();
}

// This function is executed in case of error occurrence
void Error_Handler(void)
{
  __disable_irq();
  while (1){};
}
