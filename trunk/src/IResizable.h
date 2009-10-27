/**
 * FILE:    IResizable.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IResizable.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for something that can be resized.
**/

#ifndef IRESIZABLE_H_INCLUDED
#define IRESIZABLE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"

namespace fire_engine
{

/** A simple interface for something that can be resized. All objects
 extending this class should call the IWindowManager::addResizable() method.
 The onResize() method will then automatically be called when the window
 is resized. */
class _FIRE_ENGINE_API_ IResizable
{
public:
	/** Destructor. */
	virtual ~IResizable()
	{
	}

	/** Callback function used when the window is resized. This will
	 automatically be called.
	 \param newsize The new size of the window. */
	virtual void onResize(const dimension2i& newSize) = 0;

};

}

#endif // IRESIZABLE_H_INCLUDED

