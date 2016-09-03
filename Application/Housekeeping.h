/* 
* Housekeeping.h
*
* Created: 2016/09/03 7:33:26 PM
* Author: philb
*/


#ifndef __HOUSEKEEPING_H__
#define __HOUSEKEEPING_H__


class Housekeeping
{
//variables
public:
protected:
private:
  timer house_keeping_tick;

//functions
public:
	Housekeeping();
	~Housekeeping();
  void Run(FSM_CONTROL control);

protected:
private:
	Housekeeping( const Housekeeping &c );
	Housekeeping& operator=( const Housekeeping &c );
}; //Housekeeping

#endif //__HOUSEKEEPING_H__
