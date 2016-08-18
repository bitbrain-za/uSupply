/* 
* twi.cpp
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/
#if 1
#include "../system.h"

#define DUMMY 0xFF

TWI_STATE twi::_error_state;
pin twi::SCL(&hal::portE, 4, true, true);
pin twi::SDA(&hal::portE, 5, true, true);
bool twi::addressMode = false;
bool twi::masterWriteDataMode = false;

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
  if(!twi::USI_TWI_Start_Transceiver_With_Data(slave_address, bytes, length ) > 0)
  {
    return false;
  }

  return true;
}

bool twi::ReadBytes(U8 slave_address, U8 *bytes, U8 length)
{
  slave_address |= 0x01;

  if(!twi::USI_TWI_Start_Transceiver_With_Data( slave_address, bytes, length ) > 0)
  {
    return false;
  }
  return true;
}

unsigned char twi::USI_TWI_Start_Transceiver_With_Data(U8 address, unsigned char *msg, unsigned char msgSize)
{
  _error_state = TWI_RESULT_OKAY;
  addressMode = TRUE;

  if ( !(address & (1<<TWI_READ_BIT)) )                // The LSB in the address byte determines if is a masterRead or masterWrite operation.
  {
    masterWriteDataMode = TRUE;
  }

  SendStart();

/*Write address and Read/Write data */
  do
  {
    if(addressMode)
    {
      if(!SetAddress(address, masterWriteDataMode))
      {
        _error_state = TWI_RESULT_NO_ACK_ON_ADDRESS;
        return 0x01;
      }
      addressMode = false;
    }
    else if (masterWriteDataMode)
    {
      if(!WriteByte(*(msg++)))
      {
        _error_state = TWI_RESULT_NO_ACK_ON_DATA;
        return 0x01;
      }
    }
    /* Else masterRead cycle*/
    else
    {
      ReadByte(msg++, msgSize == 1);
    }
  }while( --msgSize) ;                             // Until all data sent/received.
  
  SendStop();                           // Send a STOP condition on the TWI bus.

/* Transmission successfully completed*/
  return 0x00;
}

/********************
* Private Functions *
********************/

unsigned char twi::USI_TWI_Master_Transfer(bool eightBits)
{
  U8 rx;

  if(eightBits)
  {
    eight_bit_mode();
  }
  else
    one_bit_mode();
 
  do
  {
    _delay_us( T2_TWI/4 );              
    toggleClockPort();
    while(!SCL.Value());
    _delay_us( T4_TWI/4 );              
    toggleClockPort();
  }while( !CheckFlag(TWI_FLAG_COUNTER_OVERFLOW) );        // Check for transfer complete.
  
  _delay_us( T2_TWI/4 );                
  rx  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  SDA.set_output();

  return rx;                             // Return the data from the USIDR
}

bool twi::SendStop(void)
{
  PullPin(&SDA);
  ReleaseAndWait(&SCL);
  _delay_us( T4_TWI/4 );
  ReleasePin(&SDA);
  _delay_us( T2_TWI/4 );
  
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
  _delay_us( T4_TWI/4 );                         // Delay for T4TWI if TWI_FAST_MODE

  /* Generate Start Condition */
  PullPin(&SDA);
  _delay_us( T4_TWI/4 );
  PullPin(&SCL);
  ReleasePin(&SDA);
}

bool twi::ACK_received(void)
{
  SDA.set_input();
  if( USI_TWI_Master_Transfer( false ) & (1<<TWI_NACK_BIT) ) 
  {
    if ( addressMode )
      _error_state = TWI_RESULT_NO_ACK_ON_ADDRESS;
    else
      _error_state = TWI_RESULT_NO_ACK_ON_DATA;
    return false;
  }
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

bool twi::SetAddress(U8 slave_address, bool ReadNotWrite)
{
  /* Write a byte */
  PullPin(&SCL);
  if(ReadNotWrite)
    slave_address |= 1;

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

#endif