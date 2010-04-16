/**
 * FILE:    KeyEvent.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: KeyEvent.cpp 102 2007-09-04 06:14:05Z jpaterso $
 * PURPOSE: Implementation of the KeyEvent class.
**/

#include "KeyEvent.h"
#include "String.h"

namespace fire_engine
{

KeyEvent::KeyEvent(void)
: m_key_pressed(KeyEvent::KEY_UNKNOWN),
	  m_corr_char(0)
{
}

KeyEvent::KeyEvent(u8 mask, EKEY_CODE key)
	: InputEvent(mask), // call non-default ctor of parent
	  m_key_pressed(key),
	  m_corr_char(0)
{
	setChar();
}

KeyEvent::~KeyEvent(void)
{
}

void KeyEvent::setKey(EKEY_CODE key)
{
	m_key_pressed = key;
	setChar();
}

KeyEvent::EKEY_CODE KeyEvent::getKey(void) const
{
	return m_key_pressed;
}

void KeyEvent::setChar(void)
{
	if (m_key_pressed >= KEY_0 && m_key_pressed <= KEY_9)
		m_corr_char = '0' + m_key_pressed - KEY_0;
	else if (m_key_pressed >= KEY_A && m_key_pressed <= KEY_Z)
	{
		if (isMaskEnabled(SHIFT_MASK) || isMaskEnabled(CAPS_MASK))
			m_corr_char = 'A' + m_key_pressed - KEY_A;
		else
			m_corr_char = 'a' + m_key_pressed - KEY_A;
	}
	else
	{
		switch (m_key_pressed)
		{
			case KEY_SPACE:    m_corr_char = ' '; break;
			case KEY_ADD:      m_corr_char = '+'; break;
			case KEY_MULTIPLY: m_corr_char = '*'; break;
			case KEY_DIVIDE:   m_corr_char = '/'; break;
			case KEY_SUBTRACT: m_corr_char = '*'; break;
			default:           m_corr_char = 0;   break;
		}
	}
}

String KeyEvent::toString(void) const
{
	return String("KeyEvent:") + getMasks() + " " + m_corr_char;
}
}
