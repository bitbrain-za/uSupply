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
  static bool Enabled;
  static U16 limit() { return _limit; }
  static bool Dirty() { bool temp = _dirty; _dirty = false; return temp; }
protected:
private:
  static RotaryEncoder *_encoder;
  static U16 _limit;
  static bool _dirty;
  static U16 StepSize;
  static U16 max;
  static U16 min;
  static S8 calibration_value;

//functions
public:
  static void init(RotaryEncoder *encoder);
  static void FSM(FSM_CONTROL control);
  static void Enable() { SetLimit(_limit); Enabled = true; }
  static void Disable() { SetAverageVoltage(3300); Enabled = false; }
  static void SetLimit(U16 milliamps);
  
protected:
private:
  static void SetAverageVoltage(U16 millivolts);

}; //CurrentLimit

#endif //__CURRENTLIMIT_H__
