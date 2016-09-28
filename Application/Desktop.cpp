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
          DisplayDesktop();
        break; 
      }
      break;
    }
  }


}

void Desktop::DisplayDesktop()
{
  char str[32];

  display.ClearScreen(false);

  sprintf(str, "%d.%02d V", voltage_read/1000, (voltage_read%1000) / 10);
  display.GotoXY(20, 0);
  display.PutStr(str, false);
}