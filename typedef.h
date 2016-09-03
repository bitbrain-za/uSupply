/*
 * typedef.h
 *
 * Created: 2016/07/20 11:10:10 AM
 *  Author: Philip
 */ 


#ifndef TYPEDEF_H_
#define TYPEDEF_H_


typedef uint8_t                 U8 ;  //!< 8-bit unsigned integer.
typedef uint16_t                U16;  //!< 16-bit unsigned integer.
typedef uint32_t                U32;  //!< 32-bit unsigned integer.
typedef unsigned long long int  U64;  //!< 64-bit unsigned integer.

typedef int8_t                 S8 ;  //!< 8-bit unsigned integer.
typedef int16_t                S16;  //!< 16-bit unsigned integer.
typedef int32_t                S32;  //!< 32-bit unsigned integer.

typedef enum
{
  NO_RESET,
  RESET
}FSM_CONTROL;


#endif /* TYPEDEF_H_ */