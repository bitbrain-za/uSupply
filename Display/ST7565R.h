/* 
* ST7565R.h
*
* Created: 2016/07/26 1:07:12 PM
* Author: Philip
*/

#if 1


#ifndef __ST7565R_H__
#define __ST7565R_H__

typedef enum
{
  DISP_ON = 0xAF,
  DISP_OFF = 0xAE,
  DISP_START_LINE_SET = 0x40,
  DISP_SET_PAGE_ADDRESS = 0xB0,
  DISP_SET_COLUMN_ADDRESS_UPPER = 0x10,
  DISP_SET_COLUMN_ADDRESS_LOWER = 0x00,
  DISP_ADC_NORMAL = 0xA0,
  DISP_ADC_REVERSE = 0xA1,
  DISP_NORMAL= 0xA6,
  DISP_REVERSE= 0xA7,
  DISP_ENTIRE_DISP_NORMAL = 0xA4,
  DISP_ENTIRE_DISP_ON = 0xA5,
  DISP_LCD_BIAS_DIV7 = 0xA3,
  DISP_LCD_BIAS_DIV9 = 0xA2,
  DISP_SET_RMW_MODE = 0xE0,
  DISP_RESET_RMW_MODE = 0xEE,
  DISP_RESET = 0xE2,
  DISP_POWER_CONTROL_SET = 0x28,
  DISP_REGULATOR_RESISTOR_RATIO = 0x20,
  DISP_SHL_NORMAL = 0xC0,
  DISP_SHL_FLIPPED = 0xC8,
  DISP_SET_POWER_MODE = 0x28,
  DISP_ELECTRONIC_VOLUME_MODE_SET = 0x81,
  DISP_SLEEP_MODE_SELECT_SLEEP = 0xAD,
  DISP_SLEEP_MODE_SELECT_NORMAL = 0xAC,
  DISP_SLEEP_MODE_TRIGGER = 0x00,
  DISP_NOP = 0xE3,
}DISPLAY_COMMANDS;

class ST7565R
{
//variables
public:
protected:
private:
  static const U8 COLUMN_OFFSET = 0x00;

//functions
public:
	ST7565R();
	~ST7565R();

  void init();
  bool SetStartLine(U8 line);
  bool SetPageAddress(U8 page_address);
  bool SetColumnAddress(U8 column_address);
  bool SetContrast(U8 val);
  void Clear(bool Invert);
  U8 PutChar(char c);

protected:
private:
	ST7565R( const ST7565R &c );
	ST7565R& operator=( const ST7565R &c );

  void SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter);

  void SendData(U8 val);
  void SendCommand(U8 val);
}; //ST7565R

#endif //__ST7565R_H__

#endif