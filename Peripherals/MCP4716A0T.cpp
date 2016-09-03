/* 
* MCP4716A0T.cpp
*
* Created: 2016/07/20 3:17:27 PM
* Author: Philip
*/


#include "../system.h"

// default constructor
MCP4716A0T::MCP4716A0T(DAC_VREF ref, DAC_GAIN gain)
{
  config = 0x00 | ref | gain;
}

// default destructor
MCP4716A0T::~MCP4716A0T()
{
} //~MCP4716A0T

void MCP4716A0T::SetReference(DAC_VREF ref, U16 value)
{
  config &= ~(0x18);
  config |= ref;

  vref = value;

  WriteConfig(config);
}

void MCP4716A0T::SetVoltage(U16 level)
{
  WriteVolatileDACRegister(level / 20);
}

bool MCP4716A0T::WriteVolatileDACRegister(U16 level)
{
  U8 buffer[2];

  level &= 0x3FF;
  level = level << 2;

  buffer[0] = (U8)(level >> 8);
  buffer[1] = (U8)(0xFF & level);

  return twi::WriteBytes(DeviceAddress, buffer, 2);
}

bool MCP4716A0T::WriteVolatileMemory(U16 level)
{
  U8 buffer[3];
  buffer[0] = MCP47X6_CMD_VOLALL | config;

  level &= 0x3FF;
  level = level << 6;

  buffer[1] = level >> 8;
  buffer[2] = (0xFF & level);

  return twi::WriteBytes(DeviceAddress, buffer, 3);
}

bool MCP4716A0T::WriterAllMemory(U16 level)
{
  U8 buffer[3];
  buffer[0] = MCP47X6_CMD_ALL | config;

  level &= 0x3FF;
  level = level << 6;

  buffer[1] = level >> 8;
  buffer[2] = (0xFF & level);

  return twi::WriteBytes(DeviceAddress, buffer, 3);
}

bool MCP4716A0T::WriteConfig(U8 c)
{
  c |= MCP47X6_CMD_VOLCONFIG;
  return twi::WriteBytes(DeviceAddress, &c, 1);
}