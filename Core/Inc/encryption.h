#pragma once

#include "includes.h"
#include "communication.h"

void set_up_encryption();
void encrypt_and_decrypt_msg(uint8_t* msg, size_t len);

int my_rng_function(uint8_t *dest, unsigned size);
int generator_1();
int generator_2();
int generator_3();
int choose_generator();
uint8_t get_pseudorandom_number();


