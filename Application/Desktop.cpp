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
  display.ClearLine(6, invert);
}

void Desktop::ClearMenuBar(bool invert)
{
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
    ClearVoltageArea(false);
    sprintf(str, "%d.%02d V", voltage_read/1000, (voltage_read%1000) / 10);
    display.GotoXY(0, 0);
    display.PutStr(str, false, JUST_CENTER);

    display.drawHorizontalLine(0, 28, LCD_COLUMNS, false);
    voltage_changed = false;
  }
  
  /*
  Current Display
  */
  if(current_changed)
  {
    ClearSecondArea(false);
    sprintf(str, "%d.%03d mA", voltage_read/1000, (voltage_read%1000) / 10);
    display.GotoXY(0, 4);
    display.PutStr(str, false, JUST_CENTER);
    display.drawHorizontalLine(0, 28, LCD_COLUMNS, false);
    current_changed = false;
  }

  /*
    Menu Bar
  */
  
  if(menu_changed)
  {
    ClearMenuBar(true);

    display.SetFont(FONT_SMALL);
    
    sprintf(str, "Menu");
    DrawButton(0, str, true);
    
    sprintf(str, "LIM");
    DrawButton(1, str, !disp_limit);
    
    sprintf(str, "Timer");
    DrawButton(2, str, true);

    sprintf(str, "Calib");
    DrawButton(3, str, true);

    menu_changed = false;
  }
}

void Desktop::DrawButton(U8 pos, char *text, bool invert)
{
  U8 width = 32;
  U8 height = 1;
  U8 x = 32 * pos;

  U8 string_length = display.strwidth(text);
  
  if((string_length + 2) > x)
    text[width - 2] = '\0';

  if(pos > 0)
  {
    display.drawVerticalLine(x, 7, 8, invert);
  }

  U8 string_offset = (width - string_length) / 2 + x;
  display.GotoXY(string_offset, 7);
  display.PutStr(text, invert, JUST_NONE);

//  display.drawHorizontalLine(x, 54, width, invert);
}