/* 
* ST7565R.cpp
*
* Created: 2016/07/26 1:07:12 PM
* Author: Philip
*/

#if 1

#include "../system.h"


// default constructor
ST7565R::ST7565R()
{
} //ST7565R

// default destructor
ST7565R::~ST7565R()
{
} //~ST7565R

void ST7565R::init()
{
  LCD_HW::RDPinSet();
  LCD_HW::ChipSelect();

  LCD_HW::Reset();
 
  _delay_ms(500);

  SendCommand(DISP_LCD_BIAS_DIV9);
  SendCommand(DISP_ADC_NORMAL);
  SendCommand(DISP_SHL_FLIPPED);
  SetContrast(0x20);

  SetPowerMode(true, true, true);
  SetStartLine(0);

  LCD_HW::ChipSelect();
  SendCommand(DISP_ENTIRE_DISP_NORMAL);

  SendCommand(DISP_NORMAL);
  SendCommand(DISP_ON);

  SetPageAddress(0x00);
  SetColumnAddress(0x00);
  
  LCD_HW::ChipDeselect();
}

bool ST7565R::SetStartLine(U8 line)
{
  LCD_HW::ChipSelect();
  if(line > 0x3F)
    return false;

  SendCommand(DISP_START_LINE_SET | line);
  LCD_HW::ChipDeselect();
  return true;
}

bool ST7565R::SetPageAddress(U8 page_address)
{
  LCD_HW::ChipSelect();
  if(page_address > 0x0F)
    return false;

  SendCommand(DISP_SET_PAGE_ADDRESS | page_address);
  LCD_HW::ChipDeselect();
  return true;
}

bool ST7565R::SetColumnAddress(U8 column_address)
{
  LCD_HW::ChipSelect();
  U8 upper = (column_address >> 4) & 0x0F;
  U8 lower = column_address & 0x0F;

  SendCommand(DISP_SET_COLUMN_ADDRESS_UPPER | upper);
  SendCommand(DISP_SET_COLUMN_ADDRESS_LOWER | lower);

  LCD_HW::ChipDeselect();
  return true;
}

bool ST7565R::SetContrast(U8 val)
{
  LCD_HW::ChipSelect();
  if(val > 0x3F)
    return false;
  
  SendCommand(DISP_ELECTRONIC_VOLUME_MODE_SET);
  SendCommand(val);
 
  LCD_HW::ChipDeselect();
  return true;
}

void ST7565R::SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter)
{
  LCD_HW::ChipSelect();
  U8 val = 0x00;

  val += (VoltageFollower) ? 0x01 : 0x00;
  val += (VoltageRegulator) ? 0x02 : 0x00;
  val += (VoltageConverter) ? 0x04 : 0x00;

  SendCommand(DISP_SET_POWER_MODE | val);
  LCD_HW::ChipDeselect();
}

void ST7565R::SendData(U8 val)
{
  LCD_HW::DataMode();
  LCD_HW::SetData(val);
  LCD_HW::TriggerWrite();
}

void ST7565R::SendCommand(U8 val)
{
  LCD_HW::CommandMode();
  LCD_HW::SetData(val);
  LCD_HW::TriggerWrite();
}

void ST7565R::Clear(bool Invert)
{
	unsigned char i;
	
	for(i=0 ; i < LCD_HW::PAGES ; i++)
	{
    ClearLine(i, Invert);
  }
}

void ST7565R::ClearLine(U8 line, bool Invert)
{
  U8 data = (Invert) ? 0xFF : 0x00;
	unsigned char j;

  if(line == 6)
  {
    data ^= 0x80;
  }

  if((ex_state == SYS_DESKTOP) && (line == 3))
  {
    data ^= 0x18;
  }
	
  SetColumnAddress(0);
  SetPageAddress(line);

  LCD_HW::ChipSelect();
  LCD_HW::DataMode();

  for(j = 0 ; (j < LCD_HW::COLUMNS) ; j++)
  {
    if((line == 7) && ((j % 32) == 0))
      LCD_HW::SetData(~data);
    else
      LCD_HW::SetData(data);

    LCD_HW::TriggerWrite();
  }
  LCD_HW::ChipDeselect();
}

U8 ST7565R::PutChar(char c)
{
  return 0;
}

#endif