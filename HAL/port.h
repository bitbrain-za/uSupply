/* 
* port.h
*
* Created: 2016/07/20 11:14:52 AM
* Author: Philip
*/


#ifndef __PORT_H__
#define __PORT_H__


class port
{
//variables
public:
  U8 Port() {return port_address; }
  U8 ddr;
  U8 PIN;

protected:
private:
  U8 port_address;

//functions
public:
	port(U8 addr, U8 ddr_addr, U8 pin);
	~port();

  void SetInputs(U8 pins);
  void SetOutputs(U8 pins);

  void SetPin(U8 pin);
  void ClearPin(U8 pin);
protected:
private:
	port( const port &c );
	port& operator=( const port &c );

}; //port

#endif //__PORT_H__
