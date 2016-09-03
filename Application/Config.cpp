/* 
* Config.cpp
*
* Created: 2016/09/03 3:54:30 PM
* Author: philb
*/


#include "../system.h"

#define DEFAULT_VOLTAGE 660

// default constructor
Config::Config() : eeprom(&hal::portE, 3)
{
} //Config

// default destructor
Config::~Config()
{
} //~Config

void Config::Load()
{
  U16 pattern;
  eeprom.Read(ADDR_PATTERN, 2, (U8 *)&pattern);

  if(pattern != 0x66)
  {
    Format();
    return;
  }
  eeprom.Read(ADDR_VOLTAGE, 2, (U8 *)&saved_voltage);
}

void Config::Save()
{
  if(ram_voltage != saved_voltage)
  {
    eeprom.WriteAndVerify(ADDR_VOLTAGE, 2, (U8 *)&ram_voltage);
    saved_voltage = ram_voltage;
  }
}

void Config::Format()
{
  U16 temp;

  temp = DEFAULT_VOLTAGE;
  eeprom.WriteAndVerify(ADDR_PATTERN, 2, (U8 *)&temp);

  temp = 0x66;
  eeprom.WriteAndVerify(ADDR_PATTERN, 2, (U8 *)&temp);
}