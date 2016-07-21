/* 
* PWM.h
*
* Created: 2016/07/20 1:17:59 PM
* Author: Philip
*/


#ifndef __PWM_H__
#define __PWM_H__


class PWM
{
//variables
public:
protected:
private:

//functions
public:
	PWM();
	~PWM();

  void SetMode(bool PhaseCorrect);
  void SetDutyCycle(U8 percent);
  void SetFrequency(U16 hz);

  void Enable();
  void Disable();
protected:
private:
	PWM( const PWM &c );
	PWM& operator=( const PWM &c );

}; //PWM

#endif //__PWM_H__
