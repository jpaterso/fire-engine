/**
 * FILE:    Timer.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Timer.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A utility to calculate the amount of time that has passed since
 *          the program started execution.
**/

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include <time.h>

namespace fire_engine
{

/** A simple Timer class, that lets users know how long it has been alive
 for. It can be useful for monitoring fps, etc... */
class _FIRE_ENGINE_API_ Timer : public virtual Object
{
public:
	//! Only a singleton instance may exist.
	/** Create the Timer if it does not exist, and return it. */
	static Timer * Create();

	/** Returns the singleton instance of the Timer/ */
	static inline Timer * Get();

	/** Destructor */
	virtual ~Timer();

	/** Sets the time for all subsequent calls to getTime(), until the next call
	 to updateTime(). This means that time can be held 'constant' if needed */
	f32 updateTime();

	/** Get the time that was set with the last call to updateTime(). All calls
	 to getTime() will return the same value until a new call to updateTime() is
	 made.
	 \return The time in seconds set at the last call to updateTime(). */
	inline f32 getTime() const;

private:
	static Timer * m_instance;
	static f32     m_clocks_per_sec;
	clock_t        m_initial_time;
	f32            m_time;

	/** Private constructor to ensure it has a singleton instance. */
	Timer();
};

inline Timer * Timer::Get()
{
	return m_instance;
}

inline f32 Timer::getTime() const
{
	return m_time;
}

}

#endif
