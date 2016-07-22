/*
 * system.h
 *
 * Created: 2016/07/20 11:08:39 AM
 *  Author: Philip
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

/*
TIMER0 - Current Set
TIMER1 - FAN Control
TIMER2 - System Timer
*/

#define SYS_CLOCK 8000000

#include <avr/io.h>

#include "typedef.h"

#include "HAL/Timer8Bit.h"

#include "HAL/port.h"
#include "HAL/tc0.h"
#include "HAL/tc2.h"
#include "HAL/pin.h"
#include "HAL/twi.h"
#include "HAL/adc.h"
#include "HAL/hal.h"

#include "Timers/delay.h"
#include "Peripherals/EEPROM_24LC08.h"

#endif /* SYSTEM_H_ */