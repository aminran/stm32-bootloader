/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "crc.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Flash_Base_App_1 0x08004000UL
#define Bootloader_version 1
// # define MANUFACTURER_ID 0xCAFE00
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t RX_Uart1[300];
uint8_t RX_Uart_CMD_Checker = 0;
uint8_t responseFromMCU[20];
uint8_t responseFromMCU_lenght = 0;

uint32_t tempAddressMemWrite = 0;
uint8_t tempMaxCountWrite = 0;
int8_t y = 0;
uint32_t flash_data = 0;
volatile uint32_t crcCheck = 0;
volatile uint32_t javaCrc = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

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
	MX_RTC_Init();
	MX_USART2_UART_Init();
	MX_LPUART1_UART_Init();
	MX_ADC_Init();
	MX_CRC_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	if (HAL_GPIO_ReadPin(boot_GPIO_Port, boot_Pin) == GPIO_PIN_SET)
	{
		// Go to user app
		void (*user_App_1)(void);

		uint32_t tmep_msp = *(__IO uint32_t *)Flash_Base_App_1;
		__set_MSP(tmep_msp);
		uint32_t user_app1_RSThandler_address = *(__IO uint32_t *)(Flash_Base_App_1 + 4);
		user_App_1 = (void *)user_app1_RSThandler_address;

		user_App_1();
	}
	else
	{
		// bootloader todo

		while (1)
		{

			for (uint16_t cr = 0; cr < sizeof(RX_Uart1); cr++)
				RX_Uart1[cr] = 0;

			HAL_UART_Receive(&huart1, RX_Uart1, 1, HAL_MAX_DELAY);
			if (RX_Uart1[0] & 0x80)
			{
				// in this state we should act with this bit as a response
			}
			else
			{
				// in this state we should act with this bit as a command
				uint8_t packetLenght = (RX_Uart1[0] & 0x7f);

				HAL_UART_Receive(&huart1, RX_Uart1 + 1, (packetLenght + 4), HAL_MAX_DELAY);

				for (uint16_t co = 0; co < sizeof(responseFromMCU); co++)
					responseFromMCU[co] = 0;

				uint32_t tmep_msp = 0;
				uint32_t user_app1_RSThandler_addressx = 0;
				void (*user_App_x)(void);

				uint32_t tempPageToErase = 0;
				FLASH_EraseInitTypeDef mFLASH_EraseInitTypeDef;
				uint32_t tempErrorErase = 0;

				uint32_t tempValue;
				//	uint32_t javaCrc=0;
				responseFromMCU_lenght = 1;
				uint8_t packetLenghtAsInt = 0;
				uint8_t conw;
				uint8_t read = (packetLenght + 1);
				uint8_t tempByte[4];
				uint8_t yshow;
				//////check crc part///////////

				if (packetLenght < 2)
				{
					javaCrc = RX_Uart1[2] << 24 | RX_Uart1[3] << 16 | RX_Uart1[4] << 8 | RX_Uart1[5];
					tempValue = 0;
					tempValue = RX_Uart1[0] << 24 | RX_Uart1[1] << 16;
					crcCheck = HAL_CRC_Calculate(&hcrc, &tempValue, 1);

					if (javaCrc == crcCheck)
					{

						// ACK response which means packet has been received correctly
						// A means Ack
						responseFromMCU[responseFromMCU_lenght++] = 'A';
					}
					else
					{
						//
						//
						//
						//
					}
				}
				else
				{
					__HAL_CRC_DR_RESET(&hcrc);
					for (conw = 0; conw < read; conw += 4)
					{
						if (conw < read)
						{
							tempByte[0] = RX_Uart1[conw];
						}
						else
						{

							tempByte[0] = 0x00;
						}
						if (conw + 1 < read)
						{
							tempByte[1] = RX_Uart1[conw + 1];
						}
						else
						{

							tempByte[1] = 0x00;
						}
						if (conw + 2 < read)
						{
							tempByte[2] = RX_Uart1[conw + 2];
						}
						else
						{

							tempByte[2] = 0x00;
						}
						if (conw + 3 < read)
						{
							tempByte[3] = RX_Uart1[conw + 3];
						}
						else
						{

							tempByte[3] = 0x00;
						}
						tempValue = tempByte[0] << 24 | tempByte[1] << 16 | tempByte[2] << 8 | tempByte[3];
						crcCheck = HAL_CRC_Accumulate(&hcrc, &tempValue, 1);
					}

					javaCrc = RX_Uart1[(packetLenght) + 1] << 24 | RX_Uart1[(packetLenght) + 2] << 16 | RX_Uart1[(packetLenght) + 3] << 8 | RX_Uart1[(packetLenght) + 4];

					if (javaCrc == crcCheck)
					{

						// A means Ack
						responseFromMCU[responseFromMCU_lenght++] = 'A';
					}
					else
					{
						HAL_UART_Transmit(&huart1, "CRCNOTOK", 8, 100);
						//
						//
						//
						//
					}
				}

				///////////////end of crc check////////////////

				switch (RX_Uart1[1])
				{

				case 1:
					// Help

					if (responseFromMCU[1] == 'A')
					{
						responseFromMCU[responseFromMCU_lenght++] = 'H';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[responseFromMCU_lenght++] = 'L';
						responseFromMCU[responseFromMCU_lenght++] = 'P';
						responseFromMCU[responseFromMCU_lenght++] = 'R';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[responseFromMCU_lenght++] = 'S';
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);
						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 500);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					break;

				case 2:
					// bootloader version
					if (responseFromMCU[1] == 'A')
					{
						responseFromMCU[responseFromMCU_lenght++] = 'V';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[responseFromMCU_lenght++] = 'R';
						responseFromMCU[responseFromMCU_lenght++] = 'S';
						responseFromMCU[responseFromMCU_lenght++] = 'I';
						responseFromMCU[responseFromMCU_lenght++] = 'O';
						responseFromMCU[responseFromMCU_lenght++] = 'N';
						responseFromMCU[responseFromMCU_lenght++] = ':';
						responseFromMCU[responseFromMCU_lenght++] = Bootloader_version + 0x30;
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);
						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					break;

				case 3:
					// flash sector status
					if (responseFromMCU[1] == 'A')
					{
						responseFromMCU[responseFromMCU_lenght++] = 'F';
						responseFromMCU[responseFromMCU_lenght++] = 'L';
						responseFromMCU[responseFromMCU_lenght++] = 'A';
						responseFromMCU[responseFromMCU_lenght++] = 'S';
						responseFromMCU[responseFromMCU_lenght++] = 'H';
						responseFromMCU[responseFromMCU_lenght++] = 'S';
						responseFromMCU[responseFromMCU_lenght++] = 'T';
						responseFromMCU[responseFromMCU_lenght++] = 'A';
						responseFromMCU[responseFromMCU_lenght++] = 'T';
						responseFromMCU[responseFromMCU_lenght++] = 'U';
						responseFromMCU[responseFromMCU_lenght++] = 'S';
						responseFromMCU[responseFromMCU_lenght++] = ':';
						responseFromMCU[responseFromMCU_lenght++] = ' ';
						responseFromMCU[responseFromMCU_lenght++] = ((FLASH->WRPR) >> 24) & 0xFF;
						responseFromMCU[responseFromMCU_lenght++] = ((FLASH->WRPR) >> 16) & 0xFF;
						responseFromMCU[responseFromMCU_lenght++] = ((FLASH->WRPR) >> 8) & 0xFF;
						responseFromMCU[responseFromMCU_lenght++] = ((FLASH->WRPR)) & 0xFF;
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}

					break;

				case 4:
					// send uniqe device ID to java app pc

					if (responseFromMCU[1] == 'A')
					{
						responseFromMCU[responseFromMCU_lenght++] = 'M';
						responseFromMCU[responseFromMCU_lenght++] = 'A';
						responseFromMCU[responseFromMCU_lenght++] = 'N';

						responseFromMCU[responseFromMCU_lenght++] = 'I';
						responseFromMCU[responseFromMCU_lenght++] = 'D';
						responseFromMCU[responseFromMCU_lenght++] = ':';
						responseFromMCU[responseFromMCU_lenght++] = ' ';

						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE)) / 100) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = (((*(uint8_t *)(UID_BASE) % 100)) / 10) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE)) % 10) + 0x30;

						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 1)) / 100) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = (((*(uint8_t *)(UID_BASE + 1)) % 100) / 10) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 1)) % 10) + 0x30;

						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 2)) / 100) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = (((*(uint8_t *)(UID_BASE + 2) % 100)) / 10) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 2)) % 10) + 0x30;

						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 3)) / 100) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = (((*(uint8_t *)(UID_BASE + 3) % 100)) / 10) + 0x30;
						responseFromMCU[responseFromMCU_lenght++] = ((*(uint8_t *)(UID_BASE + 3)) % 10) + 0x30;

						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}

					responseFromMCU[0] = ((*(uint8_t *)(UID_BASE)) / 100) + 0x30;
					responseFromMCU[1] = (((*(uint8_t *)(UID_BASE) % 100)) / 10) + 0x30;
					responseFromMCU[2] = ((*(uint8_t *)(UID_BASE)) % 10) + 0x30;

					responseFromMCU[3] = ((*(uint8_t *)(UID_BASE + 1)) / 100) + 0x30;
					responseFromMCU[4] = (((*(uint8_t *)(UID_BASE + 1)) % 100) / 10) + 0x30;
					responseFromMCU[5] = ((*(uint8_t *)(UID_BASE + 1)) % 10) + 0x30;

					responseFromMCU[6] = ((*(uint8_t *)(UID_BASE + 2)) / 100) + 0x30;
					responseFromMCU[7] = (((*(uint8_t *)(UID_BASE + 2) % 100)) / 10) + 0x30;
					responseFromMCU[8] = ((*(uint8_t *)(UID_BASE + 2)) % 10) + 0x30;

					responseFromMCU[9] = ((*(uint8_t *)(UID_BASE + 3)) / 100) + 0x30;
					responseFromMCU[10] = (((*(uint8_t *)(UID_BASE + 3) % 100)) / 10) + 0x30;
					responseFromMCU[11] = ((*(uint8_t *)(UID_BASE + 3)) % 10) + 0x30;

					HAL_UART_Transmit(&huart1, responseFromMCU, 12, 100);
					break;

				case 5:
					// jump to user app

					if (responseFromMCU[1] == 'A')
					{

						responseFromMCU[responseFromMCU_lenght++] = 'D';
						responseFromMCU[responseFromMCU_lenght++] = 'O';
						responseFromMCU[responseFromMCU_lenght++] = 'N';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);

						tmep_msp = *(__IO uint32_t *)Flash_Base_App_1;
						__set_MSP(tmep_msp);
						user_app1_RSThandler_addressx = *(__IO uint32_t *)(Flash_Base_App_1 + 4);
						user_App_x = (void *)user_app1_RSThandler_addressx;
						user_App_x();
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}

					// HAL_UART_Transmit(&huart1, "Done", 4, 100);

					break;

				case 6:
					// Memory Erase

					if (responseFromMCU[1] == 'A')
					{

						tempAddressMemWrite = RX_Uart1[2] << 24 | RX_Uart1[3] << 16 | RX_Uart1[4] << 8 | RX_Uart1[5];
						tempPageToErase = RX_Uart1[6];

						mFLASH_EraseInitTypeDef.NbPages = tempPageToErase;
						mFLASH_EraseInitTypeDef.PageAddress = tempAddressMemWrite;
						mFLASH_EraseInitTypeDef.TypeErase = FLASH_TYPEERASE_PAGES;

						HAL_FLASH_Unlock();

						HAL_FLASHEx_Erase(&mFLASH_EraseInitTypeDef, &tempErrorErase);

						HAL_FLASH_Unlock();

						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[responseFromMCU_lenght++] = 'D';
						responseFromMCU[responseFromMCU_lenght++] = 'O';
						responseFromMCU[responseFromMCU_lenght++] = 'N';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}

					break;

				case 7:
					// memory write command

					if (responseFromMCU[1] == 'A')
					{

						tempAddressMemWrite = RX_Uart1[2] << 24 | RX_Uart1[3] << 16 | RX_Uart1[4] << 8 | RX_Uart1[5];
						tempMaxCountWrite = ((RX_Uart1[0] & 0x7f) - 5);
						HAL_FLASH_Unlock();

						for (int i = 0; i < tempMaxCountWrite; i += 4)
						{
							//	flash_data= (RX_Uart1[i])<<24 | (RX_Uart1[i+1])<<16 | (RX_Uart1[i+2])<<8 | (RX_Uart1[i+3]) ;

							flash_data = (RX_Uart1[i + 9]) << 24 | (RX_Uart1[i + 8]) << 16 | (RX_Uart1[i + 7]) << 8 | (RX_Uart1[i + 6]);
							HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, tempAddressMemWrite + i, flash_data);
						}

						HAL_FLASH_Lock();

						responseFromMCU[responseFromMCU_lenght++] = 'P';
						responseFromMCU[responseFromMCU_lenght++] = 'D';
						responseFromMCU[responseFromMCU_lenght++] = 'O';
						responseFromMCU[responseFromMCU_lenght++] = 'N';
						responseFromMCU[responseFromMCU_lenght++] = 'E';
						responseFromMCU[0] = 0x80 + (responseFromMCU_lenght);

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}
					else
					{
						responseFromMCU[responseFromMCU_lenght++] = 'N'; // crc not ok
						responseFromMCU[0] = 0x81;

						HAL_UART_Transmit(&huart1, responseFromMCU, responseFromMCU_lenght, 100);
					}

					break;
				}
			}
		}
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

#ifdef USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
