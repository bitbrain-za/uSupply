/* 
* RotaryEncoder.h
*
* Created: 2016/08/14 12:05:25 AM
* Author: philb
*/


#ifndef __ROTARYENCODER_H__
#define __ROTARYENCODER_H__


class RotaryEncoder
{
//variables
public:
  S8 Value() { _changed = false; return _value; }
  bool Changed() { return _changed; }
  void Reset() { _value = 0; }
protected:
private:
  pin pinA;
  pin pinB;
  S8 _value;
  bool _changed;

//functions
public:
	RotaryEncoder(port* portA, U8 pinNumberA, port* portB, U8 pinNumberB);
	~RotaryEncoder();

  void FSM(bool reset);

protected:
private:
	RotaryEncoder( const RotaryEncoder &c );
	RotaryEncoder& operator=( const RotaryEncoder &c );


}; //RotaryEncoder

#endif //__ROTARYENCODER_H__
