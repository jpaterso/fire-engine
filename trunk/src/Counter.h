/**
 *  $Id: Counter.h 113 2007-10-02 00:45:44Z jpaterso $
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
	Counter()
		: mCount(0)
	{
	}

	//! Get the number of elements
	inline s32 getCount() const
	{
		return mCount;
	}

	inline void incrementCount()
	{
		mCount++;
	}

	inline void decrementCount()
	{
		mCount--;
	}

	inline void resetCount()
	{
		mCount = 0;
	}

	inline bool isEmpty() const
	{
		return mCount == 0;
	}

private:
	//! The number of elements
	s32 mCount;

};
}

#endif // COUNTER_H_INCLUDED
