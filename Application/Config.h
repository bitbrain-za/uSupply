/* 
* Config.h
*
* Created: 2016/09/03 3:54:30 PM
* Author: philb
*/


#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ADDR_PATTERN 0x0000
#define ADDR_VOLTAGE ADDR_PATTERN + 2
#define ADDR_CURRENT ADDR_VOLTAGE + 2

class Config
{
//variables
public:
  U16 SavedVoltage() { return saved_voltage; }
  U16 SavedCurrent() { return saved_current; }
  U16 ram_voltage;
  U16 ram_current;
protected:
private:
  EEPROM_24LC08 eeprom;
  timer config_timer;
  U16 saved_voltage;
  U16 saved_current;
  
//functions
public:
	Config();
	~Config();

  void Load();
  void Save();
  void Format();
protected:
private:
	Config( const Config &c );
	Config& operator=( const Config &c );

}; //Config

#endif //__CONFIG_H__
