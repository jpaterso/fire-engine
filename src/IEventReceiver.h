/**
 * FILE:    IEventReceiver.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IEventReceiver.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: The interface for an EventReceiver class, that handles user input
**/

#ifndef IEVENTRECEIVER_H_INCLUDED
#define IEVENTRECEIVER_H_INCLUDED

#include "InputEvent.h"
#include "CompileConfig.h"

namespace fire_engine
{

class KeyEvent;
class MouseEvent;

class _FIRE_ENGINE_API_ IEventReceiver
{
	public:
		virtual ~IEventReceiver(void)
		{
		}

		//! Callback function for a KeyEvent
		virtual void onKeyEvent(KeyEvent& kevent) = 0;

		//! Callback function for a MouseEvent
		virtual void onMouseEvent(MouseEvent& mevent) = 0;
};

} // namespace fire_engine

#endif // IEVENTRECEIVER_H_INCLUDED
