/* 
* LM6029ACW.h
*
* Created: 2016/07/21 1:01:27 PM
* Author: Philip
*/


#ifndef __LM6029ACW_H__
#define __LM6029ACW_H__

#define LCD_ROWS 8
#define LCD_COLUMNS 128

typedef enum
{
  JUST_LEFT,
  JUST_CENTER,
  JUST_RIGHT,
  JUST_NONE
}JUSTIFICATION;

typedef enum
{
  FONT_SMALL,
  FONT_MED,
  FONT_LARGE,
}FONT_SIZE;

class LM6029ACW
{
//variables
public:
protected:
private:
  ST7565R controller;
  U8 pos_x;
  U8 pos_y;
  static font SmallFont;
  static font SitkaMed;
  font *_font;

//functions
public:
	LM6029ACW();
	~LM6029ACW();

  void init();
  void GotoXY(unsigned char x, unsigned char y);
  void PutStr(char *str, bool invert, JUSTIFICATION justification);
  void PutChar(unsigned char c, bool invert);
  void ClearScreen(bool Invert);
  void ClearLine(U8 line, bool invert);
  void SetFont(FONT_SIZE size);
  void drawPoint(Point pt);
  void drawLine(Line line);
  void drawHorizontalLine(Point start, U8 length, bool invert);
  void drawVerticalLine(Point start, U8 length, bool invert);
  U8 strwidth(char *str);

protected:
private:
	LM6029ACW( const LM6029ACW &c );
	LM6029ACW& operator=( const LM6029ACW &c );

  void WriteCharCol(U8 v, U8 x, U8 page, U8 colour);
  void DrawChar(unsigned char c, unsigned char x, unsigned char page, unsigned char color);

}; //LM6029ACW

#endif //__LM6029ACW_H__
