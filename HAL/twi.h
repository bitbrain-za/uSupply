/* 
* twi.h
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/

#ifndef __TWI_H__
#define __TWI_H__

#if 1

typedef enum
{
  TWI_FLAG_START_DETECTED = USISIF,
  TWI_FLAG_COUNTER_OVERFLOW = USIOIF,
  TWI_FLAG_STOP_DETECTED = USIPF,
  TWI_FLAG_DATA_COLLISION = USIDC,
}TWI_FLAGS;

typedef enum
{
  TWI_RESULT_OKAY,
  TWI_RESULT_NO_DATA,
  TWI_RESULT_DATA_OUT_OF_BOUND,
  TWI_RESULT_UE_START_CON,
  TWI_RESULT_UE_STOP_CON,
  TWI_RESULT_UE_DATA_COL,
  TWI_RESULT_NO_ACK_ON_DATA,
  TWI_RESULT_NO_ACK_ON_ADDRESS,
  TWI_RESULT_MISSING_START_CON,
  TWI_RESULT_MISSING_STOP_CON,
}TWI_STATE;

class twi
{
//variables
public:
  static TWI_STATE ErrorState() { return _error_state; }
protected:
private:
  static bool addressMode;
  static bool masterWriteDataMode;
  static TWI_STATE _error_state;
  static pin SDA;
  static pin SCL;

//functions
public:
  static void inititalise(void);

  static unsigned char USI_TWI_Start_Transceiver_With_Data(U8 address, unsigned char * , unsigned char );

  static bool WriteBytes(U8 slave_address, U8 *bytes, U8 length);
  static bool ReadBytes(U8 slave_address, U8 *bytes, U8 length);

protected:
private:
  
  static unsigned char USI_TWI_Master_Transfer( bool );

  static bool SendStop(void);
  static void SendStart(void);  
  static bool SetAddress(U8 slave_address, bool ReadNotWrite);

  static void start_condition_interrupt_enable(void) { USICR |= (1 << USISIE); }
  static void start_condition_interrupt_disable(void) { USICR &= ~(1 << USISIE); }
  static void counter_overflow_interrupt_enable(void) { USICR |= (1 << USIOIE); }
  static void counter_overflow_interrupt_disable(void) { USICR &= ~(1 << USIOIE); }

  static bool CheckFlag(TWI_FLAGS flag) { return ((USISR >> flag) == 0x01); }
  static void ClearFlag(U8 flags) { USISR |= (flags & 0xF0); }
  static void ClearFlag(TWI_FLAGS flag) { USISR |= (1 << flag); }
  static bool CheckAndClearFlag(TWI_FLAGS flag);

  static void toggleClockPort() { USICR |= (1 << USITC); }
  static void one_bit_mode() { USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|(0xE<<USICNT0); }
  static void eight_bit_mode() { USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|(0x0<<USICNT0); }
  
  static void SendACK();
  static void SendNACK();
  static bool ACK_received(void);

  static void ReleaseSCL() { SCL.Set(); while(!SCL.Value()); }
  static void ReleasePin(pin *_pin) { _pin->Set(); }
  static void ReleaseAndWait(pin *_pin) { _pin->Set(); while(!_pin->Value()); }
  static void PullPin(pin *_pin) { _pin->Clear(); }

  static bool WriteByte(U8 b);
  static void ReadByte(U8 *b, bool EndOfTransmission);

}; //twi

#endif

#endif //__TWI_H__
