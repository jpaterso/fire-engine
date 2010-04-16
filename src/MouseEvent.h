/**
 * FILE:    MouseEvent.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MouseEvent.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A MouseEvent represents input from the mouse
**/

#include "Types.h"
#include "InputEvent.h"
#include "dimension2.h"
#include "CompileConfig.h"

#ifndef MOUSEEVENT_H_INCLUDED
#define MOUSEEVENT_H_INCLUDED

namespace fire_engine
{

class String;

/** A MouseEvent describes some input from the mouse. They should automatically
 be generated from the window manager, and sent off to be processed. */
class _FIRE_ENGINE_API_ MouseEvent : public InputEvent
{
	public:
		enum MouseButton
		{
			MBUTTON1 = 0x01,
			MBUTTON2,
			MBUTTON3,
			MNO_BUTTON
		};

		enum MouseEventType
		{
			MOUSE_PRESSED,
			MOUSE_RELEASED,
			MOUSE_CLICKED,
			MOUSE_ENTERED,
			MOUSE_EXITED,
#if defined(_FIRE_ENGINE_GENERATE_MOUSE_MOVED_EVENTS_)
			MOUSE_DRAGGED
#endif
		};
	public:
		MouseEvent(void);
		MouseEvent(u8 mask, MouseEventType type, MouseButton button, s32 click_count,
			const dimension2i& pt);

		virtual ~MouseEvent(void);

		void setType(MouseEventType type);
		MouseEventType getType(void) const;

		void setButton(MouseButton button);
		MouseButton getButton(void) const;

		void setClickCount(s32 click_count);
		s32 getClickCount(void) const;

		void setPoint(const dimension2i& pt);
		dimension2i getPoint(void) const;

		virtual String toString(void) const;

	private:
		MouseEventType m_type;
		MouseButton    m_button;
		s32            m_click_count;
		dimension2i    m_point;
};

}

#endif // MOUSEEVENT_H_INCLUDED
