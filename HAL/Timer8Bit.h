/* 
* Timer8Bit.h
*
* Created: 2016/07/20 1:59:46 PM
* Author: Philip
*/


#ifndef __TIMER8BIT_H__
#define __TIMER8BIT_H__

typedef enum
{
  TC_WGM_NORMAL = 0x00,
  TC_WGM_PWM_PC = 0x01,
  TC_WGM_CTC = 0x02,
  TC_WGM_PWM_FAST = 0x03,
}TC_WGM;

typedef enum
{
  TC_COM_OC2A_DISCONNECTED = 0x00,
  TC_COM_OC2A_TOGGLE =  0x01,
  TC_COM_OC2A_CLEAR = 0x02,
  TC_COM_OC2A_SET = 0x03,
}TC_COM;

typedef enum
{
  TC_CS_NO_CLOCK = 0x00,
  TC_CS_NO_PRESCALER = 0x01,
  TC_CS_DIV8 = 0x02,
  TC_CS_DIV32 = 0x03,
  TC_CS_DIV64 = 0x04,
  TC_CS_DIV128 = 0x05,
  TC_CS_DIV256 = 0x06,
  TC_CS_DIV1024 = 0x07,
}TC_CS;

class Timer8Bit
{
//functions
public:
	virtual ~Timer8Bit(){}

  virtual U8 Count() = 0;
  virtual U8 CompareVal() = 0;

  virtual bool OverflowInterruptActive() = 0;
  virtual bool OutputCompareInterruptActive() = 0;

  virtual void SelectClock(TC_CS clock_select) = 0;
  virtual void SetWGM(TC_WGM mode) = 0;
  virtual void SetCOM(TC_COM mode) = 0;

  virtual void SetCount(U8 count) = 0;
  virtual void SetOutputCompareA(U8 val) = 0;

  virtual void ExternalClockMode(bool enable, bool xtal) = 0;

  virtual void EnableOutputCompareInterrupt() = 0;
  virtual void DisableOutputCompareInterrupt() = 0;
  virtual void ClearOutputCompareInterrupt() = 0;

  virtual void EnableOverflowInterrupt() = 0;
  virtual void DisableOverflowInterrupt() = 0;
  virtual void ClearOverflowInterrupt() = 0;

}; //Timer8Bit

#endif //__TIMER8BIT_H__
