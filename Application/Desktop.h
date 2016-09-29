/* 
* Desktop.h
*
* Created: 2016/09/05 2:31:22 PM
* Author: Philip
*/


#ifndef __DESKTOP_H__
#define __DESKTOP_H__


typedef enum
{
  DSKTP_CMD_RESET,
  DSKTP_CMD_REFRESH,
}DESKTOP_CMD;

class Desktop
{
//variables
public:
  MessageQueue commandQueue;
  void voltage(U16 val);
  bool disp_limit;

protected:
private:
  U16 voltage_read;
  bool voltage_changed;
  bool current_changed;
  bool menu_changed;
  LM6029ACW display;


//functions
public:
	Desktop();
	~Desktop();

  void FSM(FSM_CONTROL control);
protected:
private:
	Desktop( const Desktop &c );
	Desktop& operator=( const Desktop &c );

  void DisplayDesktop();
  void DrawButton(U8 position, char *text, bool invert);
  void ClearVoltageArea(bool invert);
  void ClearSecondArea(bool invert);
  void ClearMenuBar(bool invert);

  void invalidate() {voltage_changed = true; current_changed = true; menu_changed = true; }

}; //Desktop

#endif //__DESKTOP_H__
