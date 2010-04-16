/**
 *  $Id: WindowManagerWin32.h 114 2007-10-04 04:57:03Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The Win32 window manager.
**/

#ifndef __WINDOW_MANAGER_WIN32_H
#define __WINDOW_MANAGER_WIN32_H

#include <windows.h>

#include "CompileConfig.h"
#include "Types.h"
#include "dimension2.h"
#include "IWindowManager.h"
#include "HashTable.h"
#include "MouseEvent.h"
#include "Device.h"
#include "Object.h"
#include "KeyEvent.h"

namespace fire_engine
{

class String;
class IRenderer;

class _FIRE_ENGINE_API_ WindowManagerWin32 : public IWindowManager, public virtual Object
{
	private:
		/** Cursor control class for Win32 systems. */
		class CursorWin32 : public ICursor
		{
		public:
			/** Constructor. */
			CursorWin32(WindowManagerWin32 * owner);

			virtual ~CursorWin32();

			virtual void setRelativePosition(const vector2f& newpos);

			virtual vector2f getRelativePosition() const;

			virtual void setCursorVisible(bool vis);

		private:
			WindowManagerWin32 * mOwner;
		};

	public:
		/** Destructor. */
		virtual ~WindowManagerWin32(void);

		virtual void setTitle(const String& newTitle);

		virtual void onResize(const dimension2i& newSize);

		virtual bool run(void);

		virtual void swapBuffers(void);

		virtual void close(void);

		virtual void toggleFullScreen();

		virtual ICursor * getCursor();

		//! Set whether the window is still running or not
		void setRunning(bool r);

		/**
		 *	Create a  singleton instance of the WindowManager
		 *	This acts as a sort of Constructor.
		 *	@param	dType	The driver to use. FE_OPENGL is
		 *					currently supported
		 *	@param	rd		The renderer currently being used
		 *	@param	wSize	The size of the window to create
		 *	@param	title	The default title of the window
		 *	Note that all parameters are ignored when the m_instance field
		 *	of the WindowManagerWin32 is already set.
		**/
		static IWindowManager * Create(EDRIVER_TYPE dType,
			const dimension2i& wSize,
			const String& title);

	private:
		HINSTANCE                   mHinstance; //! The instance of our program
		WNDCLASSEX                  mHWndClass; //! Our window class
		HWND                        mHwnd;      //! The main window
		PIXELFORMATDESCRIPTOR       mPFD;       //! The pixel format to use
		HDC                         mHDC;       //! The rendering context
#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
		HGLRC                       mHGLRC;     //! The OpenGL rendering context
#endif
		MSG                         mMsg;       //! Messages we read and transmit
		static HashTable<s32, KeyEvent::EKEY_CODE> * mKeyCodeHT;
		ICursor *                    mCursor;

		DWORD mSavedExStyle, mSavedStyle;
		RECT mOldSize;

		//! Hash tables used to generate MouseEvents faster
		static HashTable<UINT, MouseEvent::MouseButton> *    m_msg_to_mbutton;
		static HashTable<UINT, MouseEvent::MouseEventType> * m_msg_to_mevent_type;
		static HashTable<UINT, s32> *                        m_msg_to_click_count;


		/**
		 *	Creates a window capable of rendering using a specified driver
		 *	This function must be private to ensure only a single copy of the
		 *	WindowManager.
		 *	@param	dType	The driver to use. Only FE_OPENGL is
		 *					currently supported.
		 *	@param	rd		The renderer currently being used.
		 *	@param	wSize	The size of the window to create.
		 *	@param	title	The default title of the window.
		**/
		WindowManagerWin32(u32 dType,
			const dimension2i& wSize,
			const String& title);

		/**
		 *	Set the pixel format for the newly created window
		**/
		void setPixelFormat(void);

#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
		//! Initialize the OpenGL context
		void createOpenGLContext(void);

		//! Release the OpenGL context
		void releaseOpenGLContext(void);
#endif

		/**
		 *	Our callback function, it will be called automatically by Windows
		**/
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		//! Create a KeyEvent from a user keystroke
		static KeyEvent fromKeyStroke(WPARAM wParam);

		//! Create a mouse event
		static MouseEvent fromMouse(UINT msg, WPARAM wParam, LPARAM lParam);

		//! Initialize the hash table of virtual key codes
		static void initKeyCodeHashTable(void);
		static void initMouseHashTables(void);

		/** Goes into full screen size, in native mode (no control over the resolution). */
		void setFullScreen();
};

} // namespace fire_engine
#endif
