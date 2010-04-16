/**
 * FILE:    File.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: File.h 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: A class for creating, reading and writing to files.
**/

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"
#include "IFile.h"

namespace fire_engine
{
namespace io
{

/** A simple class for reading and writing to files. */
class _FIRE_ENGINE_API_ File : public IFile
{
public:
	//! Default constructor
	File();

	//! Open a file with a given set of flags
	File(const String& filename, u32 flags);

	//! Destructor - closes the file if it is open
	virtual ~File();

	virtual bool isOpen(void) const;

	virtual bool read(void * out, s32 size);

	virtual bool write(const void * data, s32 size);

	virtual bool seek(EFILE_SEEK_POSITION from, s32 offset);

	virtual s32 getCurrentPosition() const;

	virtual bool remove(void);

	virtual bool close(void);

	virtual String toString(void) const;

private:
	s32    mFD;        // The file descriptor for the file
	s32    mSize;      // The size in bytes of the file
};

} // namespace io
} // namespace fire_engine

#endif // FILE_H_INCLUDED
