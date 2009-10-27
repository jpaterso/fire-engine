/**
 *  $Id: ByteConverter.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Useful class for changing byte ordering
**/

#ifndef	__BYTE_CONVERTER_H
#define	__BYTE_CONVERTER_H

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine {

class _FIRE_ENGINE_API_ ByteConverter {
	public:
		static u16 ByteSwap(u16 in);

		/**
		 *	Invert the order of bytes in 16 bit unsigned variable
		 *	This will convert lo-hi to hi-low and vice versa
		 *	@param	in	The input
		 *	@return		the input with bytes inversed
		**/
		static s16 ByteSwap(s16 in);

		static u32 ByteSwap(u32 in);

		/**
		 *	Invert the order of bytes in 32 bit unsigned variable
		 *	This will convert lo-hi to hi-low and vice versa
		 *	@param	in	The input
		 *	@return		the input with bytes inversed
		**/
		static s32 ByteSwap(s32 in);

		static f32 ByteSwap(f32 in);
};

} // namespace fire_engine

#endif
