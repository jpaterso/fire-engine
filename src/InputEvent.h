/**
 * FILE:    InputEvent.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: InputEvent.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: The base class for all input events.
 *          This will usually be extended by a MouseEvent or a KeyEvent, but contains information
 *          relative to both classes.
**/

#ifndef INPUTEVENT_H_INCLUDED
#define INPUTEVENT_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

class String;

class _FIRE_ENGINE_API_ InputEvent
{
	public:
		//! The valid masks for a basic InputEvent
		enum Mask
		{
			CTRL_MASK    = 0x01,
			ALT_MASK     = 0x02,
			SHIFT_MASK   = 0x04,
			BUTTON1_MASK = 0x08,
			BUTTON2_MASK = 0x10,
			BUTTON3_MASK = 0x20,
			CAPS_MASK    = 0x40
		};

	public:
		//! Default ctor
		InputEvent(void);

		//! Ctor with a given mask
		InputEvent(u8 mask);

		//! Dtor
		virtual ~InputEvent(void);

		//! Get the mask for the event
		u8 getMask(void) const;

		//! Switch on a certain mask
		void setMask(u8 mask);

		//! Check whether a certain mask is on
		bool isMaskEnabled(u8 mask) const;

		//! Consume the event
		void consume(void);

		//! Check whether the event has been consumed yet
		bool isConsumed(void) const;

		virtual String toString(void) const;

	private:
		u8  m_mask;
		bool m_is_consumed;

	protected:
		String getMasks(void) const;
};

} // namespace fire_engine

#endif // INPUTEVENT_H_INCLUDED
