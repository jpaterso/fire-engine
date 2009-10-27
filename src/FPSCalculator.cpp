/**
* FILE:    FPSCalculator.cpp
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: FPSCalculator.cpp 110 2007-09-23 15:57:55Z jpaterso $
* PURPOSE: A simple class to calculate the number of frames per second.
**/

#include "FPSCalculator.h"
#include "Timer.h"

namespace fire_engine
{

FPSCalculator::FPSCalculator()
	: mFPS(0.0), mAverageFPS(0.0), mFramesDrawn(0), mLastFramePolyCount(0)
{
	mTimer.start();
	mInitialTime = mTimer.getElapsedTimeSeconds();
	mLastCalculated = mInitialTime;
}

void FPSCalculator::registerFrameDrawn(s32 polys)
{
	mTimeNow = mTimer.getElapsedTimeSeconds();
	mFramesDrawn++;
	mFPS = 1.0/(mTimeNow-mLastCalculated);
	mLastCalculated = mTimeNow;
	mAverageFPS = mFramesDrawn/(mTimeNow-mInitialTime);
	mLastFramePolyCount = polys;
}

f64 FPSCalculator::getFPS() const
{
	return mFPS;
}

f64 FPSCalculator::getAverageFPS() const
{
	return mAverageFPS;
}

s32 FPSCalculator::getFramesDrawn() const
{
	return mFramesDrawn;
}

s32 FPSCalculator::getLastPolyCount() const
{
	return mLastFramePolyCount;
}

}
