/*
 * uSupply.cpp
 *
 * Created: 2016/07/20 10:40:38 AM
 * Author : Philip
 */ 

#define MAIN_C

#include "system.h"


int main(void)
{
//  LCD display;
  LM6029ACW display;
  SystemClock::init();

  hal::board_init();

  //lcd.init();
  display.init();


  while (1) 
  {
    _delay_ms(500);
    hal::climitLed.Set();

    
    display.GotoXY(20, 3);               //move cursor to row, col
    display.ClearScreen(true);                  //clear all the pixels on the display
    display.PutStr("Hello World!", true);  //print text in black pixels
    

    _delay_ms(500);
    hal::climitLed.Clear();

    
    display.GotoXY(20, 3);               //move cursor to row, col
    display.ClearScreen(false);                  //clear all the pixels on the display
    display.PutStr("Hello World!", false);  //print text in black pixels  }
    
  }
}

