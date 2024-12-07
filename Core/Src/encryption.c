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

void set_up_encryption()
{
	srand(time(NULL));
	uECC_set_rng(my_rng_function);
	const struct uECC_Curve_t* curve = uECC_secp256r1();
	uECC_make_key(my_public_key, my_private_key, curve);

	while(!res)
	{
		char num_str[10];
		HAL_Delay(2000);

		CDC_Transmit(acm_id, (uint8_t*)"Starting setup\n\r", 17);
		HAL_Delay(200);
		wait_for_response();

		for(int i=0; i<public_key_length; ++i)
		{
			snprintf(num_str, sizeof(num_str), "%d", my_public_key[i]);
			CDC_Transmit(acm_id,(uint8_t*)num_str,strlen(num_str));
			HAL_Delay(100);
		}

		CDC_Transmit(acm_id,(uint8_t*)"Waiting for public key: (xx)\n\r\r",32);
		HAL_Delay(100);
		uint8_t buffer[2];

		receive_message(buffer, 2); //TODO: this will need to be 32 long

		// TODO: remove from real code
		uint8_t their_private_key[20];
		uECC_make_key(their_public_key, their_private_key, curve);
		//

		if(uECC_valid_public_key(their_public_key, curve)==1)
		{
			uECC_shared_secret(their_public_key, my_private_key, secret, curve);
		}
		else
			CDC_Transmit(acm_id,(uint8_t*)"Oh oh:\n\r",9);

		CDC_Transmit(acm_id,(uint8_t*)"Sending encrypted message:\n\r",29);
		HAL_Delay(100);
		uint8_t msg[] = "Comm set";
		uint8_t m[] = "Comm set";
		encrypt_and_decrypt_msg(msg, 9);

		CDC_Transmit(acm_id,msg,9);
		HAL_Delay(100);
		CDC_Transmit(acm_id,(uint8_t*)"\n\r",3);
		HAL_Delay(100);

		//CDC_Transmit(acm_id,(uint8_t*)"Waiting for reply message:\n\r",28);
		//HAL_Delay(100);

		uint8_t r[9];
		//receive_message(r, 8);
		r[9]='\0';

		// TODO: remove from real code
		for(int i=0; i<9; ++i)
			r[i]=msg[i];
		uint8_t their_secret[32];
		uECC_shared_secret(my_public_key, their_private_key, their_secret, curve);
		ChaCha20_init(&ctx, their_secret, nonce, 1);
		ChaCha20_xor(&ctx, r, 9);

		ChaCha20_init(&ctx, their_secret, nonce, 2);
		ChaCha20_xor(&ctx, r, 9);
		//

		HAL_Delay(100);
		CDC_Transmit(acm_id,(uint8_t*)"\n\n\rReceived message:\n\r\r",24);
		HAL_Delay(100);
		CDC_Transmit(acm_id,r,9);
		HAL_Delay(100);

		encrypt_and_decrypt_msg(r,9);

		res=true;
		for(int i=0; i<8; ++i)
		{
			if(m[i]!=r[i])
				res=false;
		}

		CDC_Transmit(acm_id,(uint8_t*)"\n\n\rAre they equal:\n\r\r",22);
		HAL_Delay(100);
		if(res)
		{
			CDC_Transmit(acm_id, (uint8_t*)"Yes\n\n\r", 7);
		}
		else
		{
			CDC_Transmit(acm_id, (uint8_t*)"No\n\n\r", 6);
		}

		HAL_Delay(100);
	}
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


