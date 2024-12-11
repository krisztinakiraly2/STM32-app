#include "encryption.h"
#include "chacha20.h"

extern bool res;
extern USBD_HandleTypeDef hUsbDevice;
extern wchar_t del_str[2];
extern wchar_t* message_hun;

extern const struct uECC_Curve_t * curve;
extern int private_key_length;
extern int public_key_length;
extern int secret_length;
extern uint8_t my_private_key[32];
extern uint8_t secret[32];
extern uint8_t my_public_key[64];
extern uint8_t their_public_key[64];
extern ChaCha20_Ctx ctx;
extern nonce96_t nonce;
extern uint32_t count;
extern RTC_HandleTypeDef hrtc;
extern bool data_recieved;
extern bool setting_up_encryption;
extern int step;
extern uint8_t data[9];
extern int pos;
extern GPIO_PinState led_set;
extern GPIO_PinState led_reset;

void set_up_encryption()
{
	setting_up_encryption = true;
	srand(time(NULL));
	uECC_set_rng(my_rng_function);
	const struct uECC_Curve_t* curve = uECC_secp256r1();
	uECC_make_key(my_public_key, my_private_key, curve);
	res=false;
	count=1;

	while(!res)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);
		char num_str[10];
		for(int i=0; i<public_key_length; ++i)
		{
			snprintf(num_str, sizeof(num_str), "%d", my_public_key[i]);
			CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
			HAL_Delay(50);
			CDC_Transmit(acm_id,(uint8_t*)"\n",2);
			HAL_Delay(50);
		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);

		HAL_Delay(100);

		step=READ_PUBLIC_KEY;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);
		while(!data_recieved);
		data_recieved = false;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);

		if(uECC_valid_public_key(their_public_key, curve)==1)
		{
			uECC_shared_secret(their_public_key, my_private_key, secret, curve);
		}
		else
			CDC_Transmit(acm_id,(uint8_t*)"E",2);

		CDC_Transmit(acm_id,(uint8_t*)"\n",2);
		HAL_Delay(50);

		uint8_t msg[] = "Comm set\n";
		uint8_t m[] = "Comm set";
		encrypt_and_decrypt_msg(msg, 10);
		msg[9] = '\n';

		HAL_Delay(200);

		CDC_Transmit(acm_id,msg,10);
		HAL_Delay(100);

		step=READ_REPLY_MESSAGE; pos=0;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_set);
		while(!data_recieved)
		{
			HAL_Delay(500);
			if(!data_recieved)
				CDC_Transmit(acm_id,msg,10);
		}
		data_recieved = false;
		step=READ_COMMAND;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, led_reset);

		data[8]='\0';

		HAL_Delay(100);

		encrypt_and_decrypt_msg(data,8);

		res=true;
		for(int i=0; i<8; ++i)
		{
			if(m[i]!=data[i])
				res=false;
		}

		HAL_Delay(100);
		if(res)
		{
			CDC_Transmit(acm_id, (uint8_t*)"Yes\n", 5);
		}
		else
		{
			CDC_Transmit(acm_id, (uint8_t*)"No\n", 4);
		}

		HAL_Delay(100);
	}

	setting_up_encryption = false;
}

void encrypt_and_decrypt_msg(uint8_t* msg, size_t len)
{
	ChaCha20_init(&ctx, secret, nonce, count++);
	ChaCha20_xor(&ctx, msg, len);
}

int my_rng_function(uint8_t *dest, unsigned size)
{
    if (dest == NULL || size == 0)
    {
        return 0;
    }

    for(int i=0; i<size; ++i)
    {
    	dest[i] = get_pseudorandom_number();
    }
    return 1;
}

int generator_1() { return rand(); }

int generator_2() { return rand() * rand(); }

int generator_3() { return rand() ^ (rand() << 5); }

int choose_generator() { return rand() % NUM_GENERATORS; }

uint8_t get_pseudorandom_number()
{
    int generator_choice = choose_generator();
    int result;

    switch (generator_choice)
    {
        case 0: result = generator_1(); break;
        case 1: result = generator_2(); break;
        case 2: result = generator_3(); break;
        default: result = 0;
    }

    RTC_TimeTypeDef sTime;
    uint32_t Format = RTC_FORMAT_BCD;

    if(HAL_RTC_GetTime(&hrtc, &sTime, Format) == HAL_OK)
    {
    	uint32_t timer_value = 0;

    	timer_value |= (uint32_t)sTime.Hours << 24;
		timer_value |= (uint32_t)sTime.Minutes << 16;
		timer_value |= (uint32_t)sTime.Seconds << 8;
		timer_value |= (sTime.SecondFraction & 0xFF);
    	result ^= (timer_value & 0xFF);
    }

    return (uint8_t)(result & 0xFF);
}
