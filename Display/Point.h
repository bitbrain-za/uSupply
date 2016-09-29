/* 
* Point.h
*
* Created: 2016/09/29 2:57:27 PM
* Author: Philip
*/


#ifndef __POINT_H__
#define __POINT_H__

class Point
{
//variables
public:
  U8 X;
  U8 Y;

//functions
public:
	Point() {;}
	Point(U8 x, U8 y) {X = x; y = y;}
	~Point() {;}

}; //Point

#endif //__POINT_H__
