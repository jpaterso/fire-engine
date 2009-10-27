/**
 * FILE:    InputEvent.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: InputEvent.cpp 15 2007-06-23 04:38:01Z jpaterso $
 * PURPOSE: Defining member functions declared in InputEvent.h
**/

#include "InputEvent.h"
#include "string.h"

namespace fire_engine
{

InputEvent::InputEvent(void)
	: m_mask(0),
	  m_is_consumed(false)
{
}

InputEvent::InputEvent(u8 mask)
	: m_mask(mask),
	  m_is_consumed(false)
{
}

InputEvent::~InputEvent(void)
{
}

u8 InputEvent::getMask(void) const
{
	return m_mask;
}

void InputEvent::setMask(u8 mask)
{
	m_mask = mask;
}

bool InputEvent::isMaskEnabled(u8 mask) const
{
	return (m_mask & mask) != 0;
}

void InputEvent::consume(void)
{
	m_is_consumed = true;
}

bool InputEvent::isConsumed(void) const
{
	return m_is_consumed;
}

string InputEvent::toString(void) const
{
	return string("InputEvent:") + getMasks();
}

string InputEvent::getMasks(void) const
{
	string masks;
	if (m_mask == 0x00)
		masks += " NO MASKS";
	else
	{
		if (m_mask & CTRL_MASK)
			masks += " CTRL_MASK";
		if (m_mask & ALT_MASK)
			masks += " ALT_MASK";
		if (m_mask & SHIFT_MASK)
			masks += " SHIFT_MASK";
		if (m_mask & CAPS_MASK)
			masks += " CAPS_MASK";
		if (m_mask & BUTTON1_MASK)
			masks += " BUTTON1_MASK";
		if (m_mask & BUTTON2_MASK)
			masks += " BUTTON2_MASK";
		if (m_mask & BUTTON3_MASK)
			masks += " BUTTON3_MASK";
	}
	return masks;
}

} // namespace fire_engine

