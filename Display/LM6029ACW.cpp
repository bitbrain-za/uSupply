/* 
* LM6029ACW.cpp
*
* Created: 2016/07/21 1:01:27 PM
* Author: Philip
*/
#include "../system.h"
#include "fonts/small.h"
#include "fonts/sitka_med.h"

/*
Because of RAM limitations, we're going to hard-code the separators:
  1 - page 3, width 4px, bits 2-5
  2 - page 6, width 1px, bits 7
*/


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

  _font = &SitkaMed;
}

void LM6029ACW::ClearScreen(bool Invert)
{
  controller.Clear(Invert);
}

void LM6029ACW::ClearLine(U8 line, bool invert)
{
  controller.ClearLine(line, invert);
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

U8 LM6029ACW::strwidth(char *str)
{
  return _font->strwidth(str);
}

void LM6029ACW::PutStr(char *str, bool invert, JUSTIFICATION justification)
{
  unsigned int i=0;

  U8 string_length = _font->strwidth(str);
  
  switch(justification)
  {
    case JUST_LEFT:
      pos_x = 0;
    break;

    case JUST_RIGHT:
      pos_x = LCD_COLUMNS - string_length;
    break;

    case JUST_CENTER:
      pos_x = (LCD_COLUMNS - string_length) / 2;
    break;

    case JUST_NONE:

    break;
  }

  do
  {
    PutChar(str[i], invert);
    i++;
  }while(str[i]!='\0');
}


void LM6029ACW::PutChar(unsigned char c, bool invert)
{
  const U8* character = _font->FetchChar(c);
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

  for(j = 0 ; j < _font->height ; j ++)
  {

    controller.SetColumnAddress(pos_x);
    controller.SetPageAddress(pos_y + j);
    
    LCD_HW::ChipSelect();
    LCD_HW::DataMode();

    for(i = 0 ; i < length ; i++) 
    {
      data = pgm_read_byte(character++);

      /* Low RAM fix */
      if((pos_y + j) == 6)
      {
        data |= 0x80;
      }
      if((ex_state == SYS_DESKTOP) && (pos_y + j == 3))
      {
        data |= 0x18;
      }

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

 void LM6029ACW::SetFont(FONT_SIZE size)
 {
  switch(size)
  {
    case FONT_SMALL:
    _font = &SmallFont;
    break;
    
    case FONT_MED:
    _font = &SitkaMed;
    break;

    case FONT_LARGE:
    break;
  }
 }