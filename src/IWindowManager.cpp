/**
 * FILE:    IWindowManager.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IWindowManager.cpp 114 2007-10-04 04:57:03Z jpaterso $
 * PURPOSE: static members of the IWindowManager class
**/

#include "IWindowManager.h"
#include "Logger.h"
#include "Device.h"

namespace fire_engine
{

IWindowManager * IWindowManager::mInstance = 0;

IWindowManager * IWindowManager::Get()
{
	return mInstance;
}

IWindowManager::IWindowManager(u32 dType, const dimension2i& wsize)
	: m_driverType(dType),
	  mWindowSize(wsize),
	  m_is_running(true)
{
	if (Device::Get()->getRenderer() == 0)
		Logger::Get()->log(ES_CRITICAL, "IWindowManager", "Initializing with NULL renderer");
	mResizableItems.push_back(this);
}

IWindowManager::~IWindowManager(void)
{
	mResizableItems.removeElement(this);
}

bool IWindowManager::isRunning(void) const
{
	return m_is_running;
}

const dimension2i& IWindowManager::getWindowSize(void) const
{
	return mWindowSize;
}

bool IWindowManager::isFullScreenToggled() const
{
	return mIsFullScreen;
}

void IWindowManager::addResizable(IResizable * const resizable)
{
	mResizableItems.push_back(resizable);
}

bool IWindowManager::removeResizable(IResizable * const resizable)
{
	return mResizableItems.removeElement(resizable);
}

const List<IResizable*>& IWindowManager::_getResizableItems() const
{
	return mResizableItems;
}

}
