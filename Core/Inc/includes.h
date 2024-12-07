/*
 * includes.h
 *
 *  Created on: Dec 5, 2024
 *      Author: krisz
 */

#ifndef INC_INCLUDES_H_
#define INC_INCLUDES_H_

#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <wchar.h>
#include <time.h>

#include "usb_device.h"
#include "usbd_hid_keyboard.h"
#include "usbd_cdc_acm.h"
#include "usbd_cdc_acm_if.h"
#include "usb_otg.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

#include "uECC.h"

#define acm_id 0
#define ENG L"english"
#define HUN L"hungarian"
#define NUM_GENERATORS 3

#define SETUPCOMM 0

#endif /* INC_INCLUDES_H_ */
