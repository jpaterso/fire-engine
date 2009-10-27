/**
 *  $Id: ByteConverter.cpp 69 2007-08-01 10:15:13Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Definitions for ByteConverter class methods
**/

#include "ByteConverter.h"

namespace fire_engine {

u16 ByteConverter::ByteSwap(u16 in)
{
	return ((in & 0x00FF) << 8) | ((in & 0xFF00) >> 8);
}

s16 ByteConverter::ByteSwap(s16 in)
{
	return ((in & 0x00FF) << 8) | ((in & 0xFF00) >> 8);
}

s32 ByteConverter::ByteSwap(s32 in)
{
	return ((in & 0xFF000000) >> 24) | ((in & 0x00FF0000) >> 8)
		| ((in & 0x0000FF00) << 8) | ((in & 0x000000FF) << 24);
}

u32 ByteConverter::ByteSwap(u32 in)
{
	return ((in & 0xFF000000) >> 24) | ((in & 0x00FF0000) >> 8)
		| ((in & 0x0000FF00) << 8) | ((in & 0x000000FF) << 24);
}

f32 ByteConverter::ByteSwap(f32 in)
{
	s32 val = *(s32*)&in;
	val = ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8)
		| ((val & 0x0000FF00) << 8) | ((val & 0x000000FF) << 24);
	in = *(f32*)&val;
	return in;
}

} // namespace fe
