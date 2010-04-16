/**
 * FILE:    KeyEvent.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: KeyEvent.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A KeyEvent represents input from the keyboard
**/

#ifndef KEYEVENT_H_INCLUDED
#define KEYEVENT_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "InputEvent.h"

namespace fire_engine
{

class String;

class _FIRE_ENGINE_API_ KeyEvent : public InputEvent
{
	public:
		//! The Keys that a user can press
		enum EKEY_CODE
		{
			KEY_0 = 0x01,     // The 0 key
			KEY_1,            // The 1 key
			KEY_2,            // The 2 key
			KEY_3,            // The 3 key
			KEY_4,            // The 4 key
			KEY_5,            // The 5 key
			KEY_6,            // The 6 key
			KEY_7,            // The 7 key
			KEY_8,            // The 8 key
			KEY_9,            // The 9 key
			KEY_A,            // The A key
			KEY_B,            // The B key
			KEY_C,            // The C key
			KEY_D,            // The D key
			KEY_E,            // The E key
			KEY_F,            // The F key
			KEY_G,            // The G key
			KEY_H,            // The H key
			KEY_I,            // The I key
			KEY_J,            // The J key
			KEY_K,            // The K key
			KEY_L,            // The L key
			KEY_M,            // The M key
			KEY_N,            // The N key
			KEY_O,            // The O key
			KEY_P,            // The P key
			KEY_Q,            // The Q key
			KEY_R,            // The R key
			KEY_S,            // The S key
			KEY_T,            // The T key
			KEY_U,            // The U key
			KEY_V,            // The V key
			KEY_W,            // The W key
			KEY_X,            // The X key
			KEY_Y,            // The Y key
			KEY_Z,            // The Z key
			KEY_BACKSPACE,    // The Backspace key
			KEY_TAB,          // The Tabulation key
			KEY_RETURN,       // The Return key
			KEY_SHIFT,        // The Shift key (either one)
			KEY_CONTROL,      // The Control key (either one)
			KEY_ALT,          // The Alt key (either one)
			KEY_PAUSE,        // The Pause key
			KEY_CAPS,         // The CapsLock key
			KEY_ESCAPE,       // The Escape key
			KEY_SPACE,        // The Space bar
			KEY_PGUP,         // The PgUp key
			KEY_PGDOWN,       // The PgDown key
			KEY_END,          // The End key
			KEY_HOME,         // The Home key
			KEY_LEFT,         // The Left key
			KEY_UP,           // The Up key
			KEY_RIGHT,        // The Right key
			KEY_DOWN,         // The Down key
			KEY_SELECT,       // The Select key
			KEY_INSERT,       // The Ins key
			KEY_DELETE,       // The Delete key
			KEY_ADD,          // The + key on the numpad
			KEY_MULTIPLY,     // The * key on the numpad
			KEY_SUBTRACT,     // The - key on the numpad
			KEY_DIVIDE,       // The / key on the numpad
			KEY_DECIMAL,      // '.' on the numpad
			KEY_F1,           // The F1 key
			KEY_F2,           // The F key
			KEY_F3,           // The F3 key
			KEY_F4,           // The F4 key
			KEY_F5,           // The F5 key
			KEY_F6,           // The F6 key
			KEY_F7,           // The F7 key
			KEY_F8,           // The F8 key
			KEY_F9,           // The F9 key
			KEY_F10,          // The F10 key
			KEY_F11,          // The F11 key
			KEY_F12,          // The F12 key
			KEY_NUMLOCK,      // The NumLock key
			KEY_UNKNOWN       // Unknown key (oops)
		};

	public:
		KeyEvent(void);
		KeyEvent(u8 mask, EKEY_CODE key);
		virtual ~KeyEvent(void);

		void setKey(EKEY_CODE key);
		EKEY_CODE getKey(void) const;

		virtual String toString(void) const;

	private:
		EKEY_CODE m_key_pressed; //! The Virtual key code of the pressed key
		c8 m_corr_char;   //! The corresponding ASCII character, 0 if none

		void setChar(void);

		static void initCharTable(void);
};

}

#endif // KEYEVENT_H_INCLUDED
