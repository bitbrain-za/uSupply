/* 
* qms.cpp
*
* Created: 2016/07/27 2:17:29 PM
* Author: Philip
*/


#include "../system.h"

#define MEM_CHECK() if(status == QUEUE_NO_MEMORY) return
#define B_MEM_CHECK() if(status == QUEUE_NO_MEMORY) return false

/*
    ____        __    ___
   / __ \__  __/ /_  / (_)____
  / /_/ / / / / __ \/ / / ___/
 / ____/ /_/ / /_/ / / / /__
/_/    \__,_/_.___/_/_/\___/

*/

/**
 * @brief MessageQueue
 *  Constructor for the MessageQueue
 *
 */
MessageQueue::MessageQueue(U8 QueueDepth, U8 DataSize)
{
  Length = QueueDepth;
  CommandPool = (U8 *)malloc(Length * 1);
  LengthPool = (U8 *)malloc(Length * 1);
  DataPool = (U8 *)malloc(DataSize * Length * 1);

  if((CommandPool == NULL) || (DataPool == NULL) || (LengthPool == NULL))
    status = QUEUE_NO_MEMORY;
  else
    status = QUEUE_OKAY;

  MessageSize = DataSize;

  NumberUnread = 0;
}

/**
 * @brief MessageQueue
 *  Constructor for the MessageQueue
 *
 */
MessageQueue::MessageQueue(U8 QueueDepth)
{
  Length = QueueDepth;
  CommandPool = (U8 *)malloc(Length * 1);

  if(CommandPool == NULL)
    status = QUEUE_NO_MEMORY;
  else
    status = QUEUE_OKAY;

  MessageSize = 0;
  NumberUnread = 0;
}

/**
 * @brief MessageQueue
 *  Destructor for the MessageQueue
 *
 */
MessageQueue::~MessageQueue(void)
{
  MEM_CHECK();

  free(CommandPool);

  if(MessageSize > 0)
  {
    free(DataPool);
    free(LengthPool);
  }
}

/**
 * @brief MessageQueue::Send
 *  Places a message at the end of the queue
 *  
 * @param[in] command byte
 * @param[in] length of the data
 * @param[in] byte array - The data being sent
 * @return true if there was space in the queue
 *
 */
bool MessageQueue::Send(U8 cmd, U8 length, U8 *data)
{
  B_MEM_CHECK();

  if(IsFull())
  {
    return false;
  }

  CommandPool[NumberUnread] = cmd;
  
  if(data != NULL)
  {
    memcpy(&DataPool[MessageSize * NumberUnread], data, MessageSize);
    LengthPool[NumberUnread] = length;
  }

  NumberUnread++;

  return true;
}

/**
 * @brief MessageQueue::Peak
 *  Reads the oldest message in the queue and leaves it there
 *  
 * @param[out] byte place to store the command byte
 * @param[out] byte place to store the length byte
 * @param[out] byte place to store the data
 * @return true if there was a message in the queue
 *
 */
bool MessageQueue::Peak(U8 *cmd, U8 *length, U8 *data)
{
  B_MEM_CHECK();

  if(IsEmpty())
    return false;

  *cmd = CommandPool[0];
  
  if(data != NULL)
  {
    memcpy(data, DataPool, MessageSize);
    *length = LengthPool[0];
  }

  return true;
}

/**
 * @brief MessageQueue::Read
 *  Reads the oldest message in the queue and deletes the message
 *  
 * @param[out] byte place to store the command byte
 * @param[out] byte place to store the length byte
 * @param[out] byte place to store the data
 * @return true if there was a message in the queue
 *
 */
bool MessageQueue::Read(U8 *cmd, U8 *length, U8 *data)
{
  B_MEM_CHECK();

  if(Peak(cmd, length, data))
  {
    RemoveFirstAndShift();
    return true;
  }
  return false;
}

/**
 * @brief MessageQueue::Clear
 *  Clears the queue
 */
void MessageQueue::Clear(void)
{
  NumberUnread = 0;
}

/*
    ____       _             __
   / __ \_____(_)   ______ _/ /____
  / /_/ / ___/ / | / / __ `/ __/ _ \
 / ____/ /  / /| |/ / /_/ / /_/  __/
/_/   /_/  /_/ |___/\__,_/\__/\___/

*/

/**
 * @brief MessageQueue::RemoveFirstAndShift
 *  Updates the queue RAM pool and variables when a message is removed 
 *
 */
void MessageQueue::RemoveFirstAndShift(void)
{
  MEM_CHECK();

  if(NumberUnread == 1)
  {
    NumberUnread = 0;
    return;    
  }

  U16 i;

  for(i = 0 ; i < NumberUnread ; i++)
  {
    CommandPool[i] = CommandPool[i+1];
    LengthPool[i] = LengthPool[i+1];
  }

  NumberUnread--;

  if(MessageSize > 0)
  {
    U8 *temp = (U8 *)malloc(MessageSize * NumberUnread);
    if(temp == NULL)
    {
      /* @todo: system error */
      while(1){;}
    }
    memcpy(temp, &DataPool[MessageSize], MessageSize * NumberUnread);
    memcpy(DataPool, temp, MessageSize * NumberUnread);

    free(temp);
  }
}