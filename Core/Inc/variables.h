#pragma once

#include "includes.h"
#include "chacha20.h"
#include "uECC.h"

GPIO_PinState led_set = GPIO_PIN_RESET;
GPIO_PinState led_reset = GPIO_PIN_SET;
GPIO_PinState is_pushed = GPIO_PIN_RESET;

uint16_t len = 8;
uint8_t report[8] = {0};

GPIO_PinState button;
wchar_t* message_hun;
wchar_t del_str[2];

bool res = false;
uint8_t my_private_key[32];
uint8_t secret[32];
uint8_t my_public_key[64];
uint8_t their_public_key[64];

int private_key_length = 32;
int public_key_length = 64;
int secret_length = 32;

uint32_t count = 0x1;
ChaCha20_Ctx ctx;
nonce96_t nonce =
{
	0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a,
	0x00, 0x00, 0x00, 0x00,
};

uint32_t address = 0x08012500;
bool data_recieved = false;
bool setting_up_encryption = false;
int step = 4;
uint8_t ok;

wchar_t* lang;
uint8_t command[] = "Login req\n";
wchar_t* tab = L"\t";
