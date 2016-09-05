/* 
* Display.h
*
* Created: 2016/08/13 11:49:13 PM
* Author: philb
*/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DISPLAY_MESSAGE_SIZE 1 + 32

typedef enum
{
  DISP_CMD_CLEAR,
  DISP_CMD_VOLTAGE,
  DISP_CMD_CURRENT,
  DISP_CMD_BUTTON,
}DISP_CMD;

typedef struct
{
  DISP_CMD command; 
  U8 *message;
}DISP_MESSAGE;

class Display
{
//variables
public:
protected:
private:
  MessageQueue commandQueue;

//functions
public:
	Display();
	~Display();

  void Run(FSM_CONTROL control);
protected:
private:
	Display( const Display &c );
	Display& operator=( const Display &c );

}; //Display

#endif //__DISPLAY_H__
