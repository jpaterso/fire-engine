/**
 * FILE:    MemoryFile.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MemoryFile.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A file that has already been loaded, and is stored in memory.
**/
#ifndef MEMORYFILE_H_INCLUDED
#define MEMORYFILE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "IFile.h"

namespace fire_engine
{
namespace io
{

/** A memory simulates a file that can be read from, but is actually stored in memory.
 This is useful if for example you are decompressing an archive, and have all the data
 stored in memory but would like to treat it as if it was an actual file. */
class _FIRE_ENGINE_API_ MemoryFile : public IFile
{
public:
	/** Default constructor. */
	MemoryFile();

	/** Construct a MemoryFile from all its components. */
	MemoryFile(void * data, s32 size, bool free_when_destroyed = true);

	virtual ~MemoryFile();

	virtual bool isOpen() const;

	virtual bool read(void * data, s32 size);

	virtual bool write(const void * data, s32 size);

	virtual bool seek(EFILE_SEEK_POSITION from, s32 offset);

	virtual s32 getCurrentPosition() const;

	virtual bool remove();

	virtual bool close();

	virtual String toString() const;

private:
	void * mData;
	s32    mDataSize;
	s32    mCurrentOffset;
	bool   mFreeWhenDestroyed;
};

} // namespace io
} // namespace fire_engine

#endif // MEMORYFILE_H_INCLUDED
