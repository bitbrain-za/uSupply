/* 
* MCP4716A0T.h
*
* Created: 2016/07/20 3:17:27 PM
* Author: Philip
*/


#ifndef __MCP4716A0T_H__
#define __MCP4716A0T_H__

// Command definitioins
#define MCP47X6_CMD_MASK       0x1F
#define MCP47X6_CMD_VOLDAC     0x00
#define MCP47X6_CMD_VOLALL     0x40
#define MCP47X6_CMD_VOLCONFIG  0x80
#define MCP47X6_CMD_ALL        0x60

typedef enum
{
  DAC_GAIN_1X = 0x0,
  DAC_GAIN_2X = 0x1,
}DAC_GAIN;

typedef enum
{
  VREF_VDD = 0x0,
  VREF_VREFPIN = 0x10,
  VREF_VREFPIN_BUFFERED = 0x18,
}DAC_VREF;

class MCP4716A0T
{
//variables
public:
protected:
private:
  const static U8 DeviceAddress = 0xC0;
  U8 config;
  float vref;

//functions
public:
	MCP4716A0T(DAC_VREF ref, DAC_GAIN gain);
	~MCP4716A0T();

  bool WriteConfig(U8 c);
  void SetReference(DAC_VREF ref, float value);
  void SetVoltage(float level);

protected:
private:
	MCP4716A0T( const MCP4716A0T &c );
	MCP4716A0T& operator=( const MCP4716A0T &c );

  bool WriteVolatileDACRegister(U16 level);
  bool WriteVolatileMemory(U16 level);
  bool WriterAllMemory(U16 level);

}; //MCP4716A0T

#endif //__MCP4716A0T_H__
