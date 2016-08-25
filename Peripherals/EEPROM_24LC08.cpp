/* 
* EEPROM_24LC08.cpp
*
* Created: 2016/07/20 2:39:46 PM
* Author: Philip
*/

#include "../system.h"

// default constructor
EEPROM_24LC08::EEPROM_24LC08(port* wp_port, U8 WPpinNumber) : write_protect(wp_port, WPpinNumber, true, true)
{
  WriteProtectEnable();
} 

// default destructor
EEPROM_24LC08::~EEPROM_24LC08()
{
} //~EEPROM_24LC08

EEPROM_RESULT EEPROM_24LC08::Write(U16 address, U8 *data, U8 length) 
{ 
  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  if(length == 0)
    return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;  
    
  U8 *buffer = (U8 *)malloc(1 + length);
  if(buffer == NULL)
    return EEPROM_ERROR_MEMORY;

  EEPROM_RESULT res = EEPROM_OKAY;

  buffer[0] = address_lower;

  memcpy(&buffer[1], data, length);
  WriteProtectDisable();
  if(!twi::WriteBytes(address_upper, buffer, length + 1 ))
  {
    res = EEPROM_ERROR_WRITING;
  }
  WriteProtectEnable();

  free(buffer);

  return res;
}

EEPROM_RESULT EEPROM_24LC08::Read(U16 address, U8 length, U8 *data) 
{ 
  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  if(length == 0)
    return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;  
    
  EEPROM_RESULT res = EEPROM_OKAY;

  if(!twi::WriteBytes(address_upper, &address_lower, 1 ))
  {
    res = EEPROM_ERROR_ADDRESS;
  }

  if(!twi::ReadBytes(address_upper, data, length ))
  {
    res = EEPROM_ERROR_READING;
  }

  return res;
}

EEPROM_RESULT EEPROM_24LC08::WriteAndVerify(U16 address, U8 length, U8 *data)
{
  U8 i = 0;
  EEPROM_RESULT res = EEPROM_OKAY;
  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;

  U8 *temp = (U8 *)malloc(length);
  if(temp == NULL)
    return EEPROM_ERROR_MEMORY;

  memset(temp, 0, length);

  res = Write(address, data, length);

  if(res != EEPROM_OKAY)
    return res;

  _delay_ms(500);

  res = Read(address, length, temp);

  if(res != EEPROM_OKAY)
    return res;

  for(i = 1; i < length ; i++)
  {
    if(temp[i] != data[i])
    {
      memcpy(data, temp, length);
      return EEPROM_ERROR_DATA_MISMATCH;
    }
  }

  return res;
}

bool EEPROM_24LC08::Test()
{
  U8 buffer[10];
  memset(buffer, 0xAA, 10);

  Read(0, 10, buffer);

  buffer[0] = 0x01;
  buffer[1] = 0x02;
  buffer[2] = 0x03;
  buffer[3] = 0x04;
  buffer[4] = 0x05;
  buffer[5] = 0x06;
  buffer[6] = 0x07;
  buffer[7] = 0x08;
  buffer[8] = 0x09;
  buffer[9] = 0x0a;

  if(WriteAndVerify(0x0000, 10, buffer) != EEPROM_OKAY)
    return false;

  return true;
}
