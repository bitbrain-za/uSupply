/* 
* LCD_HW_interface.h
*
* Created: 2016/07/27 1:17:34 PM
* Author: Philip
*/

#ifndef __LCD_HW_INTERFACE_H__
#define __LCD_HW_INTERFACE_H__

class LCD_HW
{
//variables
public:
  static const U8 COLUMNS = 128;
  static const U8 ROWS = 64;
  static const U8 PAGES = 8;

protected:
private:

//functions
public:

  static void Reset() 
  {
	  PORTC &= ~(0x01 << 1); 
	  _delay_ms(100); 
    PORTC |= (0x01 << 1); 
	}

/*	
  static void ReadMode() 
  { 
    PORTG |= 0x03;
	  PORTG &= ~0x02;
	}

  static void WriteMode() 
  { 
    PORTG |= 0x03;
	  PORTG &= ~0x01;
  }
  
  static void IdleMode() 
  { 
    PORTG |= 0x03;
	}
  */
  static void SetData(U8 val);

  static void ChipDeselect() { PORTC |= (0x01 << 2); }
  static void ChipSelect() { PORTC &= ~(0x01 << 2); }

  static void CommandMode() { PORTC &= ~(0x01 << 0); }
  static void DataMode() { PORTC |= (0x01 << 0); }

  static void WRPinSet() { PORTG |= (0x01 << 1); }
  static void WRPinClear() { PORTG &= ~(0x01 << 1); }

  static void RDPinSet() { PORTG |= (0x01 << 0); }
  static void RDPinClear() { PORTG &= ~(0x01 << 0); }

  static void TriggerWrite()
  {
    LCD_HW::WRPinClear();
    _delay_us(1);
    LCD_HW::WRPinSet();
  }

protected:
private:

}; //LCD_HW_interface

#endif //__LCD_HW_INTERFACE_H__