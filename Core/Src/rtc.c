/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include <time.h>
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  time_t currentTime;
  struct tm *timeInfo;

  time(&currentTime);
  timeInfo = localtime(&currentTime);

  sTime.Hours = timeInfo->tm_hour;
  sTime.Minutes = timeInfo->tm_min;
  sTime.Seconds = timeInfo->tm_sec;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
	  Error_Handler();
  }

  switch(timeInfo->tm_mon + 1)
  {
	  case 1: sDate.Month = RTC_MONTH_JANUARY; break;
	  case 2: sDate.Month = RTC_MONTH_FEBRUARY; break;
	  case 3: sDate.Month = RTC_MONTH_MARCH; break;
	  case 4: sDate.Month = RTC_MONTH_APRIL; break;
	  case 5: sDate.Month = RTC_MONTH_MAY; break;
	  case 6: sDate.Month = RTC_MONTH_JUNE; break;
	  case 7: sDate.Month = RTC_MONTH_JULY; break;
	  case 8: sDate.Month = RTC_MONTH_AUGUST; break;
	  case 9: sDate.Month = RTC_MONTH_SEPTEMBER; break;
	  case 10: sDate.Month = RTC_MONTH_OCTOBER; break;
	  case 11: sDate.Month = RTC_MONTH_NOVEMBER; break;
	  case 12: sDate.Month = RTC_MONTH_DECEMBER; break;
  }

  sDate.Date = timeInfo->tm_mday;
  sDate.Year = timeInfo->tm_year + 100;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
	  Error_Handler();
  }
  /* USER CODE END RTC_Init 2 */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
