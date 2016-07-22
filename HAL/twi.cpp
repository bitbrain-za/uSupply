/* 
* twi.cpp
*
* Created: 2016/07/20 10:41:52 AM
* Author: Philip
*/

#include "../system.h"

/*
    ____       _____
   / __ \___  / __(_)___  ___  _____
  / / / / _ \/ /_/ / __ \/ _ \/ ___/
 / /_/ /  __/ __/ / / / /  __(__  )
/_____/\___/_/ /_/_/ /_/\___/____/
*/

#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.
#define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.

/*
    __  ___
   /  |/  /___ _______________  _____
  / /|_/ / __ `/ ___/ ___/ __ \/ ___/
 / /  / / /_/ / /__/ /  / /_/ (__  )
/_/  /_/\__,_/\___/_/   \____/____/
*/
#define T2_TWI (((SYS_CLOCK / 1000) * 4700) / 1000000) + 1 // >4,7us
#define T4_TWI (((SYS_CLOCK / 1000) * 4000) / 1000000) + 1 // >4,0us

/*
    ____            __                 __  _
   / __ \___  _____/ /___ __________ _/ /_(_)___  ____  _____
  / / / / _ \/ ___/ / __ `/ ___/ __ `/ __/ / __ \/ __ \/ ___/
 / /_/ /  __/ /__/ / /_/ / /  / /_/ / /_/ / /_/ / / / (__  )
/_____/\___/\___/_/\__,_/_/   \__,_/\__/_/\____/_/ /_/____/
*/
TWI_STATE twi::_error_state;
bool twi::_address_mode;
bool twi::_master_write_data_mode;
pin twi::SCL(&hal::portE, PE4, true, true);
pin twi::SDA(&hal::portE, PE5, true, true);

/*
    ____        __    ___         ______                 __  _
   / __ \__  __/ /_  / (_)____   / ____/_  ______  _____/ /_(_)___  ____  _____
  / /_/ / / / / __ \/ / / ___/  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / ____/ /_/ / /_/ / / / /__   / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_/    \__,_/_.___/_/_/\___/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
*/
void twi::inititalise(void)
{
  SCL.Set();
  SDA.Set();
  SCL.set_output();
  SDA.set_output();

  USIDR = 0xFF;

  USICR = 0x00 |
          /* TWI Mode */
          (1 << USIWM1) | (0 << USIWM0) |
          /* Software strobe as clock source */
          (1<<USICS1) | (0 << USICS0) | (1 << USICLK) |
          (0<<USITC);

  ClearFlag(0xF0);
}

/*---------------------------------------------------------------
 USI Transmit and receive function. LSB of first byte in data 
 indicates if a read or write cycles is performed. If set a read
 operation is performed.

 Function generates (Repeated) Start Condition, sends address and
 R/W, Reads/Writes Data, and verifies/sends ACK.
 
 Success or error code is returned. Error codes are defined in 
 USI_TWI_Master.h
---------------------------------------------------------------*/
U8 twi::Transceive(U8 *message, U8 length)
{
  unsigned char tempUSISR_8bit = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Prepare register value to: Clear flags, and
                                 (0x0<<USICNT0);                                     // set USI to shift 8 bits i.e. count 16 clock edges.
  unsigned char tempUSISR_1bit = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Prepare register value to: Clear flags, and
                                 (0xE<<USICNT0);                                     // set USI to shift 1 bit i.e. count 2 clock edges.

  _error_state = USI_TWI_OKAY;
  _address_mode = true;

  if(message > (unsigned char*)RAMEND)                  // Test if address is outside SRAM space
  {
    _error_state = USI_TWI_DATA_OUT_OF_BOUND;
    return false;
  }
  
  if(length <= 1)                                       // Test if the transmission buffer is empty
  {
    _error_state = USI_TWI_NO_DATA;
    return false;
  }

  if(CheckFlag(TWI_FLAG_START_DETECTED))
  {
    _error_state = USI_TWI_UE_START_CON;
    return false;
  }
  
  if(CheckFlag(TWI_FLAG_STOP_DETECTED))
  {
    _error_state = USI_TWI_UE_STOP_CON;
    return false;
  }

  if(CheckFlag(TWI_FLAG_DATA_COLLISION))
  {
    _error_state = USI_TWI_UE_DATA_COL;
    return false;
  }

  if ( !(*message & (1<<TWI_READ_BIT)) )        // The LSB in the address byte determines if is a masterRead or masterWrite operation.
  {
    _master_write_data_mode = true;
  }

  /* Release SCL to ensure that (repeated) Start can be performed */
  SCL.Set();
  while(!SCL.Value());
  delay::delay_us( T2_TWI/4 );

  if(!SendStart())
  {
    return false;
  }

  /*Write address and Read/Write data */
  do
  {
    /* If masterWrite cycle (or initial address transmission)*/
    if (_address_mode || _master_write_data_mode)
    {
      /* Write a byte */
      SCL.Clear();
      USIDR     = *(message++);                   // Setup data.
      MasterTransfer(tempUSISR_8bit);             // Send 8 bits on bus.
      
      /* Clock and verify (N)ACK from slave */
      SDA.set_input();
      if(MasterTransfer(tempUSISR_1bit) & (1<<TWI_NACK_BIT)) 
      {
        if (_address_mode)
          _error_state = USI_TWI_NO_ACK_ON_ADDRESS;
        else
          _error_state = USI_TWI_NO_ACK_ON_DATA;
        return false;
      }
      _address_mode = false;                      // Only perform address transmission once.
    }
    /* Else masterRead cycle*/
    else
    {
      /* Read a data byte */
      SDA.set_input();
      *(message++) = MasterTransfer(tempUSISR_8bit);

      /* Prepare to generate ACK (or NACK in case of End Of Transmission) */
      if( length == 1)                            // If transmission of last byte was performed.
      {
        USIDR = 0xFF;                             // Load NACK to confirm End Of Transmission.
      }
      else
      {
        USIDR = 0x00;                             // Load ACK. Set data register bit 7 (output for SDA) low.
      }
      MasterTransfer( tempUSISR_1bit );           // Generate ACK/NACK.
    }
  }while( --length) ;                             // Until all data sent/received.
  
  SendStop();                                     // Send a STOP condition on the TWI bus.

  /* Transmission successfully completed */
  return true;
}

