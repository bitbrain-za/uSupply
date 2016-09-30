/* 
* tc0.h
*
* Created: 2016/07/20 12:06:22 PM
* Author: Philip
*/


#ifndef __TC0_H__
#define __TC0_H__


class tc0
{
//variables
public:
  static U8 Count() { return TCNT0; }
  static U8 CompareVal() { return OCR0A; }

  static bool OverflowInterruptActive() {return ((TIFR0 & 0x02) == 0x02); }
  static bool OutputCompareInterruptActive() {return ((TIFR0 & 0x01) == 0x01); }

protected:
private:

//functions
public:
  static void init(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select);
  static void SelectClock(TC_CS clock_select);
  static void SetWGM(TC_WGM mode);
  static void SetCOM(TC_COM mode);

  static void SetCount(U8 count) { TCNT0 = count; }
  static void SetOutputCompareA(U8 val) { OCR0A = val; }

  static void SetDutyCycle(U8 duty);

  static void ExternalClockMode(bool enable, bool xtal);

  static void EnableOutputCompareInterrupt() { TIMSK0 |= 0x02; }
  static void DisableOutputCompareInterrupt() { TIMSK0 &= ~0x02; }
  static void ClearOutputCompareInterrupt() { TIFR0 &= ~0x02; }

  static void EnableOverflowInterrupt() { TIMSK0 |= 0x01; }
  static void DisableOverflowInterrupt() { TIMSK0 &= ~0x01; }
  static void ClearOverflowInterrupt() { TIFR0 &= ~0x01; }

protected:
private:

}; //tc0

#endif //__TC0_H__
