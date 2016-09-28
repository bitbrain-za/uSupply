/* 
* Desktop.h
*
* Created: 2016/09/05 2:31:22 PM
* Author: Philip
*/


#ifndef __DESKTOP_H__
#define __DESKTOP_H__


typedef enum
{
  DSKTP_CMD_RESET,
  DSKTP_CMD_REFRESH,
}DESKTOP_CMD;

class Desktop
{
//variables
public:
  MessageQueue commandQueue;
  U16 voltage_read;

protected:
private:
  LM6029ACW display;

//functions
public:
	Desktop();
	~Desktop();

  void FSM(FSM_CONTROL control);
protected:
private:
	Desktop( const Desktop &c );
	Desktop& operator=( const Desktop &c );

  void DisplayDesktop();

}; //Desktop

#endif //__DESKTOP_H__
