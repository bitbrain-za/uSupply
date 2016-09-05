/* 
* Desktop.h
*
* Created: 2016/09/05 2:31:22 PM
* Author: Philip
*/


#ifndef __DESKTOP_H__
#define __DESKTOP_H__


class Desktop
{
//variables
public:
  MessageQueue commandQueue;
protected:
private:

//functions
public:
	Desktop();
	~Desktop();

  void Run(FSM_CONTROL control);
protected:
private:
	Desktop( const Desktop &c );
	Desktop& operator=( const Desktop &c );

}; //Desktop

#endif //__DESKTOP_H__
