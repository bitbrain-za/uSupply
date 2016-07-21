/* 
* twi.h
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/


#ifndef __TWI_H__
#define __TWI_H__

typedef enum
{
  TWI_FLAG_START_DETECTED = USISIF,
  TWI_FLAG_COUNTER_OVERFLOW = USIOIF,
  TWI_FLAG_STOP_DETECTED = USIPF,
  TWI_FLAG_DATA_COLLISION = USIDC,
}TWI_FLAGS;

typedef enum
{
  USI_TWI_OKAY,
  USI_TWI_NO_DATA,
  USI_TWI_DATA_OUT_OF_BOUND,
  USI_TWI_UE_START_CON,
  USI_TWI_UE_STOP_CON,
  USI_TWI_UE_DATA_COL,
  USI_TWI_NO_ACK_ON_DATA,
  USI_TWI_NO_ACK_ON_ADDRESS,
  USI_TWI_MISSING_START_CON,
  USI_TWI_MISSING_STOP_CON,
}TWI_STATE;

class twi
{
//variables
public:
  static TWI_STATE ErrorState() { return _error_state; }
protected:
private:
  static TWI_STATE _error_state;
  static bool _address_mode;
  static bool _master_write_data_mode;
  static pin SDA;
  static pin SCL;

//functions
public:
  static void inititalise(void);
  static U8 Transceive(U8 slave, U8 *message, U8 length);
  static U8 Transceive(U8 *message, U8 length);
  static U8 GetState(void);

protected:
private:
  static U8 MasterTransfer(U8 temp);
  static bool SendStop(void);
  static bool SendStart(void);  

  static void start_condition_interrupt_enable(void) { USICR |= (1 << USISIE); }
  static void start_condition_interrupt_disable(void) { USICR &= ~(1 << USISIE); }
  static void counter_overflow_interrupt_enable(void) { USICR |= (1 << USIOIE); }
  static void counter_overflow_interrupt_disable(void) { USICR &= ~(1 << USIOIE); }

  static bool CheckFlag(void) { return ((USISR & 0xF0) != 0x00); }
  static bool CheckFlag(TWI_FLAGS flag) { return ((USISR & flag) == flag); }
  static void ClearFlag(U8 flags) { USISR |= (flags & 0xF0); }
  static void ClearFlag(TWI_FLAGS flag) { USISR |= (1 << flag); }
  static bool CheckAndClearFlag(TWI_FLAGS flag);
}; //twi

#endif //__TWI_H__
