/* 
* font.cpp
*
* Created: 2016/07/29 12:19:11 PM
* Author: Philip
*/


#include "../system.h"


// default constructor
font::font()
{
} //font

// default destructor
font::~font()
{
} //~font

const U8* font::FetchChar(char c)
{
  return (map[(U8)c - 0x20]);
}

U8 font::strwidth(char *str)
{
  unsigned int i=0;
  U8 length = 0;
  
  do
  {
    const U8* character = FetchChar(str[i]);
    length += pgm_read_byte(character++);
    i++;
  }while(str[i]!='\0');

  return length;
}