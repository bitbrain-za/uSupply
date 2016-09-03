/* 
* timer.h
*
* Created: 2016/07/29 10:36:37 AM
* Author: Philip
*/


#ifndef __TIMER_H__
#define __TIMER_H__

#define MAX_TIMERS 5 

class timer
{
//variables
public:
  U16 Interval;
  bool Registered() { return (_id != 0xFF); }
protected:
private:
  bool _callback_set;
  bool _isr_flag;
  void (*CallBack)(void);
  U8 _id;
  U16 _count;
  U16 _interval;
  bool _running;

//functions
public:
	timer();
	~timer();

  bool init();
  bool init(U16 milliseconds);
  void Start();
  void Start(U16 milliseconds);
  void Restart();
  void Restart(U16 milliseconds);

  bool Expired() { return (_count >= _interval); }
  U16 Read() { return _count; }
  U16 Remaining() {return (_interval - _count); }
  void ForceExpire() { _count = _interval; }
  void Stop() { _running = false; }

  void SetCallback(void (*fp)(void)) { CallBack = fp; _callback_set = true; }
  void ClearCallback() { _callback_set = false; }

  void tick(U16 ms);

  void ClearInterrupt() { _isr_flag = false; }

protected:
private:
	timer( const timer &c );
	timer& operator=( const timer &c );

}; //timer

class timer_list
{
/* variables */
public:
private:
  static timer *timers[MAX_TIMERS];

/* functions */
public:
  static void init(void);
  static void tick(U16 ms);
  static U8 RegisterTimer(timer *t);
  static void DeregisterTimer(U8 id);
private:
};

#endif //__TIMER_H__
