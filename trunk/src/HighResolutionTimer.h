/**
* FILE:    HighResolutionTimer.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: HighResolutionTimer.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: A high resolution, system independent timer.
**/

#ifndef HIGH_RESOLUTION_TIMER_H_INCLUDED
#define HIGH_RESOLUTION_TIMER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

#if defined(_FIRE_ENGINE_WIN32_)
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

namespace fire_engine
{
namespace sys
{

/** A high resolution system independent timer. This timer is precise up to
 one micro-second. */
class _FIRE_ENGINE_API_ HighResolutionTimer
{
public:
	/** Constructor. */
	HighResolutionTimer();

	/** Destructor. */
	~HighResolutionTimer();

	/** Start the timer. */
	void start();

	/** Returns the elapsed time since the timer was started, in microseconds. */
	f64 getElapsedTimeMicroSeconds();

	/** Returns the elapsed time since the timer was started, in miliseconds. */
	f64 getElapsedTimeMiliSeconds();

	/** Returns the elapsed time since the timer was started, in seconds. */
	f64 getElapsedTimeSeconds();
private:
	bool mIsStarted;
	f64 mStartingTimeMicroSeconds;
#if defined (_FIRE_ENGINE_WIN32_)
	LARGE_INTEGER mStartTime;
	f64           mInvTicksPerMicroSeconds;
#else
	struct timeval mStartTime;
#endif
};

}
}

#endif // HIGH_RESOLUTION_TIMER_H_INCLUDED
