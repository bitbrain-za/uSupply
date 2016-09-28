/* 
* LM6029ACW.cpp
*
* Created: 2016/07/21 1:01:27 PM
* Author: Philip
*/
#include "../system.h"
#include "fonts/small.h"
#include "fonts/sitka_med.h"

font LM6029ACW::SmallFont;
font LM6029ACW::SitkaMed;

// default constructor
LM6029ACW::LM6029ACW()
{
  pos_x = 0;
  pos_y = 0;
} //LM6029ACW

// default destructor
LM6029ACW::~LM6029ACW()
{
} //~LM6029ACW

void LM6029ACW::init()
{
  controller.init();
  ClearScreen(false);
  SmallFont.map = small_font;
  SmallFont.height = 1;

  SitkaMed.map = sitka_med_font;
  SitkaMed.height = 3;
}

void LM6029ACW::ClearScreen(bool Invert)
{
  controller.Clear(Invert);
}

void LM6029ACW::GotoXY(unsigned char x, unsigned char y)
{
  if(x < LCD_COLUMNS - 1)
    pos_x = x;
  else
    pos_x = 0;
  
  if(y < LCD_ROWS)
    pos_y = y;
  else
    pos_y = 0;
}

void LM6029ACW::PutStr(char *str, bool invert)
{
  unsigned int i=0;
  
  do
  {
    PutChar(str[i], invert);
    i++;
  }while(str[i]!='\0');
}


void LM6029ACW::PutChar(unsigned char c, bool invert)
{
  const U8* character = SitkaMed.FetchChar(c);
  U8 length = pgm_read_byte(character++);
  int i = 0;
  int j = 0;
  U8 data = 0;
  
  if((pos_x + length) >= LCD_HW::COLUMNS)
  {
    pos_x = 0;
    if(pos_y == (LCD_ROWS - 1))
      pos_y = 0;
    else
      pos_y++;
  }
  else
    pos_x++;

  for(j = 0 ; j < SitkaMed.height ; j ++)
  {

    controller.SetColumnAddress(pos_x);
    controller.SetPageAddress(pos_y + j);
    
    LCD_HW::ChipSelect();
    LCD_HW::DataMode();

    for(i = 0 ; i < length ; i++) 
    {
      data = pgm_read_byte(character++);
      if(invert)
        data = ~data;

      LCD_HW::SetData(data);
      LCD_HW::TriggerWrite();
    }
    LCD_HW::ChipDeselect();
  }


  pos_x += length;
}
 
void LM6029ACW::WriteCharCol(U8 v, U8 x, U8 page, U8 colour)
{
  if(colour == 1)
    v = ~v;

  LCD_HW::DataMode();
  LCD_HW::SetData(v);
  LCD_HW::TriggerWrite();
}