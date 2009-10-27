/**
 * FILE:    Timer.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Timer.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of the Timer class.
**/

#include "Timer.h"
#include <stdio.h>

namespace fire_engine
{

Timer * Timer::m_instance = 0;
f32     Timer::m_clocks_per_sec = CLOCKS_PER_SEC;

Timer::Timer()
	: m_time(0.0f)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Timer");
#endif
	m_initial_time = clock();
}

Timer * Timer::Create()
{
	if (m_instance == 0)
		m_instance = new Timer();
	return m_instance;
}

Timer::~Timer()
{
	m_instance = 0;
}

f32 Timer::updateTime()
{
	m_time = (clock()-m_initial_time)/m_clocks_per_sec;
	return m_time;
}

}
