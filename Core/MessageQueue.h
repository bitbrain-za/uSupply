/* 
* qms.h
*
* Created: 2016/07/27 2:17:29 PM
* Author: Philip
*/


#ifndef __QMS_H__
#define __QMS_H__

typedef enum
{
  QUEUE_OKAY,
  QUEUE_EMPTY,
  QUEUE_FULL,
  QUEUE_NO_MEMORY,
  QUEUE_MESSAGE_ACCEPTED,
  QUEUE_ERROR,
}QueueStatus;

class MessageQueue
{
//variables
public:
protected:
private:
  QueueStatus status;
  U8 NumberUnread;
  U8 *CommandPool;
  U8 *LengthPool;
  U8 *DataPool;
  U8 MessageSize;
  U8 Length;


//functions
public:
  MessageQueue(U8 QueueDepth);
  MessageQueue(U8 QueueDepth, U8 DataSize);
  ~MessageQueue();

  QueueStatus Status(void) { return status; }

  bool IsEmpty(void) { return (NumberUnread == 0); }
  bool IsFull(void) { return (NumberUnread == Length); }

  bool Send(U8 cmd, U8 length, U8 *data);
  bool Peak(U8 *cmd, U8* lenght, U8 *data);
  bool Read(U8 *cmd, U8 *length, U8 *data);

  bool Send(U8 cmd) { return Send(cmd, 0, NULL); }
  bool Peak(U8 *cmd) { return Peak(cmd, NULL, NULL); }
  bool Read(U8 *cmd) { return Read(cmd, NULL, NULL); }

  void Clear(void);

protected:
private:
	MessageQueue( const MessageQueue &c );
	MessageQueue& operator=( const MessageQueue &c );

  void RemoveFirstAndShift(void);
};

#endif //__QMS_H__
