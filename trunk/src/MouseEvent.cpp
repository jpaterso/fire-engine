/**
 * FILE:    MouseEvent.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MouseEvent.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of the MouseEvent class.
**/

#include "MouseEvent.h"
#include "String.h"

namespace fire_engine
{

MouseEvent::MouseEvent(void)
	: m_button(MNO_BUTTON),
	  m_click_count(0),
	  m_point(0, 0)
{
}

MouseEvent::MouseEvent(u8 mask, MouseEventType type, MouseButton button, s32 click_count,
	const dimension2i& pt)
	: InputEvent(mask),
	  m_type(type),
	  m_button(button),
	  m_click_count(click_count),
	  m_point(pt)
{
}

MouseEvent::~MouseEvent(void)
{
}

void MouseEvent::setType(MouseEventType type)
{
	m_type = type;
}

MouseEvent::MouseEventType MouseEvent::getType(void) const
{
	return m_type;
}

void MouseEvent::setButton(MouseButton button)
{
	m_button = button;
}

MouseEvent::MouseButton MouseEvent::getButton(void) const
{
	return m_button;
}

void MouseEvent::setClickCount(s32 click_count)
{
	m_click_count = click_count;
}

s32 MouseEvent::getClickCount(void) const
{
	return m_click_count;
}

void MouseEvent::setPoint(const dimension2i& point)
{
	m_point = point;
}

dimension2i MouseEvent::getPoint(void) const
{
	return m_point;
}

String MouseEvent::toString(void) const
{
	String button;
	String type;
	switch (m_button)
	{
		case MBUTTON1:   button = "BUTTON 1";   break;
		case MBUTTON2:   button = "BUTTON 2";   break;
		case MBUTTON3:   button = "BUTTON 3";   break;
		case MNO_BUTTON: button = "NO BUTTON"; break;
	}
	switch (m_type)
	{
		case MOUSE_PRESSED:  type = "PRESSED";  break;
		case MOUSE_RELEASED: type = "RELEASED"; break;
		case MOUSE_CLICKED:  type = "CLICKED";  break;
		case MOUSE_ENTERED:  type = "ENTERED";  break;
		case MOUSE_EXITED:   type = "EXITED";   break;
#if defined(_FIRE_ENGINE_GENERATE_MOUSE_MOVED_EVENTS_)
		case MOUSE_DRAGGED:  type = "DRAGGED";  break;
#endif
	}
	return String("MouseEvent:")+getMasks()+" "+button+ " "+type+m_click_count+" CLICKS AT "+
		m_point.toString();
}

}
