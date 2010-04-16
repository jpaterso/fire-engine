/**
 * FILE:    IWriter.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IWriter.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for a writer: something that takes a certain type of
 *          object, and writes it to disk.
**/

#ifndef IWRITER_H_INCLUDED
#define IWRITER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"

namespace fire_engine
{

/** An interface for classes that have the ability to write some sort of Object
 into a file. This is used by the MediaManager to ease writing of files to disk. */
template <class Obj>
class _FIRE_ENGINE_API_ IWriter
{
public:
	/** Destructor. */
	virtual ~IWriter(void)
	{
	}

	//! Write an object to disk
	virtual bool write(const String& filename, const Obj * object) const = 0;
};

}

#endif
