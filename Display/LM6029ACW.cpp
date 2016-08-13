/* 
* LM6029ACW.cpp
*
* Created: 2016/07/21 1:01:27 PM
* Author: Philip
*/
#include "../system.h"
#include "fonts/small.h"

font LM6029ACW::SmallFont;

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
}

void LM6029ACW::ClearScreen(bool Invert)
{
  controller.Clear(Invert);
}

void LM6029ACW::GotoXY(unsigned char x, unsigned char y)
{
  if(x<=20)
    pos_x = x;
  else
    pos_x = 0;
  
  if(y<=7)
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
  const U8* character = SmallFont.FetchChar(c);
  U8 length = pgm_read_byte(character++);
  int i = 0;
  U8 data = 0;
  
  if((pos_x + length) >= LCD_HW::COLUMNS)
  {
    pos_x = 0;
    if(pos_y==7)
      pos_y = 0;
    else
      pos_y++;
  }
  else
    pos_x++;
    
  controller.SetColumnAddress(pos_x);
  controller.SetPageAddress(pos_y); 
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