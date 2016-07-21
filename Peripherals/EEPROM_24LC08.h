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

};

#endif //__EEPROM_24LC08_H__
