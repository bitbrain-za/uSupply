/* 
* tc2.h
*
* Created: 2016/07/20 12:07:43 PM
* Author: Philip
*/


#ifndef __TC2_H__
#define __TC2_H__

class tc2
{
//variables
public:
  static U8 Count() { return TCNT2; }
  static U8 CompareVal() { return OCR2A; }

  static bool OverflowInterruptActive() {return ((TIFR2 & 0x02) == 0x02); }
  static bool OutputCompareInterruptActive() {return ((TIFR2 & 0x01) == 0x01); }
protected:
private:
  static bool ocrCallbackSet;
  static void (*ocrCallBack)(void);


//functions
public:
  static void init(TC_WGM waveform_mode, TC_COM compare_mode, TC_CS clock_select);

  static void SelectClock(TC_CS clock_select);
  static void SetWGM(TC_WGM mode);
  static void SetCOM(TC_COM mode);

  static void SetCount(U8 count) { TCNT2 = count; }
  static void SetOutputCompareA(U8 val) { OCR2A = val; }

  static void ExternalClockMode(bool enable, bool xtal);

  static void EnableOutputCompareInterrupt() { TIMSK2 |= 0x02; }
  static void DisableOutputCompareInterrupt() { TIMSK2 &= ~0x02; }
  static void ClearOutputCompareInterrupt() { TIFR2 &= ~0x02; }

  static void EnableOverflowInterrupt() { TIMSK2 |= 0x01; }
  static void DisableOverflowInterrupt() { TIMSK2 &= ~0x01; }
  static void ClearOverflowInterrupt() { TIFR2 &= ~0x01; }

  static void SetOutputCompareCallback(void (*fp)(void)) { ocrCallbackSet = true; ocrCallBack = fp; }
  static void ClearOutputCompareCallback(void) { ocrCallbackSet = false; }

  static void TriggerCompareMatchCallback(void)
  {
    if(ocrCallbackSet)
    {
      ocrCallBack();
    }
  }

protected:
private:

}; //tc2

#endif //__TC2_H__
