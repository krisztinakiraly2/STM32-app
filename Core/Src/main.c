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
#include "rtc.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "variables.h"
#include "communication.h"
#include "encryption.h"
#include "flash.h"
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

void my_init();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool send = false;
volatile uint8_t mybuffer[3];
volatile uint8_t data[9];
int pos=0;
int num=0;
volatile uint8_t their_public_key_test[3];
extern int public_key_length;
extern bool data_recieved;
extern uint8_t ok;
wchar_t* email_or_username;
wchar_t* password;
bool nothing_to_send=false;
int email_or_username_len=0;
int password_len=0;
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
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  MX_USB_DEVICE_Init();
  my_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  button = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	  if(button == is_pushed)
	  {
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);
		  encrypt_and_decrypt_msg(command,11);
		  command[10] = '\n';
		  step=LOGIN_REQ;
		  nothing_to_send=false;

		  while(!data_recieved)
		  {
			  CDC_Transmit(acm_id, command, 11);
			  HAL_Delay(2000);
		  }

		  if(!nothing_to_send)
		  {
			  send_hid(email_or_username, email_or_username_len);
			  USBD_Delay(100);
			  send_hid(tab,1);
			  USBD_Delay(100);
			  send_hid(password,password_len);

			  free(email_or_username);
			  free(password);
		  }

		  step=READ_COMMAND;
		  data_recieved = false;
		  nothing_to_send=false;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);

	  }
	  else
	  {
		  if(data_recieved)
		  {
			  data_recieved = false;
			  switch(mybuffer[0])
			  {
				  case SET_UP_COMM: set_up_encryption(); break;

				  case SET_KEY_BOARD_LANG:
					  if(mybuffer[1] == 'e')
						  set_keyboard_language(ENG);
					  else
						  if(mybuffer[1] == 'h')
							  set_keyboard_language(HUN);
					  break;
			  }
		  }
	  }

	  USBD_Delay(10);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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

void my_init()
{
	  setlocale(LC_CTYPE, "");
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	  message_hun = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ \nabcdefghijklmnopqrstuvwxyz \n0123456789 \nárvíztűrő tükörfúrógép \nÁRVÍZTŰRŐ TÜKÖRFÚRÓGÉP \n, . - / * + ? ! : \t @\n";

	  del_str[0] = 127;
	  del_str[1] = L'\0';
}

void USB_CDC_RxHandler(uint8_t* Buf, uint32_t Len)
{
	switch(step)
	{
		case WAIT_FOR_REPLY:
			ok = Buf[0];

		case READ_PUBLIC_KEY:
			if(Len==64)
			{
				for(int i=0; i<Len; ++i)
				{
					their_public_key[i] = Buf[i];
				}

				data_recieved = true;
				pos=0;
				++step;
			} break;

		case READ_REPLY_MESSAGE:
			if(Len==8)
			{
				for(int i=0; i<Len; ++i)
				{
					data[i] = Buf[i];
				}

				data_recieved = true;
				pos=0;
			} break;

		case READ_COMMAND:
			if(Buf[0]>='0' || Buf[0]<='9')
			{
				mybuffer[0] = Buf[0]-'0';

				if(mybuffer[0]==SET_KEY_BOARD_LANG)
				{
					mybuffer[1] = Buf[2];
				}

				data_recieved = true;
				pos=0;
			} break;
		case LOGIN_REQ:
			switch(pos)
			{
				case 0:
					if(Len==1)
					{
						nothing_to_send=true;
						data_recieved = true;
						break;
					}
					else
					{
						uint8_t buffer[Len];
						for(int i=0; i<Len; ++i)
							buffer[i] = (wchar_t)Buf[i];
						encrypt_and_decrypt_msg(buffer, Len);
						email_or_username_len = Len;
						email_or_username = (wchar_t*) malloc(Len*sizeof(wchar_t));
						for (int i = 0; i < Len; ++i)
						{
							email_or_username[i] = (wchar_t)buffer[i];
						}
					}
					++pos;
					break;

				case 1:
					uint8_t buffer[Len];
					for(int i=0; i<Len; ++i)
						buffer[i] = Buf[i];
					encrypt_and_decrypt_msg(buffer, Len);
					password_len = Len;
					password  = (wchar_t*) malloc(Len*sizeof(wchar_t));
					for (int i = 0; i < Len; ++i)
					{
						password[i] = (wchar_t)buffer[i];
					}
					data_recieved = true;
					break;
			}

	}
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
