/* 
* tc2.h
*
* Created: 2016/07/20 12:07:43 PM
* Author: Philip
*/


#ifndef __TC2_H__
#define __TC2_H__

class tc2 : Timer8Bit
{
//variables
public:
  U8 Count() { return TCNT2; }
  U8 CompareVal() { return OCR2A; }

  bool OverflowInterruptActive() {return ((TIFR2 & 0x02) == 0x02); }
  bool OutputCompareInterruptActive() {return ((TIFR2 & 0x01) == 0x01); }
protected:
private:

//functions
public:
	tc2(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select);
	~tc2();

  void SelectClock(TC_CS clock_select);
  void SetWGM(TC_WGM mode);
  void SetCOM(TC_COM mode);

  void SetCount(U8 count) { TCNT2 = count; }
  void SetOutputCompareA(U8 val) { OCR2A = val; }

  void ExternalClockMode(bool enable, bool xtal);

  void EnableOutputCompareInterrupt() { TIMSK2 |= 0x02; }
  void DisableOutputCompareInterrupt() { TIMSK2 &= ~0x02; }
  void ClearOutputCompareInterrupt() { TIFR2 &= ~0x02; }

  void EnableOverflowInterrupt() { TIMSK2 |= 0x01; }
  void DisableOverflowInterrupt() { TIMSK2 &= ~0x01; }
  void ClearOverflowInterrupt() { TIFR2 &= ~0x01; }

protected:
private:
	tc2( const tc2 &c );
	tc2& operator=( const tc2 &c );

}; //tc2

#endif //__TC2_H__
