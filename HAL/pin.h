/* 
* pin.h
*
* Created: 2016/07/20 3:00:38 PM
* Author: Philip
*/


#ifndef __PIN_H__
#define __PIN_H__

class pin
{
//variables
public:
protected:
private:
  port *_port;
  U8 _pin;
  bool state;
  bool _output;

//functions
public:
	pin(port *parent, U8 pin_number, bool initial_state, bool Output)
  {
    _port = parent;
    _pin = pin_number;

    if(Output)
    {
      set_output();
      if(initial_state)
        Set();
      else
        Clear();
    }
    else
    {
      set_input();
    }
  };
	~pin() {;}

  void set_input()  { (_port->ddr &= ~(1<<_pin)); _output = false; }
  void set_output() { (_port->ddr |= (1<<_pin)); _output = true; }

  void Set() { if(!_output) return; _port->SetPin(_pin); state = true; }
  void Clear() { if(!_output) return; _port->ClearPin(_pin); state = false; }
  void Toggle() 
  {
    if(!_output)
      return;

    if(state) 
      Clear(); 
    else 
      Set(); 
  }
  bool Value() { return read_pin(); }

protected:
private:
	pin( const pin &c );
	pin& operator=( const pin &c );

  bool read_pin() { return (_port->PIN & (1<<_pin)); }
};

#endif //__PIN_H__
