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
  volatile U8* Port() {return port_address; }
  volatile U8* ddr;
  volatile U8* PIN;

protected:
private:
  volatile U8 *port_address;

//functions
public:
	port(volatile U8 *addr, volatile U8 *ddr_addr, volatile U8 *pin);
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
