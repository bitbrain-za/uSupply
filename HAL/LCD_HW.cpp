/* 
* LCD_HW_interface.cpp
*
* Created: 2016/07/27 1:17:34 PM
* Author: Philip
*/

#include "../system.h"

void LCD_HW::SetData(U8 val)
{
  PORTD = pgm_read_byte(BitReverseTable256 + val);
}