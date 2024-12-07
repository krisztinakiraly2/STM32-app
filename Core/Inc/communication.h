#pragma once

#include "includes.h"

int* convert_message_hun(wchar_t*, int*, int*);
int* convert_message_eng(wchar_t*, int*, int*);
void send_hid(wchar_t*, wchar_t*);

void receive_message(uint8_t* buffer, uint8_t length);
void wait_for_response();

int read_command_code(int length);
