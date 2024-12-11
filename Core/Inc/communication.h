#pragma once

#include "includes.h"

int* convert_message_hun(wchar_t*, int*, int*, int);
int* convert_message_eng(wchar_t*, int*, int*, int);
void send_hid(wchar_t*, int);
void wait_for_response();
void set_keyboard_language(wchar_t*);
