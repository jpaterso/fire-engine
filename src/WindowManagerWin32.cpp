/**
 *  $Id: WindowManagerWin32.cpp 116 2007-10-05 13:22:45Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  WindowManager for Win32. Manages windows, and callback functions
**/

#include "WindowManagerWin32.h"
#include "Logger.h"
#include "IRenderer.h"
#include "InputEvent.h"
#include "MouseEvent.h"
#include "String.h"
#include "IResizable.h"

#define _WINDOW_CLASS_NAME	"__FireEngineWin32ClassName"

namespace fire_engine
{

HashTable<s32, KeyEvent::EKEY_CODE> *         WindowManagerWin32::mKeyCodeHT           = 0;
HashTable<UINT, MouseEvent::MouseButton> *    WindowManagerWin32::m_msg_to_mbutton     = 0;
HashTable<UINT, MouseEvent::MouseEventType> * WindowManagerWin32::m_msg_to_mevent_type = 0;
HashTable<UINT, s32> *                        WindowManagerWin32::m_msg_to_click_count = 0;

WindowManagerWin32::WindowManagerWin32(u32 dType, const dimension2i& wSize, const String& title)
	: IWindowManager(dType, wSize)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::WindowManagerWin32");
#endif
	mInstance  = this; // this line *has* to be here, since the callback function needs
	                    // an instance (for WM_RESIZE), and it will be called before the
	                    // constructor returns
	mHinstance = GetModuleHandle(NULL);
	initKeyCodeHashTable();
	initMouseHashTables();
	mIsFullScreen = false;
	mCursor = new CursorWin32(this);

	{ // Register the style of our window
		mHWndClass.cbSize        = sizeof(WNDCLASSEX);
		// remove CS_DBLCLKS to not catch double clicks
		mHWndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		mHWndClass.lpfnWndProc   = WindowManagerWin32::WndProc;
		mHWndClass.cbClsExtra    = 0;
		mHWndClass.cbWndExtra    = 0;
		mHWndClass.hInstance     = mHinstance;
		mHWndClass.hIcon         = NULL;
		mHWndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		mHWndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		mHWndClass.lpszMenuName  = NULL;
		mHWndClass.lpszClassName = _WINDOW_CLASS_NAME;
		mHWndClass.hIconSm       = NULL;

		if (! RegisterClassEx(&mHWndClass))
			Logger::Get()->log(ES_CRITICAL,
				String("WindowManagerWin32"),
				String("Could not register class"));
	}

	// Create our window
	mHwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		_WINDOW_CLASS_NAME,
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		mWindowSize.getWidth(),
		mWindowSize.getHeight(),
		NULL,
		NULL,
		mHinstance,
		NULL);

	if (!mHwnd)
		Logger::Get()->log(ES_CRITICAL,
			String("WindowManagerWin32"),
			String("Failed to create window"));

	mHDC = GetDC(mHwnd);
	setPixelFormat();

	// Initialize our window
	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
	if (dType == EDT_OPENGL)
		createOpenGLContext();
#endif
}

WindowManagerWin32::~WindowManagerWin32()
{
#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
	if (mHGLRC != NULL)
		releaseOpenGLContext();
#endif
	delete mKeyCodeHT;
	delete m_msg_to_mbutton;
	delete m_msg_to_mevent_type;
	delete m_msg_to_click_count;
	mCursor->drop();
}

