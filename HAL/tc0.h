/* 
* tc0.h
*
* Created: 2016/07/20 12:06:22 PM
* Author: Philip
*/


#ifndef __TC0_H__
#define __TC0_H__


class tc0 : Timer8Bit
{
//variables
public:
  U8 Count() { return TCNT0; }
  U8 CompareVal() { return OCR0A; }

  bool OverflowInterruptActive() {return ((TIFR0 & 0x02) == 0x02); }
  bool OutputCompareInterruptActive() {return ((TIFR0 & 0x01) == 0x01); }

protected:
private:

//functions
public:
  tc0(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select);
	~tc0();

  void SelectClock(TC_CS clock_select);
  void SetWGM(TC_WGM mode);
  void SetCOM(TC_COM mode);

  void SetCount(U8 count) { TCNT0 = count; }
  void SetOutputCompareA(U8 val) { OCR0A = val; }

  void ExternalClockMode(bool enable, bool xtal);

  void EnableOutputCompareInterrupt() { TIMSK0 |= 0x02; }
  void DisableOutputCompareInterrupt() { TIMSK0 &= ~0x02; }
  void ClearOutputCompareInterrupt() { TIFR0 &= ~0x02; }

  void EnableOverflowInterrupt() { TIMSK0 |= 0x01; }
  void DisableOverflowInterrupt() { TIMSK0 &= ~0x01; }
  void ClearOverflowInterrupt() { TIFR0 &= ~0x01; }

protected:
private:
	tc0( const tc0 &c );
	tc0& operator=( const tc0 &c );

}; //tc0

#endif //__TC0_H__
