/**
 *  $Id: Types.h 7 2007-06-19 03:21:11Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Contains typedefs of basic types
**/

#ifndef	__TYPES_H
#define	__TYPES_H

namespace fire_engine 
{

typedef	unsigned char      u8;
typedef	signed char        s8;
typedef	char               c8;
typedef unsigned short     u16;
typedef signed short       s16;
typedef unsigned int       u32;
typedef	signed int         s32;
typedef	float              f32;
typedef unsigned long long u64;
typedef long long          s64;
typedef	double             f64;

} // namespace fire_engine

/** C++ 0x will introduce the nullptr keyword. We define it for now as being
 like the NULL macro, a void pointer to zero. */
#if !defined(nullptr)
#	define nullptr 0x00
#endif

#endif
