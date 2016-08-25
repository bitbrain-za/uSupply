/* 
* twi.cpp
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/
#include "../system.h"

#define DUMMY 0xFF
#define SIGNAL_VERIFY

#define TWI_SLOW_DELAY 40
#define TWI_FAST_DELAY 30

TWI_STATE twi::_error_state;
pin twi::SCL(&hal::portE, 4, true, true);
pin twi::SDA(&hal::portE, 5, true, true);

/*********************
* External Functions *
**********************/

void twi::inititalise(void)
{
  SDA.Set();
  SCL.Set();

  SCL.set_output();
  SDA.set_output();

  USIDR = 0xFF;
  USICR = (0<<USISIE)|(0<<USIOIE)|                            // Disable Interrupts.
          (1<<USIWM1)|(0<<USIWM0)|                            // Set USI in Two-wire mode.
          (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|                // Software storbe as counter clock source
          (0<<USITC);

  /* Clear Flags and reset Counter */
  USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|(0x0<<USICNT0);

}

bool twi::WriteBytes(U8 slave_address, U8* bytes, U8 length)
{
  _error_state = TWI_RESULT_OKAY;

  SendStart();

  if(!SetAddress(slave_address))
  {
    _error_state = TWI_RESULT_NO_ACK_ON_ADDRESS;
    return false;
  }

  /*Write address and Read/Write data */
  do
  {
    /* Write a byte */
    PullPin(&SCL);
    USIDR = (*(bytes++));
    USI_TWI_Master_Transfer( true );    // Send 8 bits on bus.

    /* Clock and verify (N)ACK from slave */
    if(!ACK_received())
    {
      _error_state = TWI_RESULT_NO_ACK_ON_DATA;
      return false;
    }
  }while(--length > 0);
  
  SendStop();

  return true;
}

bool twi::ReadBytes(U8 slave_address, U8 *bytes, U8 length)
{
  slave_address |= 0x01;
  _error_state = TWI_RESULT_OKAY;

  SendStart();
  
  if(!SetAddress(slave_address))
  {
    _error_state = TWI_RESULT_NO_ACK_ON_ADDRESS;
    return false;
  }
  
  do
  {
    SDA.set_input();
    *(bytes++)  = USI_TWI_Master_Transfer( true );
    //*(bytes++) = 0x22;
    
    if(length == 1)                            // If transmission of last byte was performed.
      SendNACK();
    else
      SendACK();
  }while(--length > 0);
  
  SendStop();

  return true;
}

U8 twi::USI_TWI_Start_Transceiver_With_Data(U8 address, U8 *msg, U8 msgSize, bool ReadNotWrite)
{
  if(ReadNotWrite)
    address |= 0x01;

  _error_state = TWI_RESULT_OKAY;

  SendStart();

  if(!SetAddress(address))
  {
    _error_state = TWI_RESULT_NO_ACK_ON_ADDRESS;
    return 0x01;
  }

  /*Write address and Read/Write data */
  do
  {
    if(ReadNotWrite)
    {
      ReadByte(msg++, msgSize == 1);
    }
    /* Else masterRead cycle*/
    else
    {
      if(!WriteByte(*(msg++)))
      {
        _error_state = TWI_RESULT_NO_ACK_ON_DATA;
        return 0x01;
      }
    }
  }while( --msgSize) ;                             // Until all data sent/received.
  
  SendStop();                           // Send a STOP condition on the TWI bus.

/* Transmission successfully completed*/
  return 0x00;
}

/********************
* Private Functions *
********************/

U8 twi::USI_TWI_Master_Transfer(bool eightBits)
{
  U8 rx;

  if(eightBits)
    eight_bit_mode();
  else
    one_bit_mode();
 
  do
  {
    _delay_us( TWI_SLOW_DELAY/4 );              
    toggleClockPort();
    while(!SCL.Value());
    _delay_us( TWI_FAST_DELAY/4 );              
    toggleClockPort();
  }while( !CheckFlag(TWI_FLAG_COUNTER_OVERFLOW) );        // Check for transfer complete.
  
  _delay_us( TWI_SLOW_DELAY/4 );                
  rx  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  SDA.set_output();

  return rx;                             // Return the data from the USIDR
}

bool twi::SendStop(void)
{
  PullPin(&SDA);
  ReleaseAndWait(&SCL);
  _delay_us( TWI_FAST_DELAY/4 );
  ReleasePin(&SDA);
  _delay_us( TWI_SLOW_DELAY/4 );
  
#ifdef SIGNAL_VERIFY
  if( !(USISR & (1<<USIPF)) )
  {
    _error_state = TWI_RESULT_MISSING_STOP_CON;
    return false;
  }
#endif

  return true;
}

void twi::SendStart(void)
{
  /* Release SCL to ensure that (repeated) Start can be performed */
  ReleaseAndWait(&SCL);
  _delay_us( TWI_FAST_DELAY/4 );                         // Delay for T4TWI if TWI_FAST_MODE

  /* Generate Start Condition */
  PullPin(&SDA);
  _delay_us( TWI_FAST_DELAY/4 );
  PullPin(&SCL);
  ReleasePin(&SDA);
}

bool twi::ACK_received(void)
{
  U8 rx;

  USIDR = DUMMY;

  SDA.set_input();
  one_bit_mode();
 
  do
  {
    _delay_us( TWI_SLOW_DELAY/4 );              
    toggleClockPort();
    while(!SCL.Value());
    _delay_us( TWI_FAST_DELAY/4 );              
    toggleClockPort();
  }while( !CheckFlag(TWI_FLAG_COUNTER_OVERFLOW) );        // Check for transfer complete.
  
  _delay_us( TWI_SLOW_DELAY/4 );                
  rx  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  SDA.set_output();

  if((rx & 0x01) != 0)
    return false;

  return true;
}

void twi::SendACK()
{
  USIDR = 0x00; 
  USI_TWI_Master_Transfer( false );   // Generate ACK/NACK.
}

void twi::SendNACK()
{
  USIDR = 0xFF; 
  USI_TWI_Master_Transfer( false );   // Generate ACK/NACK.
}

bool twi::SetAddress(U8 slave_address)
{
  /* Write a byte */
  PullPin(&SCL);

  USIDR = slave_address;
  USI_TWI_Master_Transfer(true);    // Send 8 bits on bus.
  
  /* Clock and verify (N)ACK from slave */
  if(!ACK_received()) 
    return false;
  
  return true;
}

bool twi::WriteByte(U8 b)
{
  /* Write a byte */
  PullPin(&SCL);
  USIDR = b;
  USI_TWI_Master_Transfer( true );    // Send 8 bits on bus.

  /* Clock and verify (N)ACK from slave */
  if(!ACK_received())
    return false;

  return true;
}

void twi::ReadByte(U8 *b, bool EndOfTransmission)
{
  /* Read a data byte */
  SDA.set_input();
  *(b)  = USI_TWI_Master_Transfer( true );

  /* Prepare to generate ACK (or NACK in case of End Of Transmission) */
  if(EndOfTransmission)                            // If transmission of last byte was performed.
  {
    SendNACK();
  }
  else
  {
    SendACK();
  }
}