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
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_device.h"
#include "usbd_hid_keyboard.h"
#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDevice;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void send_message(char*);
int* convert_message(char*, int*, bool*);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
GPIO_PinState led_set = GPIO_PIN_RESET;
GPIO_PinState led_reset = GPIO_PIN_SET;
GPIO_PinState is_pushed = GPIO_PIN_RESET;

uint16_t len = 8;
uint8_t report[8] = {0};
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
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  MX_USB_DEVICE_Init();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  GPIO_PinState button;
  char* message = "Tudok stringet convertalni. Wuhu\n";

  HAL_Delay(3000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  button = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	  if (button == is_pushed)
	  {
		  send_message(message);
		  USBD_Delay(500);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void send_message(char* message)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);

	int length = strlen(message);
	int t[length+1];
	bool is_bigletter[length+1];
	convert_message(message, t, is_bigletter);
	for(int i=0; i<length+1;++i)
	{
		if(i!=0 && t[i-1]=='\n')
			USBD_Delay(50);

		report[2] = t[i];
		if(is_bigletter[i])
			report[0] = 0x02;
		else
			report[0] = 0x00;

		USBD_HID_Keybaord_SendReport(&hUsbDevice, report, len);
		USBD_Delay(150);
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);
}

int* convert_message(char* message, int *t, bool* is_bigletter)
{
	int length = strlen(message);

	for(int i=0; i<length; ++i)
	{
		is_bigletter[i] = false;
		switch (message[i])
		{
		    case 'A': is_bigletter[i] = true;
		    case 'a': t[i] = 0x04; break;
		    case 'B': is_bigletter[i] = true;
		    case 'b': t[i] = 0x05; break;
		    case 'C': is_bigletter[i] = true;
		    case 'c': t[i] = 0x06; break;
		    case 'D': is_bigletter[i] = true;
		    case 'd': t[i] = 0x07; break;
		    case 'E': is_bigletter[i] = true;
		    case 'e': t[i] = 0x08; break;
		    case 'F': is_bigletter[i] = true;
		    case 'f': t[i] = 0x09; break;
		    case 'G': is_bigletter[i] = true;
		    case 'g': t[i] = 0x0a; break;
		    case 'H': is_bigletter[i] = true;
		    case 'h': t[i] = 0x0b; break;
		    case 'I': is_bigletter[i] = true;
		    case 'i': t[i] = 0x0c; break;
		    case 'J': is_bigletter[i] = true;
		    case 'j': t[i] = 0x0d; break;
		    case 'K': is_bigletter[i] = true;
		    case 'k': t[i] = 0x0e; break;
		    case 'L': is_bigletter[i] = true;
		    case 'l': t[i] = 0x0f; break;
		    case 'M': is_bigletter[i] = true;
		    case 'm': t[i] = 0x10; break;
		    case 'N': is_bigletter[i] = true;
		    case 'n': t[i] = 0x11; break;
		    case 'O': is_bigletter[i] = true;
		    case 'o': t[i] = 0x12; break;
		    case 'P': is_bigletter[i] = true;
		    case 'p': t[i] = 0x13; break;
		    case 'Q': is_bigletter[i] = true;
		    case 'q': t[i] = 0x14; break;
		    case 'R': is_bigletter[i] = true;
		    case 'r': t[i] = 0x15; break;
		    case 'S': is_bigletter[i] = true;
		    case 's': t[i] = 0x16; break;
		    case 'T': is_bigletter[i] = true;
		    case 't': t[i] = 0x17; break;
		    case 'U': is_bigletter[i] = true;
		    case 'u': t[i] = 0x18; break;
		    case 'V': is_bigletter[i] = true;
		    case 'v': t[i] = 0x19; break;
		    case 'W': is_bigletter[i] = true;
		    case 'w': t[i] = 0x1a; break;
		    case 'X': is_bigletter[i] = true;
		    case 'x': t[i] = 0x1b; break;
		    case 'Y': is_bigletter[i] = true;
		    case 'y': t[i] = 0x1d; break;
		    case 'Z': is_bigletter[i] = true;
		    case 'z': t[i] = 0x1c; break;
		    case '\n': t[i] = 0x28; break;
		    case ' ': t[i] = 0x2c; break;
		    case '.': t[i] = 0x37; break;
		    default: t[i] = 0x00; break;
		}
	}

	t[length] = 0x00;

	return t;
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
