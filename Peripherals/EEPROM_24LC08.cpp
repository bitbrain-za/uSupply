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
  if(length == 0)
    return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;  
    
  U8 *buffer = (U8 *)malloc(2 + length);
  if(buffer == NULL)
    return EEPROM_ERROR_MEMORY;

  EEPROM_RESULT res = EEPROM_OKAY;

  buffer[0] = DeviceAddress;
  buffer[0] |= (U8)((address & 0x03) >> 7);
  buffer[1] = (U8)(address & 0xFF);

  memcpy(&buffer[2], data, length);
  WriteProtectDisable();
  if(!twi_master::USI_TWI_Start_Transceiver_With_Data( buffer, 2 ) > 0)
  {
    res = EEPROM_ERROR_WRITING;
  }
  WriteProtectEnable();

  free(buffer);

  return res;
}

EEPROM_RESULT EEPROM_24LC08::Read(U16 address, U8 length, U8 *data) 
{ 
  if(length == 0)
    return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;  
    
  U8 *buffer = (U8 *)malloc(1 + length);
  if(buffer == NULL)
    return EEPROM_ERROR_MEMORY;

  EEPROM_RESULT res = EEPROM_OKAY;

  buffer[0] = DeviceAddress;
  buffer[0] |= (U8)((address & 0x03) >> 7);
  buffer[1] = (U8)(address & 0xFF);

  if(!twi_master::USI_TWI_Start_Transceiver_With_Data( buffer, 2 ) > 0)
  {
    res = EEPROM_ERROR_ADDRESS;
  }

  buffer[0] = DeviceAddress | 0x01;

  if(!twi_master::USI_TWI_Start_Transceiver_With_Data( buffer, length ) > 0)
  {
    res = EEPROM_ERROR_READING;
  }

  memcpy(data, &buffer[1], length);
  free(buffer);
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

  res = Write(address, data, length);

  if(res != EEPROM_OKAY)
    return res;

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
  static U8 state = 0;
  U8 buffer[10];
  U8 temp;
  memset(buffer, 0x55, 10);

  if(WriteAndVerify(0x0000, 10, buffer) != EEPROM_OKAY)
    return false;

  return true;
}