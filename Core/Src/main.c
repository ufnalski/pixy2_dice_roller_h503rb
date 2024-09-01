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
#include "i2c.h"
#include "icache.h"
#include "memorymap.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssd1306.h"
#include "sts3215_servo.h"
#include "pixy2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STS3215_ID 1
#define REFRESH_RATE 4000 // ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;
__IO uint32_t BspButtonState = BUTTON_RELEASED;

/* USER CODE BEGIN PV */
uint32_t softTimerOLED;
char lcd_line[32];

uint8_t change_position_flag;
uint16_t ref_position;

uint8_t drive_internal_temperature;
uint8_t drive_supply_voltage;

uint16_t refresh_rate = REFRESH_RATE;

uint8_t pixy2_found_flag = 0;
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
	MX_USART2_UART_Init();
	MX_ICACHE_Init();
	MX_I2C2_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(22, 2);
	ssd1306_WriteString("ufnalski.edu.pl", Font_6x8, White);
	ssd1306_SetCursor(13, 15);
	ssd1306_WriteString("Dice roller/tester", Font_6x8, White);
	ssd1306_SetCursor(15, 25);
	ssd1306_WriteString("(Pixy2 + STS3215)", Font_6x8, White);
	ssd1306_UpdateScreen();
	/* USER CODE END 2 */

	/* Initialize led */
	BSP_LED_Init(LED_GREEN);

	/* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
	BspCOMInit.BaudRate = 115200;
	BspCOMInit.WordLength = COM_WORDLENGTH_8B;
	BspCOMInit.StopBits = COM_STOPBITS_1;
	BspCOMInit.Parity = COM_PARITY_NONE;
	BspCOMInit.HwFlowCtl = COM_HWCONTROL_NONE;
	if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
	{
		Error_Handler();
	}

	/* USER CODE BEGIN BSP */

	/* -- Sample board code to send message over COM1 port ---- */
	printf(
			"\r\nWelcome to STM32 + Pixy2 + STS3215 automatic dice roller!\r\n\r\n");

	/* -- Sample board code to switch on led ---- */
	BSP_LED_On(LED_GREEN);

	printf("Searching for Pixy2: ");
	HAL_StatusTypeDef i2c_result;

	for (uint8_t i = 1; i < 128; i++)
	{

		i2c_result = HAL_I2C_IsDeviceReady(PIXY2_I2C, (uint16_t) (i << 1), 2,
				2);
		if (i2c_result != HAL_OK) // HAL_ERROR or HAL_BUSY or HAL_TIMEOUT
		{
			printf("."); // No ACK received at that address
		}
		if (i2c_result == HAL_OK)
		{
			printf("0x%X", i); // Received an ACK at that address
			pixy2_found_flag = 1;
		}
	}
	printf("\r\n");

	if (1 == pixy2_found_flag)
	{
		printf("Pixy2 found :)\r\n\r\n");
	}
	else
	{
		printf("No Pixy2 found :(\r\n\r\n");
	}

	softTimerOLED = HAL_GetTick();

	// Testing communication with Pixy2
	Pixy2SetLamp(1, 1);
	HAL_Delay(2000);
	Pixy2SetLamp(0, 0);

	// Changing baud rate for UART to make it compatible with all converters out there ever produced :)
	STS3215SetBaudrate(STS3215_ID, 115200);
	MX_USART1_UART_Init_115200();
	STS3215SetSilentMode(STS3215_ID);

	// Testing motor mode - just for fun
	STS3215SetMotorMode(STS3215_ID);

	STS3215SetSpeed(STS3215_ID, 2000);
	HAL_Delay(1500);

	STS3215SetSpeed(STS3215_ID, 0);
	HAL_Delay(1500);

	STS3215SetSpeed(STS3215_ID, -2000);
	HAL_Delay(1500);

	STS3215SetSpeed(STS3215_ID, 0);
	HAL_Delay(1500);

	STS3215SetServoMode(STS3215_ID);

	/* USER CODE END BSP */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		if ((HAL_GetTick() - softTimerOLED) > refresh_rate)
		{
			softTimerOLED = HAL_GetTick();
			change_position_flag = change_position_flag == 0 ? 1 : 0; // toggle the flag

			STS3215ReadTemperature(STS3215_ID, &drive_internal_temperature);
			STS3215ReadVoltage(STS3215_ID, &drive_supply_voltage);

			ssd1306_SetCursor(10, 36);
			sprintf(lcd_line, "Temp.:    %u deg. C",
					drive_internal_temperature);
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			ssd1306_SetCursor(10, 46);
			sprintf(lcd_line, "Voltage:   %.1f V",
					((float) drive_supply_voltage) / 10.0);
			ssd1306_WriteString(lcd_line, Font_6x8, White);

			if (0 == change_position_flag)
			{
				ref_position = 0;
				refresh_rate = REFRESH_RATE / 2;

				switch (Pixy2SignatureDetect(
						PIXY2_SIGNATURE_1 | PIXY2_SIGNATURE_2
								| PIXY2_SIGNATURE_3 | PIXY2_SIGNATURE_4
								| PIXY2_SIGNATURE_5 | PIXY2_SIGNATURE_6))
				{
				case 1:
					printf("Dice face color: %s.\r\n", PIXY2_LABEL_1);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_1);
					break;
				case 2:
					printf("Dice face color: %s.   \r\n", PIXY2_LABEL_2);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_2);
					break;
				case 3:
					printf("Dice face color: %s.   \r\n", PIXY2_LABEL_3);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_3);
					break;
				case 4:
					printf("Dice face color: %s.   \r\n", PIXY2_LABEL_4);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_4);
					break;
				case 5:
					printf("Dice face color: %s.   \r\n", PIXY2_LABEL_5);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_5);
					break;
				case 6:
					printf("Dice face color: %s.   \r\n", PIXY2_LABEL_6);
					sprintf(lcd_line, "Dice face: %s   ", PIXY2_LABEL_6);
					break;
				case 0xFF:
					printf("Communication error.\r\n");
					sprintf(lcd_line, "Communication error.");
					break;
				default:
					printf("No face detected - check the illumination.\r\n");
					sprintf(lcd_line, "Dice face: %s.   ", "ERROR");
				}

				ssd1306_SetCursor(10, 56);
				ssd1306_WriteString(lcd_line, Font_6x8, White);
				ssd1306_UpdateScreen();
//				HAL_Delay(20000); // take photo
				printf("Rolling...\r\n");
			}
			else
			{
				ref_position = 4095;  // resolution 360 degrees / 4096
				refresh_rate = REFRESH_RATE;
			}

			STS3215TurnToPosition(STS3215_ID, ref_position, 0, 3000, 50);
		}

		/* -- Sample board code for User push-button in interrupt mode ---- */
		if (BspButtonState == BUTTON_PRESSED)
		{
			/* Update button state */
			BspButtonState = BUTTON_RELEASED;
			/* -- Sample board code to toggle led ---- */
			BSP_LED_Toggle(LED_GREEN);

			/* ..... Perform your action ..... */
		}
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
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
	{
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the programming delay
	 */
	__HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief BSP Push Button callback
 * @param Button Specifies the pressed button
 * @retval None
 */
void BSP_PB_Callback(Button_TypeDef Button)
{
	if (Button == BUTTON_USER)
	{
		BspButtonState = BUTTON_PRESSED;
	}
}

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
