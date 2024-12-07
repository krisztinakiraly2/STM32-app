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
<<<<<<< HEAD

=======
void set_up_encryption();
void encrypt_and_decrypt_msg(uint8_t *);
void set_up_connection_with_app();
uint8_t modExp(uint8_t base, uint8_t exp, uint8_t mod);
void wait_for_response();
void receive_message(uint8_t* buffer, uint8_t length);
void save_data(uint8_t* d, uint8_t length);
void read_data(uint8_t* d, uint8_t length, uint32_t addr);
int read_command_code(int length);
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*GPIO_PinState led_set = GPIO_PIN_RESET;
GPIO_PinState led_reset = GPIO_PIN_SET;
GPIO_PinState is_pushed = GPIO_PIN_RESET;

uint8_t txbuffer[12] = "Welcome\n\r";
#define acm_id 0

#define ENG L"english"
#define HUN L"hungarian"

uint16_t len = 8;
uint8_t report[8] = {0};

GPIO_PinState button;
wchar_t* message_hun;
wchar_t del_str[2];

uint8_t my_private_key;
uint8_t my_public_key;
uint8_t their_public_key;
uint8_t secret;
uint8_t pow_base;
uint8_t mod_base;
bool res = false;

<<<<<<< HEAD
uint32_t address = 0x08020000;*/
=======
uint32_t address = 0x08020000;

#define SETUPCOMM 0
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38
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
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  MX_USB_DEVICE_Init();
  my_init();

<<<<<<< HEAD
=======
  //HAL_FLASH_Unlock();
  //FLASH_Erase_Sector(5, FLASH_VOLTAGE_RANGE_1);
  //HAL_FLASH_Lock();

  /*uint8_t ms[] = "Hihi hihi"; uint8_t d[10];
  save_data(ms,10);
  read_data(d,10,0x08020000);*/
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  button = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	  if(button == is_pushed)
	  {
		  send_hid(message_hun,HUN);
		  send_hid(del_str, HUN);
	  }
	  else
	  {
<<<<<<< HEAD
		  int code = read_command_code(1);
		  switch(code)
		  {
			  case SETUPCOMM: set_up_encryption();
=======
		  /*
		  uint8_t mess[] = "Hello\r\n";
		  encrypt_and_decrypt_msg(mess);
		  if(res)
			  CDC_Transmit(acm_id,mess,8);
		  else
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		  */

		  int code = read_command_code(1);
		  switch(code)
		  {
		  	  case SETUPCOMM: set_up_encryption();
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38
		  }
	  }

	  USBD_Delay(100);

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
<<<<<<< HEAD
=======

int read_command_code(int length)
{
	int l=0;
	uint8_t buffer[length];

	while(l<length)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		USBD_CDC_SetRxBuffer(acm_id, &hUsbDevice, &buffer[l]);
		while (1)
		{
			USBD_CDC_ReceivePacket(acm_id, &hUsbDevice);

			if (buffer[l] >= '0' && buffer[l] <= '9')
			{
				buffer[l] = buffer[l] - '0';
				break;
			}
		}
		++l;
	}

	int num=0;

	for(int i=length-1, j=0; i>=0; --i,++j)
	{
		num += buffer[j]*pow(10, i);
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	return num;
}

void save_data(uint8_t* d, uint8_t length)
{
	HAL_FLASH_Unlock();
	for(int i=0; i<length; ++i)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address+i, d[i]);
	}
	address += length;
	HAL_FLASH_Lock();
}

void read_data(uint8_t* d, uint8_t length, uint32_t addr)
{
	HAL_FLASH_Unlock();

	for(int i=0; i<length; ++i)
	{
		d[i] = *(__IO uint8_t*) addr++;
	}
	HAL_FLASH_Lock();
}
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38

void my_init()
{
	  setlocale(LC_CTYPE, "");
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	  message_hun = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ \nabcdefghijklmnopqrstuvwxyz \n0123456789 \nárvíztűrő tükörfúrógép \nÁRVÍZTŰRŐ TÜKÖRFÚRÓGÉP \n, . - / * + ? ! : \t @\n";

	  del_str[0] = 127;
	  del_str[1] = L'\0';
<<<<<<< HEAD
}

=======

	  my_private_key = 13;
	  pow_base = 2;
	  mod_base = 17;
	  my_public_key = modExp(pow_base,my_private_key,mod_base);
}

