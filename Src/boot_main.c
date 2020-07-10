/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "boot_main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef void (*pFunction)(void);


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RxBufferSIZE 43
#define APP_START_ADDRESS 0x08060000
#define SECTOR_NUM 7
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;


/* USER CODE BEGIN PV */
/* Maximum length of one line of the hex format */
char RxBuffer[255][RxBufferSIZE];

uint8_t record_length, data[16];
uint16_t i;
uint32_t address;
uint8_t application_state=0;
uint16_t length=0;
uint16_t cpt=0;

pFunction appEntry;
uint32_t appStack;

uint8_t *a;
uint8_t val;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void Process_HexLine(char *RxBuffer, uint16_t Size);
uint8_t char2hex(char);
uint32_t getAddress(char *);
void JumpToBootloader(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int boot_main(void)
{
  /* USER CODE BEGIN 1 */
	a=0x08060000;
	val=a[0];
/*	if (val!=0xff)
		JumpToBootloader();
*/
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer[length], RxBufferSIZE)!=HAL_OK)
	  Error_Handler();


  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOD, LED4_Pin);
	  HAL_Delay(100);
	  if (application_state==1){
			JumpToBootloader();
/*		  for(cpt=0;cpt<length;cpt++){
			  if (RxBuffer[cpt][7]=='0'&&RxBuffer[cpt][8]=='0'){
				  Process_HexLine(RxBuffer[cpt],RxBufferSIZE);
			  }
		  }
		  HAL_GPIO_WritePin(GPIOD, LED5_Pin, GPIO_PIN_SET);
		  SCB->VTOR = APP_START_ADDRESS;
		  JumpToBootloader();
	  */}

  }
	/* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED4_Pin|LED3_Pin|LED5_Pin|LED6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED4_Pin LED3_Pin LED5_Pin LED6_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|LED3_Pin|LED5_Pin|LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1){
		if (RxBuffer[length][7]=='0'&&RxBuffer[length][8]=='0'){
			length++;
			if (HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer[length], RxBufferSIZE)!=HAL_OK)
				Error_Handler();
		}

		else if (RxBuffer[length][7]=='0'&&RxBuffer[length][8]=='1'){
				//HAL_GPIO_WritePin(GPIOD, LED5_Pin, GPIO_PIN_SET);
				MY_FLASH_SetSectorAddrs(SECTOR_NUM, APP_START_ADDRESS);
				HAL_FLASH_Unlock();
				FLASH_Erase_Sector(SECTOR_NUM, FLASH_VOLTAGE_RANGE_3);

				for(cpt=0;cpt<length;cpt++){
					if (RxBuffer[cpt][7]=='0'&&RxBuffer[cpt][8]=='0'){
						Process_HexLine(RxBuffer[cpt],RxBufferSIZE);
					}
				}
				HAL_FLASH_Lock();

				application_state=1;
//				JumpToBootloader();
			  }
		}

}


void Process_HexLine(char *pData, uint16_t Size)
{
	record_length=(uint8_t)(char2hex(pData[1])*16+char2hex(pData[2]));
	address = getAddress(&pData[3]);


	for(i=0;i<(uint16_t)record_length<<1;i++)
	{
		data[i]=(uint8_t)(char2hex(pData[2*i+9])*16+char2hex(pData[2*i+9+1]));
	}

	uint32_t flashAddress = APP_START_ADDRESS + address;
	for(uint32_t i=0; i<record_length; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddress , ((uint8_t *)data)[i]);
		flashAddress++;
	}



}

uint8_t char2hex(char c)
{

	switch (c)
	{
	case 'F': return 15;break;
	case 'E': return 14;break;
	case 'D': return 13;break;
	case 'C': return 12;break;
	case 'B': return 11;break;
	case 'A': return 10;break;
	default : return c-'0';
	}

}

uint32_t getAddress(char *pData)
{
	uint16_t j;
	uint32_t ret=0;
	uint32_t expo=1;
	for(j=0;j<4;j++){
		ret+=(uint32_t)char2hex(pData[3-j])*expo;
		expo*=16;
	}
	return ret;
}


void JumpToBootloader(void) {


    appStack = (uint32_t) *((__IO uint32_t*)APP_START_ADDRESS);

    appEntry = (pFunction) *(__IO uint32_t*) (APP_START_ADDRESS + 4);

    SCB->VTOR = APP_START_ADDRESS;

    __set_MSP(appStack);

    appEntry();
    /**
     * Step: Set main stack pointer.
     *       This step must be done last otherwise local variables in this function
     *       don't have proper value since stack pointer is located on different position
     *
     *       Set direct address location which specifies stack pointer in SRAM location
     */





}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_WritePin(GPIOD,LED3_Pin, GPIO_PIN_SET);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