/*
    ____       _             __          ______                 __  _
   / __ \_____(_)   ______ _/ /____     / ____/_  ______  _____/ /_(_)___  ____  _____
  / /_/ / ___/ / | / / __ `/ __/ _ \   / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / ____/ /  / /| |/ / /_/ / /_/  __/  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_/   /_/  /_/ |___/\__,_/\__/\___/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
*/


/*---------------------------------------------------------------
 Core function for shifting data in and out from the USI.
 Data to be sent has to be placed into the USIDR prior to calling
 this function. Data read, will be returned from the function.
---------------------------------------------------------------*/
U8 twi::MasterTransfer(U8 temp)
{
  USISR = temp;                                     // Set USISR according to temp.
                                                    // Prepare clocking.
  temp  =  (0<<USISIE)|(0<<USIOIE)|                 // Interrupts disabled
           (1<<USIWM1)|(0<<USIWM0)|                 // Set USI in Two-wire mode.
           (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|     // Software clock strobe as source.
           (1<<USITC);                              // Toggle Clock Port.

  do
  {
    delay::delay_us(T2_TWI/4);              
    USICR = temp;                           // Generate positive SCL edge.
    while(!SCL.Value());                  // Wait for SCL to go high.
    delay::delay_us(T4_TWI/4);              
    USICR = temp;                           // Generate negative SCL edge.
  }while(!CheckFlag(TWI_FLAG_COUNTER_OVERFLOW));
  
  delay::delay_us(T2_TWI/4);                
  temp  = USIDR;                            // Read out data.
  USIDR = 0xFF;                             // Release SDA.
  SDA.set_output();

  return temp;                              // Return the data from the USIDR
}

/*---------------------------------------------------------------
 Function for generating a TWI Stop Condition. Used to release 
 the TWI bus.
---------------------------------------------------------------*/
bool twi::SendStop(void)
{
  SDA.Clear();
  SCL.Set();
  while(!SCL.Value());  // Wait for SCL to go high.
  delay::delay_us( T4_TWI/4 );               
  SDA.Set();
  delay::delay_us( T2_TWI/4 );                
  
  if(!CheckFlag(TWI_FLAG_STOP_DETECTED))
  {
    _error_state = USI_TWI_MISSING_STOP_CON;    
    return false;
  }

  return true;
}

/* Generate Start Condition */
bool twi::SendStart(void)
{
  SDA.Clear();
  delay::delay_us( T4_TWI/4 );                         
  SCL.Clear();
  SDA.Set();

  if(!CheckFlag(TWI_FLAG_START_DETECTED))
  {
    _error_state = USI_TWI_MISSING_START_CON;  
    return false;
  }
  return true;
}

bool twi::CheckAndClearFlag(TWI_FLAGS flag)
{
  if(CheckFlag(flag))
  {
    ClearFlag(flag);
    return true;
  }
  return false;
}

