/**
 * FILE:    ILoader.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ILoader.h 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: An interface for a loader: something that loads a file, and returns
 *          a certain type of file.
**/

#ifndef ILOADER_H_INCLUDED
#define ILOADER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"

namespace fire_engine
{

namespace io
{
class IFileProvider;
}

/** An interface for a class that can load some sort of Object from a file.
 This class is used by the MediaManager, to ease the loading of objects from disk. */
template <class Obj>
class _FIRE_ENGINE_API_ ILoader
{
public:
	/** Destructor. */
	virtual ~ILoader()
	{
	}

	/** Load an object from disk, and returns a pointer to the object created.
	 Classes can implement this interface in order to register themselves
	 with the MediaManager class, to provide seamless loading of objects
	 from disk using a single entry point.
	 \param filename A String object containing the name of the file to load.
	 \param fileProvider A file provider to use first. Useful if loading from within some archive.
	 \return A pointer to the object created. */
	virtual Obj * load(const String& filename, io::IFileProvider * fileProvider) const = 0;
};

}

#endif // ILOADER_H_INCLUDED
