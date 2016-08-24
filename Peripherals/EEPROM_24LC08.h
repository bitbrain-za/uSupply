/* 
* EEPROM_24LC08.h
*
* Created: 2016/07/20 2:39:46 PM
* Author: Philip
*/


#ifndef __EEPROM_24LC08_H__
#define __EEPROM_24LC08_H__

typedef enum
{
  EEPROM_OKAY,
  EEPROM_BOUNDARY_ERROR,
  EEPROM_ERROR_ADDRESS,
  EEPROM_ERROR_READING,
  EEPROM_ERROR_WRITING,
  EEPROM_ERROR_PARAMATER,
  EEPROM_ERROR_MEMORY,
  EEPROM_ERROR_DATA_MISMATCH
}EEPROM_RESULT;

class EEPROM_24LC08
{
//variables
public:
protected:
private:
  pin write_protect;
  static const U8 DeviceAddress = 0xA0;
  static const U16 MemorySize = 1024;
  
//functions
public:
	EEPROM_24LC08(port* wp_port, U8 WPpinNumber);
	~EEPROM_24LC08();

  EEPROM_RESULT Write(U16 address, U8 *data, U8 length);
  EEPROM_RESULT Read(U16 address, U8 length, U8 *data);

  EEPROM_RESULT WriteAndVerify(U16 address, U8 length, U8 *data);

  bool Test();

protected:

private:
	EEPROM_24LC08( const EEPROM_24LC08 &c );
	EEPROM_24LC08& operator=( const EEPROM_24LC08 &c );

  void WriteProtectEnable(void) { write_protect.Set(); }
  void WriteProtectDisable(void) { write_protect.Clear(); }

  EEPROM_RESULT SetAddress(U16 address);

};

#endif //__EEPROM_24LC08_H__
