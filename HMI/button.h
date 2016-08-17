/* 
* button.h
*
* Created: 2016/08/14 4:42:28 PM
* Author: philb
*/


#ifndef __BUTTON_H__
#define __BUTTON_H__

class button
{
//variables
public:
  bool Peak() { return _pressed; }
  bool Read() { bool retval = _pressed; _pressed = false; return retval; }
  void Clear() { _pressed = false; }
  
protected:
private:
  pin _pin;
  bool _pressed;

//functions
public:
	button(port* ptr_port, U8 pinNumber);
	~button();
  
  bool FSM(bool reset);

protected:
private:
	button( const button &c );
	button& operator=( const button &c );

}; //button

#endif //__BUTTON_H__
