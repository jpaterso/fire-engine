/**
 *  $Id: counter.h 113 2007-10-02 00:45:44Z jpaterso $
**/

#ifndef COUNTER_H_INCLUDED
#define COUNTER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{
//! A simple class that keeps count of something
class _FIRE_ENGINE_API_ Counter
{
public:
	//! Default ctor
	Counter(s32 initialCount = 0)
		: _Count(initialCount)
	{
	}

	//! Get the number of elements
	inline s32 getCount() const
	{
		return _Count;
	}

	inline void incrementCount()
	{
		_Count++;
	}

	inline void decrementCount()
	{
		_Count--;
	}

	inline void resetCount()
	{
		_Count = 0;
	}

	inline bool isEmpty() const
	{
		return _Count == 0;
	}

private:
	//! The number of elements
	s32 _Count;

};
}

#endif // COUNTER_H_INCLUDED
