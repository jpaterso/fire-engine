/**
 *  $Id: Device.cpp 116 2007-10-05 13:22:45Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The entry point for the user. This is where all core classes are created
 *  and managed.
 *  NOTE: a Device should *always* be created. The classes need to be created
 *  in a specific order, and some methods cannot be called unless previous
 *  methods have been. The Device simplifies this by doing it all properly
 *  itself.
**/

#include "Device.h"
#include "Logger.h"
#include "Timer.h"
#include "SceneManager.h"
#include "IEventReceiver.h"
#include "IWindowManager.h"
#include "FileSystem.h"
#if defined(_FIRE_ENGINE_WIN32_)
#	include "WindowManagerWin32.h"
#endif
#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
#	include "OpenGLRenderer.h"
#endif

namespace fire_engine
{

Device * Device::mInstance = 0;

Device::Device(EDRIVER_TYPE dType, const dimension2i& size, const string& title, const c8 * lang)
	: Object(), mIsRunning(true)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Device");
#endif
	mInstance = this;
	Math32::Create();
    Logger::Create();
	io::FileSystem::Create();
	mFPSCalculator = new FPSCalculator();
#if defined(_FIRE_ENGINE_DEBUG_ALL_)
	Logger::Get()->log(ES_DEBUG, "Device", "DEBUG MODE ACTIVATED");
#endif

#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
	if (dType == EDT_OPENGL)
		mRenderer = OpenGLRenderer::Create();
#else
	Logger::Get()->log(ES_CRITICAL,
		string("Device"),
		string("Only OpenGL is currently supported"));
#endif
#if defined(_FIRE_ENGINE_WIN32_)
	mWindowManager = WindowManagerWin32::Create(dType, size, title);
#else
	Logger::Get()->log(ES_CRITICAL,
		string("Device"),
		string("Only Win32 systems are currently supported"));
#endif

	mSceneManager = SceneManager::Create(mRenderer);

	mRenderer->init();
	mRenderer->onResize(mWindowManager->getWindowSize());
}

Device * Device::Create(EDRIVER_TYPE dType, const dimension2i& size, const string& title, const c8 * lang)
{
	if (mInstance == 0)
		mInstance = new Device(dType, size, title, lang);
	return (mInstance);
}

IWindowManager * Device::getWindowManager(void)
{
	return mWindowManager;
}

SceneManager * Device::getSceneManager(void)
{
	return mSceneManager;
}

IRenderer * Device::getRenderer(void)
{
	return mRenderer;
}

Device * Device::Get(void)
{
	return mInstance;
}

void Device::addEventReceiver(IEventReceiver * er)
{
	mEvtReceivers.push_back(er);
}

void Device::close(void)
{
	mWindowManager->close();
	mIsRunning = false;
}

bool Device::isRunning(void) const
{
	return mIsRunning && mWindowManager->isRunning();
}

void Device::postMessage(KeyEvent& evt)
{
	for (s32 i = 0; i < mEvtReceivers.size(); i++)
		mEvtReceivers.at(i)->onKeyEvent(evt);
}

void Device::postMessage(MouseEvent& evt)
{
	for (s32 i = 0; i < mEvtReceivers.size(); i++)
		mEvtReceivers.at(i)->onMouseEvent(evt);
}

f64 Device::getFPS() const
{
	return mFPSCalculator->getFPS();
}

FPSCalculator * Device::_getFPSCalculator()
{
	return mFPSCalculator;
}

Device::~Device(void)
{
	if (mSceneManager)
		mSceneManager->drop();
	if (mWindowManager)
		mWindowManager->drop();
	if (mRenderer)
		mRenderer->drop();
	delete mFPSCalculator;
	delete Math32::Get();
	delete io::FileSystem::Get();
}
} // namespace fire_engine
