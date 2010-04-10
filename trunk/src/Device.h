/**
 *  $Id: Device.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The entry point for the user. This is where all core classes are created
 *  and managed.
 *  NOTE: a Device should *always* be created. The classes need to be created
 *  in a specific order, and some methods cannot be called unless previous
 *  methods have been. The Device simplifies this by doing it all properly
 *  itself.
**/

#ifndef __DEVICE_H
#define __DEVICE_H

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include "dimension2.h"
#include "string.h"
#include "array.h"
#include "Color.h"
#include "FPSCalculator.h"

namespace fire_engine
{

class Logger;
class IRenderer;
class IWindowManager;
class IEventReceiver;
class SceneManager;
class StringBundle;
class KeyEvent;
class MouseEvent;

namespace io
{
class FileSystem;
}

/** The 3D drivers currently supported by the system. */
enum EDRIVER_TYPE
{
	EDT_OPENGL = 0x01
};

/** The main class for fire engine. It should all start here. All the setup
 code for the critical components (scene management, rendering, window
 managing) is here. This class should *always* be created. */
class _FIRE_ENGINE_API_ Device : public virtual Object
{
	public:
		//! Destructor
		~Device();

		//! Get the Window Manager
		IWindowManager * getWindowManager();

		//! Get the Scene Manager
		SceneManager * getSceneManager();

		//! Get the String Bundle
		StringBundle * getStringBundle();

		//! Get the renderer currently being used
		IRenderer * getRenderer();

		/**
		 *	Create a device, or return the existing one.
		 *	@param	dType	The driver to use for rendering
		 *	@param	size	The size of the window to use
		 *	@param	title	The title of the window to create
		 *	@param	lang	The language of the engine
		 *	@return			A singleton instance of the Device
		**/
		static Device * Create(EDRIVER_TYPE dType = EDT_OPENGL,
			const dimension2i& size      = dimension2i(640, 480),
			const string& title          = "Default FireEngine Application",
			const c8 * lang              = "en");

		/**
		 *	Return the singleton instance of this class
		 *	@return	A pointer to the singleton instance of this class
		**/
		static Device * Get();

		/**
		 *	Set the background color, used when rendering
		 *	@param	c	The color of the background you wish to use
		**/
		void setBackgroundColor(const Color32& c);

		//! Add an IEventReceiver to the list of Receivers.
		/** Add an IEventReceiver to the list of Receivers. All the IEventReceivers in the list
		    will receive all the posted events */
		void addEventReceiver(IEventReceiver * er);

		//! close the Device, and all associated classes
		void close(void);

		//! Check whether the Device is still running
		bool isRunning(void) const;

		/** Post a KeyEvent message to the IEventReceivers. */
		void postMessage(KeyEvent& evt);

		/** Post a MouseEvent message to the IEventReceivers. */
		void postMessage(MouseEvent& evt);

		/** Sets the number of frames per second that fire engine is running at.
		 internal function, it is set automatically. Do not use. */
		void _setFPS(f32 fps);

		/** Returns the frames per second that fire engine is running at. */
		f64 getFPS() const;

		/** Returns the FPS calculator. Internal function - do not use. */
		FPSCalculator * _getFPSCalculator();

	private:
		static Device *        mInstance;   //! The singleton instance of the Device
		bool                   mIsRunning; //! Whether the Device is still running or not
		SceneManager *         mSceneManager;  //! The SceneManager, which manages all displayable items
		IWindowManager *       mWindowManager; //! The WindowManager, that creates and manages the window
		IRenderer *            mRenderer;      //! The Renderer, in charge of writing into the window
		array<IEventReceiver*> mEvtReceivers;         //! The Event Receiver - should be custom
		FPSCalculator *        mFPSCalculator;
		io::FileSystem *       mFileSystem;

		/**
		 *	Construct a device using a specific driver.
		 *	The constructor is made private in order to make sure there is only
		 *	a single instance of this class. The constructor is used privately,
		 *	by the Device::createDevice() method.
		 *	@param	dType	The driver to use for rendering
		 *	@param	size	The size of the window to use
		 *	@param	title	The title of the window to create
		 *	@param	lang	The language to use, for localised text
		**/
		Device(EDRIVER_TYPE dType, const dimension2i& size, const string& title, const c8 * lang);
};

} // namespace fire_engine

#endif
