#include "flash.h"

extern uint32_t address;

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

void test_flash()
{
	  HAL_FLASH_Unlock();
	  FLASH_Erase_Sector(5, FLASH_VOLTAGE_RANGE_3);
	  HAL_FLASH_Lock();

	  uint8_t ms[] = "Hihi hihi"; uint8_t d[10];
	  save_data(ms,10);
	  read_data(d,10,0x08020000);
}
