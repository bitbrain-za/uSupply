/* 
* EEPROM_24LC08.cpp
*
* Created: 2016/07/20 2:39:46 PM
* Author: Philip
*/

#include "../system.h"

// default constructor
EEPROM_24LC08::EEPROM_24LC08(pin *wp_pin)
{
  write_protect = wp_pin;
} 

// default destructor
EEPROM_24LC08::~EEPROM_24LC08()
{
} //~EEPROM_24LC08
