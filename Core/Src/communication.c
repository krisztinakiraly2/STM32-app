#include "communication.h"

extern USBD_HandleTypeDef hUsbDevice;
extern GPIO_PinState led_set;
extern GPIO_PinState led_reset;
extern uint16_t len;
extern uint8_t report[8];
extern bool setting_up_encryption;
extern bool data_recieved;
extern int step;
extern uint8_t recieved[9];
extern uint8_t their_public_key[64];
extern uint8_t ok;
extern wchar_t* lang;

int* convert_message_hun(wchar_t* message, int *t, int* is_special, int length)
{
	//int length = wcslen(message);

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

int* convert_message_eng(wchar_t* message, int *t, int* is_special, int length)
{
	//int length = wcslen(message);

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

void send_hid(wchar_t* message, int length)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);

	//int length = wcslen(message);
	int t[length+1];
	int is_special[length+1];
	wchar_t* language;

	if(lang == NULL)
		language = HUN;
	else
		language = lang;

	int res = wcscmp(language,HUN);
	if(res == 0)
		convert_message_hun(message, t, is_special, length);
	else
		convert_message_eng(message, t, is_special, length);
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

void wait_for_response()
{
	ok ='\0';
	int8_t res[] = "kK";
	uint8_t msg[] = "Waiting for response (k)\n\r";
	CDC_Transmit(acm_id, msg, sizeof(msg) - 1);

	while (1)
	{
		// Check if the received message matches "k"
		if (ok==res[0] || ok==res[1])
		{
			CDC_Transmit(acm_id,(uint8_t*)"\n\r",3);
			HAL_Delay(100);
			break;
		}
	}
}

void set_keyboard_language(wchar_t* language)
{
	lang = language;
}
