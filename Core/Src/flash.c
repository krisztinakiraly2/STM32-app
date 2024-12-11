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
		d[i] = *(uint32_t *) addr++;
	}
	HAL_FLASH_Lock();
}

void erase_data(uint32_t addr, uint32_t length)
{
	uint8_t erase = 0x00;

	HAL_FLASH_Unlock();

	for(int i=0; i<length; ++i)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, erase);
	}

	HAL_FLASH_Lock();

}

void test_flash()
{
	uint8_t erase = 0x00;
	int addr = 0x08012500;
	uint8_t ms[] = "Hihi hihi";
	uint8_t d[10];

	HAL_FLASH_Unlock();

	for(int i=0; i<10; ++i)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, ms[i]);
	}

	for(int i=0; i<10; ++i)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, erase);
	}

	HAL_FLASH_Lock();


	save_data(ms,10);
	read_data(d,10,0x08020000);
}

void save_profile()
{
	//name: Krisztina
	//email: krisztinakiraly2@gmail.com
	//username: krisztina
	//password: 123

	/*uint8_t* id;
	uint8_t* name;
	uint8_t* email;
	uint8_t* username;
	uint8_t* password;

	uint8_t pid[] = "id: ";
	uint8_t pname[] = "name: ";
	uint8_t pemail[] = "email: ";
	uint8_t pusername[] = "username: ";
	uint8_t ppassword[] = "password: ";

	save_data(pid,4);
	save_data(*id,strlen(id));
	*/
}
