
#include "HighResolutionTimer.h"

namespace fire_engine
{
namespace sys
{

HighResolutionTimer::HighResolutionTimer()
	: mIsStarted(false)
{
#if defined(_FIRE_ENGINE_WIN32_)
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	mInvTicksPerMicroSeconds = 1000000.0f / freq.QuadPart;
#endif
}

HighResolutionTimer::~HighResolutionTimer()
{
}

void HighResolutionTimer::start()
{
	mIsStarted = true;
#if defined(_FIRE_ENGINE_WIN32_)
	QueryPerformanceCounter(&mStartTime);
	mStartingTimeMicroSeconds = mStartTime.QuadPart * mInvTicksPerMicroSeconds;
#else
	gettimeofday(&mStartTime);
	mStartingTimeMicroSeconds = mStartTime.tv_sec * 1000000.0 + mStartTime.tv_usec;
#endif
}

f64 HighResolutionTimer::getElapsedTimeMicroSeconds()
{
	if (!mIsStarted)
		return 0.0;
#if defined(_FIRE_ENGINE_WIN32_)
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);
	return cur.QuadPart * mInvTicksPerMicroSeconds - mStartingTimeMicroSeconds;
#else
	struct timeval now;
	gettimeofday(&now);
	return now.tv_sec * 1000000.0 + now.tv_usec - mStartingTimeMicroSeconds;
#endif
}

f64 HighResolutionTimer::getElapsedTimeMiliSeconds()
{
	return this->getElapsedTimeMicroSeconds() * 0.001;
}

f64 HighResolutionTimer::getElapsedTimeSeconds()
{
	return this->getElapsedTimeMicroSeconds() * 0.000001;
}

}
}
