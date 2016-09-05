/* 
* Display.cpp
*
* Created: 2016/08/13 11:49:13 PM
* Author: philb
*/


#include "../system.h"

// default constructor
Display::Display() : commandQueue(1, DISPLAY_MESSAGE_SIZE)
{
} //Display

// default destructor
Display::~Display()
{
} //~Display

typedef enum
{
  DISP_INIT,
}DISPLAY_STATE;

void Display::Run(FSM_CONTROL control)
{
  static DISPLAY_STATE state;

  if(control == RESET)
  {
    state = DISP_INIT;  
  }

  switch(state)
  {
    case DISP_INIT:
      commandQueue.Clear();
      break;

      

  }
}