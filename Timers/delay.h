/* 
* delay.h
*
* Created: 2016/07/20 1:17:50 PM
* Author: Philip
*/


#ifndef __DELAY_H__
#define __DELAY_H__


class delay
{
//variables
public:
protected:
private:

//functions
public:
  static void init();
  static void delay_us(U16 us);
  static void delay_ms(U16 ms) {;}
  static void delay_s(U16 seconds) {;}
protected:
private:

}; //delay

#endif //__DELAY_H__
