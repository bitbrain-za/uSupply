/* 
* font.h
*
* Created: 2016/07/29 12:19:11 PM
* Author: Philip
*/


#ifndef __FONT_H__
#define __FONT_H__

class font
{
//variables
public:
  const U8 **map;
protected:
private:

//functions
public:
	font();
	~font();

  U8 height;
  const U8* FetchChar(char c);

protected:
private:
	font( const font &c );
	font& operator=( const font &c );

}; //font

#endif //__FONT_H__
