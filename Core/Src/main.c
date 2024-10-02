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
#include <wchar.h>
#include <locale.h>
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
void send_message(wchar_t*, wchar_t*);
int* convert_message_hun(wchar_t*, int*, int*);
int* convert_message_eng(wchar_t*, int*, int*);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
GPIO_PinState led_set = GPIO_PIN_RESET;
GPIO_PinState led_reset = GPIO_PIN_SET;
GPIO_PinState is_pushed = GPIO_PIN_RESET;

#define ENG L"english"
#define HUN L"hungarian"

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
  setlocale(LC_CTYPE, "");

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  GPIO_PinState button;
  bool started=false;
  wchar_t* message_hun = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ \nabcdefghijklmnopqrstuvwxyz \n0123456789 \nárvíztűrő tükörfúrógép \nÁRVÍZTŰRŐ TÜKÖRFÚRÓGÉP \n, . - / * + ? ! : \t @\n";
  wchar_t* message_eng = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ \nabcdefghijklmnopqrstuvwxyz \n0123456789 \n, . - / * + ? ! : \t @\n";
  int num = 0x50;
  wchar_t del_str[2];
  del_str[0] = 127;
  del_str[1] = L'\0';

  bool test = false;

  HAL_Delay(3000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  button = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	  if (!test)
	  {
		  if(button == is_pushed)
		  {
			  /*send_message(message_hun,HUN);
			  send_message(del_str,HUN);
			  USBD_Delay(500);*/

			  send_message(message_eng,ENG);
			  USBD_Delay(500);
		  }
	  }
	  else
	  {
		  if (button == is_pushed && test)
		  	  {
		  		  started = !started;
		  	  }

		  	  if(started)
		  	  {
		  		  report[2]=num;
		  		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);

		  		  USBD_HID_Keybaord_SendReport(&hUsbDevice, report, len);
		  		  USBD_Delay(100);
		  		  num++;

		  		  report[2]= 0x00;
		  		  USBD_HID_Keybaord_SendReport(&hUsbDevice, report, len);
		  		  USBD_Delay(100);

		  		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);
		  		  started = !started;
		  	  }
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
void send_message(wchar_t* message, wchar_t* lang)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);

	int length = wcslen(message);
	int t[length+1];
	int is_special[length+1];
	if(wcscmp(lang,HUN)==1)
		convert_message_hun(message, t, is_special);
	else
		convert_message_eng(message, t, is_special);
	for(int i=0; i<length+1;++i)
	{
		if(i!=0 && t[i-1]=='\n')
			USBD_Delay(50);

		report[2] = t[i];
		report[0] = is_special[i];

		USBD_HID_Keybaord_SendReport(&hUsbDevice, report, len);
		USBD_Delay(150);
	}

	report[0] = 0x00;
	USBD_HID_Keybaord_SendReport(&hUsbDevice, report, len);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);
}

