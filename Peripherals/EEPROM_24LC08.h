/* 
* EEPROM_24LC08.h
*
* Created: 2016/07/20 2:39:46 PM
* Author: Philip
*/


#ifndef __EEPROM_24LC08_H__
#define __EEPROM_24LC08_H__


class EEPROM_24LC08
{
//variables
public:
protected:
private:
  pin *write_protect;
  static const U8 address = 0xA0;
  
//functions
public:
	EEPROM_24LC08(pin *wp_pin);
	~EEPROM_24LC08();

protected:

private:
	EEPROM_24LC08( const EEPROM_24LC08 &c );
	EEPROM_24LC08& operator=( const EEPROM_24LC08 &c );

  void WriteProtectEnable(void) { write_protect->Clear(); }
  void WriteProtectDisable(void) { write_protect->Set(); }

  bool Write(U8 *data, U8 length) { return twi::WriteBytes(address, length, data); }
  bool Read(U8 length, U8 *data) { return twi::ReadBytes(address, length, data); }

};

#endif //__EEPROM_24LC08_H__