void WindowManagerWin32::setPixelFormat(void)
{
	s32 nPixelFormat;
	mPFD.nSize             = sizeof(PIXELFORMATDESCRIPTOR);
	mPFD.nVersion          = 1;
	mPFD.dwFlags           = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	mPFD.iPixelType        = PFD_TYPE_RGBA;
	mPFD.cColorBits        = 32; //TODO: Do we really need this field?
	mPFD.cRedBits          = 0;
	mPFD.cRedShift         = 0;
	mPFD.cGreenBits        = 0;
	mPFD.cGreenShift       = 0;
	mPFD.cBlueBits         = 0;
	mPFD.cBlueShift        = 0;
	mPFD.cAlphaBits        = 0;
	mPFD.cAlphaShift       = 0;
	mPFD.cAccumBits        = 0;
	mPFD.cAccumRedBits     = 0;
	mPFD.cAccumBlueBits    = 0;
	mPFD.cAccumGreenBits   = 0;
	mPFD.cAccumAlphaBits   = 0;
	mPFD.cDepthBits        = 24; //TODO: Check depth bits
	mPFD.cStencilBits      = 1; //TODO: What the heck is a stencil buffer?
	mPFD.cAuxBuffers       = 0;  //TODO: What the heck is an auxiliary buffer?
	mPFD.iLayerType        = PFD_MAIN_PLANE;
	mPFD.bReserved	        = 0;
	mPFD.dwLayerMask       = 0;  // Obsolete - this is ignored
	mPFD.dwVisibleMask     = 0;
	mPFD.dwDamageMask      = 0;  // Obsolete - this is ignored
	nPixelFormat = ChoosePixelFormat(mHDC, &mPFD);
	SetPixelFormat(mHDC, nPixelFormat, &mPFD);
}

#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
void WindowManagerWin32::createOpenGLContext(void)
{
	// Create and set the OpenGL context
	if ((mHGLRC = wglCreateContext(mHDC)) == 0)
		Logger::Get()->log(ES_CRITICAL, "WindowManagerWin32", "Failed to create OpenGL context");
#if	defined(_FIRE_ENGINE_DEBUG_OPENGL_)
	else
		Logger::Get()->log(ES_DEBUG, "WindowManagerWin32", "OpenGL context created");
#endif

	if (wglMakeCurrent(mHDC, mHGLRC) == FALSE)
		Logger::Get()->log(ES_CRITICAL, "WindowManagerWin32", "Failed to set OpenGL context");
#if	defined(_FIRE_ENGINE_DEBUG_OPENGL_)
	else
		Logger::Get()->log(ES_DEBUG, "WindowManagerWin32", "OpenGL context set");
#endif
	//TODO: Note: only after this has been run can we use OpenGL functions
}

void WindowManagerWin32::releaseOpenGLContext(void)
{
	if (wglMakeCurrent(mHDC, NULL) == FALSE)
		Logger::Get()->log(ES_HIGH, "WindowManagerWin32", "Failed to release OpenGL context");

	wglDeleteContext(mHGLRC);
#if defined(_FIRE_ENGINE_DEBUG_OPENGL_)
	Logger::Get()->log(ES_DEBUG, "WindowManagerWin32", "OpenGL context deleted");
#endif
}
#endif

void WindowManagerWin32::setTitle(const String& newTitle)
{
	if (!mHwnd)
		return;
	SetWindowText(mHwnd, newTitle.c_str());
}

void WindowManagerWin32::onResize(const dimension2i& newSize)
{
	mWindowSize = newSize;
	Device::Get()->getRenderer()->onResize(mWindowSize);
}

IWindowManager *
WindowManagerWin32::Create(EDRIVER_TYPE dType, const dimension2i& wSize, const String& title)
{
	if (mInstance == 0)
		mInstance = new WindowManagerWin32(dType, wSize, title);
	return mInstance;
}

bool WindowManagerWin32::run(void)
{
	// PeekMessage doesn't block (ie. returns even if there are no messages), whereas
	// GetMessage blocks.
	if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE) == TRUE)
	//if (GetMessage(&m_msg, NULL, 0, 0) == TRUE)
	{
		TranslateMessage(&mMsg);
		DispatchMessage(&mMsg);
		return true;
	}
	return false;
}

void WindowManagerWin32::close(void)
{
	SendMessage(mHwnd, WM_CLOSE, 0, 0);
}

inline void WindowManagerWin32::swapBuffers(void)
{
	SwapBuffers(mHDC);
}

void WindowManagerWin32::setRunning(bool is_running)
{
	m_is_running = is_running;
}

void WindowManagerWin32::toggleFullScreen()
{
	mIsFullScreen = !mIsFullScreen;
	setFullScreen();
}

ICursor * WindowManagerWin32::getCursor()
{
	return mCursor;
}