int* convert_message_hun(wchar_t* message, int *t, int* is_special)
{
	int length = wcslen(message);

	for(int i=0; i<length; ++i)
	{
		is_special[i] = 0x00;
		switch (message[i])
		{
		    case 'A': is_special[i] = 0x02;
		    case 'a': t[i] = 0x04; break;
		    case 'B': is_special[i] = 0x02;
		    case 'b': t[i] = 0x05; break;
		    case 'C': is_special[i] = 0x02;
		    case 'c': t[i] = 0x06; break;
		    case 'D': is_special[i] = 0x02;
		    case 'd': t[i] = 0x07; break;
		    case 'E': is_special[i] = 0x02;
		    case 'e': t[i] = 0x08; break;
		    case 'F': is_special[i] = 0x02;
		    case 'f': t[i] = 0x09; break;
		    case 'G': is_special[i] = 0x02;
		    case 'g': t[i] = 0x0a; break;
		    case 'H': is_special[i] = 0x02;
		    case 'h': t[i] = 0x0b; break;
		    case 'I': is_special[i] = 0x02;
		    case 'i': t[i] = 0x0c; break;
		    case 'J': is_special[i] = 0x02;
		    case 'j': t[i] = 0x0d; break;
		    case 'K': is_special[i] = 0x02;
		    case 'k': t[i] = 0x0e; break;
		    case 'L': is_special[i] = 0x02;
		    case 'l': t[i] = 0x0f; break;
		    case 'M': is_special[i] = 0x02;
		    case 'm': t[i] = 0x10; break;
		    case 'N': is_special[i] = 0x02;
		    case 'n': t[i] = 0x11; break;
		    case 'O': is_special[i] = 0x02;
		    case 'o': t[i] = 0x12; break;
		    case 'P': is_special[i] = 0x02;
		    case 'p': t[i] = 0x13; break;
		    case 'Q': is_special[i] = 0x02;
		    case 'q': t[i] = 0x14; break;
		    case 'R': is_special[i] = 0x02;
		    case 'r': t[i] = 0x15; break;
		    case 'S': is_special[i] = 0x02;
		    case 's': t[i] = 0x16; break;
		    case 'T': is_special[i] = 0x02;
		    case 't': t[i] = 0x17; break;
		    case 'U': is_special[i] = 0x02;
		    case 'u': t[i] = 0x18; break;
		    case 'V': is_special[i] = 0x02;
		    case 'v': t[i] = 0x19; break;
		    case 'W': is_special[i] = 0x02;
		    case 'w': t[i] = 0x1a; break;
		    case 'X': is_special[i] = 0x02;
		    case 'x': t[i] = 0x1b; break;
		    case 'Y': is_special[i] = 0x02;
		    case 'y': t[i] = 0x1d; break;
		    case 'Z': is_special[i] = 0x02;
		    case 'z': t[i] = 0x1c; break;

		    case L'Ö': is_special[i] = 0x20;
		    case L'ö': t[i] = 0x27; break;
			case L'Ü': is_special[i] = 0x20;
			case L'ü': t[i] = 0x2d; break;
			case L'Ó': is_special[i] = 0x20;
			case L'ó': t[i] = 0x2e; break;
			case L'Ő': is_special[i] = 0x20;
			case L'ő': t[i] = 0x2f; break;
			case L'Ú': is_special[i] = 0x20;
			case L'ú': t[i] = 0x30; break;
			case L'É': is_special[i] = 0x20;
			case L'é': t[i] = 0x33; break;
			case L'Á': is_special[i] = 0x20;
			case L'á': t[i] = 0x34; break;
			case L'Ű': is_special[i] = 0x20;
			case L'ű': t[i] = 0x31; break;
			case L'Í': is_special[i] = 0x20;
			case L'í': t[i] = 0x64; break;

		    case '\n': t[i] = 0x28; break;
		    case ' ': t[i] = 0x2c; break;
		    case ',': t[i] = 0x36; break;
		    case '.': t[i] = 0x37; break;
		    case '-': t[i] = 0x38; break;

		    case '/': t[i] = 0x54; break;
		    case '*': t[i] = 0x55; break;
		    case '+': t[i] = 0x57; break;
		    case '?': is_special[i] = 0x20; t[i] = 0x36; break;
		    case '!': is_special[i] = 0x20; t[i] = 0x21; break;
		    case ':': is_special[i] = 0x20; t[i] = 0x37; break;
		    case '\t': is_special[i] = 0x00; t[i] = 0x2b; break;
		    case 127: is_special[i] = 0x00; t[i] = 0x2a; break; // delete
		    case '@': is_special[i] = 0x40; t[i] = 0x19; break;

		    case '0': t[i] = 0x35; break;
		    case '1': t[i] = 0x1e; break;
		    case '2': t[i] = 0x1f; break;
		    case '3': t[i] = 0x20; break;
		    case '4': t[i] = 0x21; break;
		    case '5': t[i] = 0x22; break;
		    case '6': t[i] = 0x23; break;
		    case '7': t[i] = 0x24; break;
		    case '8': t[i] = 0x25; break;
		    case '9': t[i] = 0x26; break;

		    default: t[i] = 0x00; break;
		}
	}

	t[length] = 0x00;
	is_special[length]= 0x00;

	return t;
}