void set_up_encryption()
{
	while(!res)
	{
		//uint8_t enter[] = "\n\r\n\r";
		char num_str[10];
		HAL_Delay(2000);

		//CDC_Transmit(acm_id, (uint8_t*)"Starting setup\n\r", 17);
		//HAL_Delay(200);
		//wait_for_response();

		//CDC_Transmit(acm_id, (uint8_t*)"Sending pow base:\n\r", 20);
		HAL_Delay(100);

		snprintf(num_str, sizeof(num_str), "%d", pow_base);
		CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
		HAL_Delay(100);
		//CDC_Transmit(acm_id,enter,5);
		//HAL_Delay(100);
		//wait_for_response();

		//CDC_Transmit(acm_id,(uint8_t*)"Sending mod base:\n\r",20);
		//HAL_Delay(100);

		snprintf(num_str, sizeof(num_str), "%d", mod_base);
		CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
		HAL_Delay(100);
		//CDC_Transmit(acm_id,enter,5);
		//HAL_Delay(100);
		//wait_for_response();

		//CDC_Transmit(acm_id,(uint8_t*)"Sending public key:\n\r",22);
		//HAL_Delay(100);

		snprintf(num_str, sizeof(num_str), "%d", my_public_key);
		CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
		HAL_Delay(100);
		//CDC_Transmit(acm_id,enter,5);
		//HAL_Delay(100);
		//wait_for_response();

		//CDC_Transmit(acm_id,(uint8_t*)"Waiting for public key: (xx)\n\r\r",32);
		//HAL_Delay(100);
		uint8_t buffer[2];
		int length = 0;

		while(length<2)
		{
			USBD_CDC_SetRxBuffer(acm_id, &hUsbDevice, &buffer[length]);
			while (1)
			{
				USBD_CDC_ReceivePacket(acm_id, &hUsbDevice);

				if (buffer[length] >= '0' && buffer[length] <= '9')
				{
					buffer[length] = buffer[length] - '0';

					if(length==1)
						CDC_Transmit(acm_id,(uint8_t*)"\n\n\n\r",5);

					HAL_Delay(100);
					break;
				}
			}
			++length;
		}

		their_public_key = buffer[0]*10+buffer[1];

		//CDC_Transmit(acm_id,(uint8_t*)"Received public key:\n\r\r",24);
		//HAL_Delay(100);
		//snprintf(num_str, sizeof(num_str), "%d", their_public_key);
		//CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
		//HAL_Delay(100);
		//CDC_Transmit(acm_id,enter,5);
		//HAL_Delay(100);
		secret = modExp(their_public_key, my_private_key, mod_base);

		//CDC_Transmit(acm_id,(uint8_t*)"Sending encrypted message:\n\r",29);
		//HAL_Delay(100);
		uint8_t m[9] = "Comm set";
		encrypt_and_decrypt_msg(m);
		CDC_Transmit(acm_id,m,9);
		HAL_Delay(100);
		//CDC_Transmit(acm_id,enter,5);
		//HAL_Delay(100);
		//wait_for_response();
		encrypt_and_decrypt_msg(m);

		//CDC_Transmit(acm_id,(uint8_t*)"Waiting for reply message:\n\r",28);
		//HAL_Delay(100);

		uint8_t r[9];
		//receive_message(r, 8);
		//r[9]='\0';
		receive_message(r, 9);

		HAL_Delay(100);
		//CDC_Transmit(acm_id,(uint8_t*)"\n\n\rReceived message:\n\r\r",24);
		//HAL_Delay(100);
		//CDC_Transmit(acm_id,r,9);
		//HAL_Delay(100);

		encrypt_and_decrypt_msg(r);

		//CDC_Transmit(acm_id,(uint8_t*)"\n\r\r",4);
		//HAL_Delay(100);
		//CDC_Transmit(acm_id,r,9);
		//HAL_Delay(100);

		uint8_t om[9] = "Comm set";

		res=true;
		for(int i=0; i<8; ++i)
		{
			if(om[i]!=r[i])
				res=false;
		}

		//CDC_Transmit(acm_id,(uint8_t*)"\n\n\rAre they equal:\n\r\r",22);
		//HAL_Delay(100);
		if(res)
		{
			//CDC_Transmit(acm_id, (uint8_t*)"Yes\n\n\r", 7);
			CDC_Transmit(acm_id, (uint8_t*)"Yes", 4);
		}
		else
		{
			//CDC_Transmit(acm_id, (uint8_t*)"No\n\n\r", 6);
			CDC_Transmit(acm_id, (uint8_t*)"No", 3);
		}

		HAL_Delay(100);
	}
}

void wait_for_response()
{
	uint8_t ok;
	uint8_t res[] = "kK";;

	uint8_t msg[] = "Waiting for response (k)\n\r";
	CDC_Transmit(acm_id, msg, sizeof(msg) - 1);

	while (1)
	{
		// Set RX buffer and attempt to receive data
		USBD_CDC_SetRxBuffer(acm_id, &hUsbDevice, &ok);
		USBD_CDC_ReceivePacket(acm_id, &hUsbDevice);

		// Check if the received message matches "k"
		if (ok==res[0] || ok==res[1])
		{
			HAL_Delay(100);
			CDC_Transmit(acm_id,(uint8_t*)"\n\r",3);
			HAL_Delay(100);
			break;
		}
	}
}

void receive_message(uint8_t* buffer, uint8_t length)
{
	if(length < 1)
		return;

	uint8_t l=0;

	while(l<length)
	{
		USBD_CDC_SetRxBuffer(acm_id, &hUsbDevice, &buffer[l]);
		while (1)
		{
			USBD_CDC_ReceivePacket(acm_id, &hUsbDevice);

			if(buffer[l] >= '!' && buffer[l] <= '~')
			{
				break;
			}
		}
		++l;
	}
}

uint8_t modExp(uint8_t base, uint8_t exp, uint8_t mod)
{
    uint8_t result = 1; // Initial result is 1
    base = base % mod;  // In case base is larger than mod

    while (exp > 0)
    {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }

        // Now exp must be even
        exp = exp >> 1; // Divide exp by 2
        base = (base * base) % mod; // Square the base
    }

    return result;
}

void encrypt_and_decrypt_msg(uint8_t* m)
{
	size_t len = strlen((char*)m)-1;
	for (size_t i = 0; i < len; i++)
	{
		m[i] ^= secret;
	}
}

uint32_t modularPow(uint32_t base, uint32_t exponent, uint32_t mod)
{
    uint32_t result = 1;
    base = base % mod; // Take base modulo mod to avoid unnecessary large numbers

    while (exponent > 0) {
        // If the exponent is odd, multiply the result by the current base
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }

        // Square the base and take modulo
        base = (base * base) % mod;

        // Halve the exponent (shift right by 1)
        exponent = exponent >> 1;
    }

    return result;
}

void send_hid(wchar_t* message, wchar_t* lang)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);

	int length = wcslen(message);
	int t[length+1];
	int is_special[length+1];
	int res = wcscmp(lang,HUN);
	if(res == 0)
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
>>>>>>> 436364dcadc332030af8e4fa4feb42885150cb38
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