void WindowManagerWin32::setFullScreen()
{
	if (mIsFullScreen)
	{
		mSavedExStyle = GetWindowLong(mHwnd, GWL_EXSTYLE);
		mSavedStyle = GetWindowLong(mHwnd, GWL_STYLE);
		GetWindowRect(mHwnd, &mOldSize);
		SetWindowLong(mHwnd, GWL_EXSTYLE, 0);
		SetWindowLong(mHwnd, GWL_STYLE, WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		//SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED|SWP_SHOWWINDOW);
		mWindowSize.setWidth(GetSystemMetrics(SM_CXSCREEN));
		mWindowSize.setHeight(GetSystemMetrics(SM_CYSCREEN));
		SetWindowPos(mHwnd, HWND_TOPMOST, 0, 0, mWindowSize.getWidth(), mWindowSize.getHeight(), SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLong(mHwnd, GWL_EXSTYLE, mSavedExStyle);
		SetWindowLong(mHwnd, GWL_STYLE, mSavedStyle);
		//SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED|SWP_SHOWWINDOW);
		mWindowSize.setWidth(mOldSize.right-mOldSize.left);
		mWindowSize.setHeight(mOldSize.bottom-mOldSize.top);
		SetWindowPos(mHwnd, HWND_NOTOPMOST, mOldSize.left, mOldSize.top,
			mWindowSize.getWidth(), mWindowSize.getHeight(), SWP_SHOWWINDOW);
	}
	this->onResize(mWindowSize);
}

LRESULT CALLBACK WindowManagerWin32::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	KeyEvent k_evt;
	MouseEvent m_evt;
	RECT r;
    switch (Msg)
    {
		case WM_SIZE:
			GetClientRect(hWnd, &r);
			if ((r.right%2) != 0)
				r.right++;
			if ((r.bottom%2) != 0)
				r.bottom++;
			for (List<IResizable*>::iterator it = mInstance->_getResizableItems().begin();
				it != mInstance->_getResizableItems().end(); it++)
			{
				(*it)->onResize(dimension2i(r.right, r.bottom));
			}
			break;

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN: // Left button down
		case WM_LBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
#if defined(_FIRE_ENGINE_GENERATE_MOUSE_MOVED_EVENTS_)
		case WM_MOUSEMOVE:
#endif
		case WM_MOUSELEAVE:
		//case WM_MOUSEACTIVATE:
		//case WM_MOUSEFIRST:
		//case WM_MOUSEHOVER:
		//case WM_MOUSELAST:
		//case WM_MOUSEWHEEL:
			m_evt = fromMouse(Msg, wParam, lParam);
			Device::Get()->postMessage(m_evt);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
			dynamic_cast<WindowManagerWin32*>(mInstance)->setRunning(false);
			return DefWindowProc(hWnd, Msg, wParam, lParam);

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			k_evt = fromKeyStroke(wParam);
			Device::Get()->postMessage(k_evt);
			break;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
			break;
	}
	return 0;
}

KeyEvent WindowManagerWin32::fromKeyStroke(WPARAM wParam)
{
	u8 mask  = 0;
	KeyEvent::EKEY_CODE * key = 0;
	if (GetKeyState(VK_CONTROL) < 0)
		mask |= InputEvent::CTRL_MASK;
	if (GetKeyState(VK_SHIFT) < 0)
		mask |= InputEvent::SHIFT_MASK;
	if (GetKeyState(VK_CAPITAL) > 0)
		mask |= InputEvent::CAPS_MASK;
	if (GetKeyState(VK_MENU) < 0)
		mask |= InputEvent::ALT_MASK;
	if (GetKeyState(VK_LBUTTON) < 0)
		mask |= InputEvent::BUTTON1_MASK;
	if (GetKeyState(VK_MBUTTON) < 0)
		mask |= InputEvent::BUTTON2_MASK;
	if (GetKeyState(VK_RBUTTON) < 0)
		mask |= InputEvent::BUTTON3_MASK;
	key = mKeyCodeHT->find(wParam);
	return key == 0 ? KeyEvent(mask, KeyEvent::KEY_UNKNOWN) : KeyEvent(mask, *key);
}

MouseEvent WindowManagerWin32::fromMouse(UINT msg, WPARAM wParam, LPARAM lParam)
{
	MouseEvent                evt;
	MouseEvent::MouseButton * button;
	s32 *                     clicks;
	u32                       mask = 0;
	if (wParam & MK_CONTROL)
		mask &= InputEvent::CTRL_MASK;
	if (wParam & MK_LBUTTON)
		mask &= InputEvent::BUTTON1_MASK;
	if (wParam & MK_MBUTTON)
		mask &= InputEvent::BUTTON3_MASK;
	if (wParam & MK_RBUTTON)
		mask &= InputEvent::BUTTON2_MASK;
	if (wParam & MK_SHIFT)
		mask &= InputEvent::SHIFT_MASK;
	evt.setMask(mask);
	evt.setPoint(dimension2i(LOWORD(lParam), HIWORD(lParam)));
	if ((button = m_msg_to_mbutton->find(msg)) != 0)
		evt.setButton(*button);
	evt.setType(*m_msg_to_mevent_type->find(msg));
	if ((clicks = m_msg_to_click_count->find(msg)) != 0)
		evt.setClickCount(*clicks);
	return evt;
}

void WindowManagerWin32::initKeyCodeHashTable(void)
{
	mKeyCodeHT = new HashTable<s32, KeyEvent::EKEY_CODE>(255);
	mKeyCodeHT->insert(65, KeyEvent::KEY_A);
	mKeyCodeHT->insert(66, KeyEvent::KEY_B);
	mKeyCodeHT->insert(67, KeyEvent::KEY_C);
	mKeyCodeHT->insert(68, KeyEvent::KEY_D);
	mKeyCodeHT->insert(69, KeyEvent::KEY_E);
	mKeyCodeHT->insert(70, KeyEvent::KEY_F);
	mKeyCodeHT->insert(71, KeyEvent::KEY_G);
	mKeyCodeHT->insert(72, KeyEvent::KEY_H);
	mKeyCodeHT->insert(73, KeyEvent::KEY_I);
	mKeyCodeHT->insert(74, KeyEvent::KEY_J);
	mKeyCodeHT->insert(75, KeyEvent::KEY_K);
	mKeyCodeHT->insert(76, KeyEvent::KEY_L);
	mKeyCodeHT->insert(77, KeyEvent::KEY_M);
	mKeyCodeHT->insert(78, KeyEvent::KEY_N);
	mKeyCodeHT->insert(79, KeyEvent::KEY_O);
	mKeyCodeHT->insert(80, KeyEvent::KEY_P);
	mKeyCodeHT->insert(81, KeyEvent::KEY_Q);
	mKeyCodeHT->insert(82, KeyEvent::KEY_R);
	mKeyCodeHT->insert(83, KeyEvent::KEY_S);
	mKeyCodeHT->insert(84, KeyEvent::KEY_T);
	mKeyCodeHT->insert(85, KeyEvent::KEY_U);
	mKeyCodeHT->insert(86, KeyEvent::KEY_V);
	mKeyCodeHT->insert(87, KeyEvent::KEY_W);
	mKeyCodeHT->insert(88, KeyEvent::KEY_X);
	mKeyCodeHT->insert(89, KeyEvent::KEY_Y);
	mKeyCodeHT->insert(90, KeyEvent::KEY_Z);
	mKeyCodeHT->insert(VK_NUMPAD0, KeyEvent::KEY_0);
	mKeyCodeHT->insert(VK_NUMPAD1, KeyEvent::KEY_1);
	mKeyCodeHT->insert(VK_NUMPAD2, KeyEvent::KEY_2);
	mKeyCodeHT->insert(VK_NUMPAD3, KeyEvent::KEY_3);
	mKeyCodeHT->insert(VK_NUMPAD4, KeyEvent::KEY_4);
	mKeyCodeHT->insert(VK_NUMPAD5, KeyEvent::KEY_5);
	mKeyCodeHT->insert(VK_NUMPAD6, KeyEvent::KEY_6);
	mKeyCodeHT->insert(VK_NUMPAD7, KeyEvent::KEY_7);
	mKeyCodeHT->insert(VK_NUMPAD8, KeyEvent::KEY_8);
	mKeyCodeHT->insert(VK_NUMPAD9, KeyEvent::KEY_9);
	mKeyCodeHT->insert(VK_BACK, KeyEvent::KEY_BACKSPACE);
	mKeyCodeHT->insert(VK_TAB, KeyEvent::KEY_TAB);
	mKeyCodeHT->insert(VK_RETURN, KeyEvent::KEY_RETURN);
	mKeyCodeHT->insert(VK_SHIFT, KeyEvent::KEY_SHIFT);
	mKeyCodeHT->insert(VK_CONTROL, KeyEvent::KEY_CONTROL);
	mKeyCodeHT->insert(VK_MENU, KeyEvent::KEY_ALT);
	mKeyCodeHT->insert(VK_PAUSE, KeyEvent::KEY_PAUSE);
	mKeyCodeHT->insert(VK_CAPITAL, KeyEvent::KEY_CAPS);
	mKeyCodeHT->insert(VK_ESCAPE, KeyEvent::KEY_ESCAPE);
	mKeyCodeHT->insert(VK_SPACE, KeyEvent::KEY_SPACE);
	mKeyCodeHT->insert(VK_PRIOR, KeyEvent::KEY_PGUP);
	mKeyCodeHT->insert(VK_NEXT, KeyEvent::KEY_PGDOWN);
	mKeyCodeHT->insert(VK_END, KeyEvent::KEY_END);
	mKeyCodeHT->insert(VK_HOME, KeyEvent::KEY_HOME);
	mKeyCodeHT->insert(VK_LEFT, KeyEvent::KEY_LEFT);
	mKeyCodeHT->insert(VK_UP, KeyEvent::KEY_UP);
	mKeyCodeHT->insert(VK_RIGHT, KeyEvent::KEY_RIGHT);
	mKeyCodeHT->insert(VK_DOWN, KeyEvent::KEY_DOWN);
	mKeyCodeHT->insert(VK_SELECT, KeyEvent::KEY_SELECT);
	mKeyCodeHT->insert(VK_INSERT, KeyEvent::KEY_INSERT);
	mKeyCodeHT->insert(VK_DELETE, KeyEvent::KEY_DELETE);
	mKeyCodeHT->insert(VK_MULTIPLY, KeyEvent::KEY_MULTIPLY);
	mKeyCodeHT->insert(VK_ADD, KeyEvent::KEY_ADD);
	mKeyCodeHT->insert(VK_SUBTRACT, KeyEvent::KEY_SUBTRACT);
	mKeyCodeHT->insert(VK_DIVIDE, KeyEvent::KEY_DIVIDE);
	mKeyCodeHT->insert(VK_DECIMAL, KeyEvent::KEY_DECIMAL);
	mKeyCodeHT->insert(VK_F1, KeyEvent::KEY_F1);
	mKeyCodeHT->insert(VK_F2, KeyEvent::KEY_F2);
	mKeyCodeHT->insert(VK_F3, KeyEvent::KEY_F3);
	mKeyCodeHT->insert(VK_F4, KeyEvent::KEY_F4);
	mKeyCodeHT->insert(VK_F5, KeyEvent::KEY_F5);
	mKeyCodeHT->insert(VK_F6, KeyEvent::KEY_F6);
	mKeyCodeHT->insert(VK_F7, KeyEvent::KEY_F7);
	mKeyCodeHT->insert(VK_F8, KeyEvent::KEY_F8);
	mKeyCodeHT->insert(VK_F9, KeyEvent::KEY_F9);
	mKeyCodeHT->insert(VK_F10, KeyEvent::KEY_F10);
	mKeyCodeHT->insert(VK_F11, KeyEvent::KEY_F11);
	mKeyCodeHT->insert(VK_F12, KeyEvent::KEY_F12);
	mKeyCodeHT->insert(VK_NUMLOCK, KeyEvent::KEY_NUMLOCK);
}

void WindowManagerWin32::initMouseHashTables(void)
{
	m_msg_to_mbutton     = new HashTable<UINT, MouseEvent::MouseButton>(15);
	m_msg_to_mevent_type = new HashTable<UINT, MouseEvent::MouseEventType>();
	m_msg_to_click_count = new HashTable<UINT, s32>;

	m_msg_to_mbutton->insert(WM_LBUTTONDBLCLK, MouseEvent::MBUTTON1);
	m_msg_to_mbutton->insert(WM_LBUTTONDOWN,   MouseEvent::MBUTTON1);
	m_msg_to_mbutton->insert(WM_LBUTTONUP,     MouseEvent::MBUTTON1);
	m_msg_to_mbutton->insert(WM_MBUTTONDBLCLK, MouseEvent::MBUTTON3);
	m_msg_to_mbutton->insert(WM_MBUTTONDOWN,   MouseEvent::MBUTTON3);
	m_msg_to_mbutton->insert(WM_MBUTTONUP,     MouseEvent::MBUTTON3);
	m_msg_to_mbutton->insert(WM_RBUTTONDBLCLK, MouseEvent::MBUTTON2);
	m_msg_to_mbutton->insert(WM_RBUTTONDOWN,   MouseEvent::MBUTTON2);
	m_msg_to_mbutton->insert(WM_RBUTTONUP,     MouseEvent::MBUTTON2);

	m_msg_to_mevent_type->insert(WM_LBUTTONDBLCLK, MouseEvent::MOUSE_CLICKED);
	m_msg_to_mevent_type->insert(WM_MBUTTONDBLCLK, MouseEvent::MOUSE_CLICKED);
	m_msg_to_mevent_type->insert(WM_RBUTTONDBLCLK, MouseEvent::MOUSE_CLICKED);
	m_msg_to_mevent_type->insert(WM_LBUTTONDOWN,   MouseEvent::MOUSE_PRESSED);
	m_msg_to_mevent_type->insert(WM_MBUTTONDOWN,   MouseEvent::MOUSE_PRESSED);
	m_msg_to_mevent_type->insert(WM_RBUTTONDOWN,   MouseEvent::MOUSE_PRESSED);
	m_msg_to_mevent_type->insert(WM_LBUTTONUP,     MouseEvent::MOUSE_RELEASED);
	m_msg_to_mevent_type->insert(WM_MBUTTONUP,     MouseEvent::MOUSE_RELEASED);
	m_msg_to_mevent_type->insert(WM_RBUTTONUP,     MouseEvent::MOUSE_RELEASED);
	m_msg_to_mevent_type->insert(WM_MOUSEACTIVATE, MouseEvent::MOUSE_ENTERED);
	m_msg_to_mevent_type->insert(WM_MOUSELEAVE,    MouseEvent::MOUSE_EXITED);
#if defined(_FIRE_ENGINE_GENERATE_MOUSE_MOVED_EVENTS_)
	m_msg_to_mevent_type->insert(WM_MOUSEMOVE,     MouseEvent::MOUSE_DRAGGED);
#endif

	m_msg_to_click_count->insert(WM_LBUTTONDBLCLK, 2);
	m_msg_to_click_count->insert(WM_MBUTTONDBLCLK, 2);
	m_msg_to_click_count->insert(WM_RBUTTONDBLCLK, 2);
	m_msg_to_click_count->insert(WM_LBUTTONDOWN, 0);
	m_msg_to_click_count->insert(WM_MBUTTONDOWN, 0);
	m_msg_to_click_count->insert(WM_RBUTTONDOWN, 0);
	m_msg_to_click_count->insert(WM_LBUTTONUP, 1);
	m_msg_to_click_count->insert(WM_MBUTTONUP, 1);
	m_msg_to_click_count->insert(WM_RBUTTONUP, 1);
}

WindowManagerWin32::CursorWin32::CursorWin32(WindowManagerWin32 * owner)
	: mOwner(owner)
{
}

WindowManagerWin32::CursorWin32::~CursorWin32()
{
}

void WindowManagerWin32::CursorWin32::setRelativePosition(const vector2f& newpos)
{
	POINT pos;
	pos.x = (s32)(newpos.getX()*mOwner->getWindowSize().getWidth());
	pos.y = (s32)(newpos.getY()*mOwner->getWindowSize().getHeight());
	ClientToScreen(mOwner->mHwnd, &pos);
	SetCursorPos(pos.x, pos.y);
}

vector2f WindowManagerWin32::CursorWin32::getRelativePosition() const
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(mOwner->mHwnd, &pos);
	return vector2f(((f32)pos.x)/mOwner->getWindowSize().getWidth(),
		((f32)pos.y)/mOwner->getWindowSize().getHeight());
}

void WindowManagerWin32::CursorWin32::setCursorVisible(bool vis)
{
	ShowCursor(vis ? TRUE : FALSE);
}

} // namespace fire_engine
