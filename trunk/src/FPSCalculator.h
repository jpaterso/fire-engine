/**
* FILE:    FPSCalculator.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: FPSCalculator.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: A simple class to calculate the number of frames per second.
**/

#ifndef FPSCALCULATOR_H_INCLUDED
#define FPSCALCULATOR_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "HighResolutionTimer.h"

namespace fire_engine
{

/** A class to keep track of the current number of frames per second
 that are being drawn. The FPSCalculator uses a system-dependent
 high-resolution timer to time the interval between calls to
 registerFrameDrawn(). The getFPS() method will therefore be a very
 high-precision and accurate frame-rate, updated at every single frame. */
class _FIRE_ENGINE_API_ FPSCalculator
{
public:
	/** Constructor. */
	FPSCalculator();

	/** Register that a new frame has been drawn, and (maybe) recalculate the FPS count. */
	void registerFrameDrawn(s32 polys);

	/** Returns the last FPS count. */
	f64 getFPS() const;

	/** Returns the average FPS count, since the FPSCalculator was started. */
	f64 getAverageFPS() const;

	/** Returns the total number of frames that have been drawn. */
	s32 getFramesDrawn() const;

	/** Returns the number of polygons drawn in the last frame. */
	s32 getLastPolyCount() const;

private:
	f64 mInitialTime; // the time at which the FPS Calculator was created
	f64 mLastCalculated;
	f64 mTimeNow; // the current time
	f64 mFPS; // the current FPS count
	f64 mAverageFPS; // the average FPS count
	s32 mFramesDrawn; // the total number of frames drawn
	s32 mLastFramePolyCount;
	sys::HighResolutionTimer mTimer;

};

}

#endif // FPSCALCULATOR_H_INCLUDED
