/**
 *  $Id: IWindowManager.h 116 2007-10-05 13:22:45Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Interface for generic window managers. Should provide facilities
 *  like creating a new window, and adding rendering callback functions.
**/

#ifndef	__IWINDOW_MANAGER_H
#define	__IWINDOW_MANAGER_H

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "Object.h"
#include "vector2.h"
#include "List.h"
#include "IResizable.h"

namespace fire_engine
{

class IRenderer;
class SceneManager;
class string;
class Device;
class ICursor;

class _FIRE_ENGINE_API_ IWindowManager : public virtual Object, public virtual IResizable
{
	public:
		/**
		 *	Basic destructor, does nothing
		**/
		virtual ~IWindowManager(void);

		/**
		 *	Set the title for the window
		 *	@param	newTitle	The new title for the window
		**/
		virtual void setTitle(const string& newTitle) = 0;

		/**
		 *	Call this when our window has been resized
		 *	@param	newSize	The new size of our window
		**/
		virtual void onResize(const dimension2i& newSize) = 0;

		/**
		 *	The function that the user must call in order for rendering to
		 *	proceed.
		 *	@return	true if more rendering is needed,
		 *			false if application is ready to exit
		**/
		virtual bool run(void) = 0;

		//! Check whether the WindowManager is still running
		bool isRunning(void) const;

		//! Close the main window
		virtual void close(void) = 0;

		/**
		 *	Rendering will be done using double-buffering, so we need to swap
		 *	them once rendering has been done
		**/
		virtual void swapBuffers(void) = 0;

		/**
		 *	Get the window size
		 *	@return	a dimension containing the current window size
		**/
		const dimension2i& getWindowSize(void) const;

		/** Returns whether the application is running in full screen mode or not. */
		bool isFullScreenToggled() const;

		/** Toggles full screen mode on or off. */
		virtual void toggleFullScreen() = 0;

		/**
		 *	Set the SceneManager. This way the WindowManager can call
		 *	run() when it needs to be repainted
		**/
		//TODO: Is this right? Should the WindowManager call run () ? Or
		//TODO: should the Device run () it all... Not sure...
		void setSceneManager(SceneManager * sc);

		/** Returns the mouse associated with the IWindowManager. */
		virtual ICursor * getCursor() = 0;

		/** Adds a resizable object to the list. */
		void addResizable(IResizable * const resizable);

		/** Removes a resizable object from the list of resizable objects.
		 \return Whether the item was correctly removed. */
		bool removeResizable(IResizable * const resizable);

		/** Returns the list of items that should be notified when the window is resized. */
		const List<IResizable*>& _getResizableItems() const;

		static IWindowManager * Get();
	protected:
		static IWindowManager * mInstance;   //! Singleton instance of our class
		u32                     m_driverType; //! The driver that our renderer is using
		dimension2i             mWindowSize;      //! The size of our window
		bool                    m_is_running; //! Whether the IWIndowManager is still running or not
		bool                    mIsFullScreen;
		List<IResizable*>       mResizableItems;

		//! Constructor - made protected so it can't be called directly - singleton instance
		IWindowManager(u32 dType, const dimension2i& wsize);

};

/** An interface to the cursor. */
class ICursor : public Object
{
public:
	virtual ~ICursor()
	{
	}

	/** Sets the relative position of the cursor, (0.0, 0.0) being the top left of the =
	 screen and (1.0, 1.0) being the bottom right. */
	virtual void setRelativePosition(const vector2f& newpos) = 0;

	/** Returns the relative position of the cursor, (0.0, 0.0) being the top left of the =
	screen and (1.0, 1.0) being the bottom right. */
	virtual vector2f getRelativePosition() const = 0;

	/** Sets whether the cursor is visible or not. */
	virtual void setCursorVisible(bool vis) = 0;
};

} // namespace fire_engine

#endif
