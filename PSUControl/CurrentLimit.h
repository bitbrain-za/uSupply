/* 
* CurrentLimit.h
*
* Created: 2016/07/28 10:27:33 AM
* Author: Philip
*/


#ifndef __CURRENTLIMIT_H__
#define __CURRENTLIMIT_H__


class CurrentLimit
{
//variables
public:
  bool Enabled;
  U16 limit() { return _limit; }
protected:
private:
  tc0 tc;
  U16 _limit;

//functions
public:
	CurrentLimit();
	~CurrentLimit();

  void Enable() { SetLimit(_limit); Enabled = true; }
  void Disable() { SetAverageVoltage(3300); Enabled = false; }
  void SetLimit(U16 milliamps);
  
protected:
private:
	CurrentLimit( const CurrentLimit &c );
	CurrentLimit& operator=( const CurrentLimit &c );
  void SetAverageVoltage(U16 millivolts);

}; //CurrentLimit

#endif //__CURRENTLIMIT_H__
