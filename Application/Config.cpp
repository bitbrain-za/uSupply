/* 
* Config.cpp
*
* Created: 2016/09/03 3:54:30 PM
* Author: philb
*/


#include "../system.h"

#define DEFAULT_VOLTAGE 660
#define DEFAULT_CURRENT 1000

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
  eeprom.Read(ADDR_CURRENT, 2, (U8 *)&saved_current);
}

void Config::Save()
{
  if(ram_voltage != saved_voltage)
  {
    eeprom.WriteAndVerify(ADDR_VOLTAGE, 2, (U8 *)&ram_voltage);
    saved_voltage = ram_voltage;
  }

  if(ram_current != saved_current)
  {
    eeprom.WriteAndVerify(ADDR_CURRENT, 2, (U8 *)&ram_current);
    saved_current = ram_current;
  }
}

void Config::Format()
{
  U16 temp;

  temp = DEFAULT_VOLTAGE;
  eeprom.WriteAndVerify(ADDR_VOLTAGE, 2, (U8 *)&temp);

  temp = DEFAULT_CURRENT;
  eeprom.WriteAndVerify(ADDR_CURRENT, 2, (U8 *)&temp);

  temp = 0x66;
  eeprom.WriteAndVerify(ADDR_PATTERN, 2, (U8 *)&temp);
}