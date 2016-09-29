/* 
* Desktop.cpp
*
* Created: 2016/09/05 2:31:21 PM
* Author: Philip
*/


#include "../system.h"

typedef enum
{
  DISP_DESKTOP,
  DISP_MENU,
}DESKTOP_STATE;

// default constructor
Desktop::Desktop() : commandQueue(1, 0)
{
} //Desktop

// default destructor
Desktop::~Desktop()
{
} //~Desktop

void Desktop::FSM(FSM_CONTROL control)
{
  static DESKTOP_STATE state = DISP_DESKTOP;
  DESKTOP_CMD cmd;

  if(control == RESET)
  {

    display.init();
    display.ClearScreen(false);
    state = DISP_DESKTOP;
  }

  if(commandQueue.Peak((U8 *)&cmd))
  {
    commandQueue.Clear();

    switch(cmd)
    {
      case DSKTP_CMD_REFRESH:
      switch(state)
      {
        case DISP_DESKTOP:
          invalidate();
        break; 
      }
      break;
    }
  }

  switch(state)
  {
    case DISP_DESKTOP:
      DisplayDesktop();
      break;
  }
}

void Desktop::voltage(U16 val)
{
  voltage_read = val;
  voltage_changed = true;
}

void Desktop::ClearVoltageArea(bool invert)
{
  display.ClearLine(0, invert);
  display.ClearLine(1, invert);
  display.ClearLine(2, invert);
}

void Desktop::ClearSecondArea(bool invert)
{
  display.ClearLine(3, invert);
  display.ClearLine(4, invert);
  display.ClearLine(5, invert);
}

void Desktop::ClearMenuBar(bool invert)
{
  display.ClearLine(6, invert);
  display.ClearLine(7, invert);
}

void Desktop::DisplayDesktop()
{
  char str[32];

  display.SetFont(FONT_MED);

  /*
  Voltage Display
  */
  if(voltage_changed)
  {
    sprintf(str, "%d.%02d V", voltage_read/1000, (voltage_read%1000) / 10);
    display.GotoXY(0, 0);
    display.PutStr(str, false, JUST_CENTER);

    display.drawHorizontalLine(Point(0, 28), LCD_COLUMNS, false);
    voltage_changed = false;
  }
  
  /*
  Current Display
  */
  if(current_changed)
  {
    ClearSecondArea(false);
  }

  /*
    Menu Bar
  */
  
  if(menu_changed)
  {
    ClearMenuBar(false);

    display.SetFont(FONT_SMALL);
    
    sprintf(str, "Menu");
    DrawButton(0, 7, 32, str, FONT_SMALL, false);
    
    sprintf(str, "LIM");
    DrawButton(32, 7, 32, str, FONT_SMALL, disp_limit);
    
    sprintf(str, "Timer");
    DrawButton(64, 7, 32, str, FONT_SMALL, false);

    sprintf(str, "Calib");
    DrawButton(93, 7, 32, str, FONT_SMALL, false);

    menu_changed = false;
  }
}

void Desktop::DrawButton(U8 x, U8 page, U8 width, char *text, FONT_SIZE size, bool invert)
{
  U8 height = 1;
  switch(size)
  {
    case FONT_SMALL:
    height = 1;
    break;

    case FONT_MED:
    height = 3;
    break;

    case FONT_LARGE:
    height = 3;
    break;
  }

  U8 string_length = display.strwidth(text);
  
  if((string_length + 2) > width)
    text[width - 2] = '\0';

  if(x > 0)
  {
    display.drawVerticalLine(Point(x, page * 8), 8 * height, invert);
  }

  U8 string_offset = (width - string_length) / 2 + x;
  display.GotoXY(string_offset, page);
  display.PutStr(text, invert, JUST_NONE);

  if((x + width) < LCD_COLUMNS)
  {
    display.drawVerticalLine(Point(x + width, page * 8), 8 * height, invert);
  }
  
  display.drawHorizontalLine(Point(x, (page * 8) - 1), width, invert);
}