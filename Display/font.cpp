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