int* convert_message_eng(wchar_t* message, int *t, int* is_special)
{
	int length = wcslen(message);

	for(int i=0; i<length; ++i)
	{
		is_special[i] = 0x00;
		switch (message[i])
		{
		    case 'A': is_special[i] = 0x02;
		    case 'a': t[i] = 0x04; break;
		    case 'B': is_special[i] = 0x02;
		    case 'b': t[i] = 0x05; break;
		    case 'C': is_special[i] = 0x02;
		    case 'c': t[i] = 0x06; break;
		    case 'D': is_special[i] = 0x02;
		    case 'd': t[i] = 0x07; break;
		    case 'E': is_special[i] = 0x02;
		    case 'e': t[i] = 0x08; break;
		    case 'F': is_special[i] = 0x02;
		    case 'f': t[i] = 0x09; break;
		    case 'G': is_special[i] = 0x02;
		    case 'g': t[i] = 0x0a; break;
		    case 'H': is_special[i] = 0x02;
		    case 'h': t[i] = 0x0b; break;
		    case 'I': is_special[i] = 0x02;
		    case 'i': t[i] = 0x0c; break;
		    case 'J': is_special[i] = 0x02;
		    case 'j': t[i] = 0x0d; break;
		    case 'K': is_special[i] = 0x02;
		    case 'k': t[i] = 0x0e; break;
		    case 'L': is_special[i] = 0x02;
		    case 'l': t[i] = 0x0f; break;
		    case 'M': is_special[i] = 0x02;
		    case 'm': t[i] = 0x10; break;
		    case 'N': is_special[i] = 0x02;
		    case 'n': t[i] = 0x11; break;
		    case 'O': is_special[i] = 0x02;
		    case 'o': t[i] = 0x12; break;
		    case 'P': is_special[i] = 0x02;
		    case 'p': t[i] = 0x13; break;
		    case 'Q': is_special[i] = 0x02;
		    case 'q': t[i] = 0x14; break;
		    case 'R': is_special[i] = 0x02;
		    case 'r': t[i] = 0x15; break;
		    case 'S': is_special[i] = 0x02;
		    case 's': t[i] = 0x16; break;
		    case 'T': is_special[i] = 0x02;
		    case 't': t[i] = 0x17; break;
		    case 'U': is_special[i] = 0x02;
		    case 'u': t[i] = 0x18; break;
		    case 'V': is_special[i] = 0x02;
		    case 'v': t[i] = 0x19; break;
		    case 'W': is_special[i] = 0x02;
		    case 'w': t[i] = 0x1a; break;
		    case 'X': is_special[i] = 0x02;
		    case 'x': t[i] = 0x1b; break;
		    case 'Y': is_special[i] = 0x02;
		    case 'y': t[i] = 0x1c; break;
		    case 'Z': is_special[i] = 0x02;
		    case 'z': t[i] = 0x1d; break;

		    case '\n': t[i] = 0x28; break;
		    case ' ': t[i] = 0x2c; break;
		    case ',': t[i] = 0x36; break;
		    case '.': t[i] = 0x37; break;
		    case '-': t[i] = 0x2d; break;
		    case '/': t[i] = 0x38; break;
		    case '*': t[i] = 0x55; break;
		    case '+': t[i] = 0x57; break;
		    case '?': is_special[i] = 0x20; t[i] = 0x38; break;
		    case '!': is_special[i] = 0x20; t[i] = 0x1e; break;
		    case ':': is_special[i] = 0x20; t[i] = 0x33; break;
		    case '\t': is_special[i] = 0x00; t[i] = 0x2b; break;
		    case 127: is_special[i] = 0x00; t[i] = 0x2a; break; // delete

		    // NO
		    case '@': is_special[i] = 0x20; t[i] = 0x34; break;

		    case '0': t[i] = 0x27; break;
		    case '1': t[i] = 0x1e; break;
		    case '2': t[i] = 0x1f; break;
		    case '3': t[i] = 0x20; break;
		    case '4': t[i] = 0x21; break;
		    case '5': t[i] = 0x22; break;
		    case '6': t[i] = 0x23; break;
		    case '7': t[i] = 0x24; break;
		    case '8': t[i] = 0x25; break;
		    case '9': t[i] = 0x26; break;

		    default: t[i] = 0x00; break;
		}
	}

	t[length] = 0x00;
	is_special[length]= 0x00;

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
