/* 
* Line.h
*
* Created: 2016/09/29 2:58:43 PM
* Author: Philip
*/


#ifndef __LINE_H__
#define __LINE_H__


class Line
{
//variables
public:
  Point Start;
  Point End;
  double Gradient() { return (deltaY() / deltaX()); }
  S8 deltaX() { return (End.X - Start.X); }
  S8 deltaY() { return (End.Y - Start.Y); }
protected:
private:

//functions
public:
	Line();
	Line(Point start, Point end);
	~Line();
protected:
private:
	Line( const Line &c );
	Line& operator=( const Line &c );

}; //Line

#endif //__LINE_H__
