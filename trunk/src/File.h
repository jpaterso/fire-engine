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
#include "string.h"
#include "IFile.h"
#include <sys/stat.h>
#include <fcntl.h>

namespace fire_engine
{
namespace io
{

//! Flags to open the file with
enum EFILE_OPEN_FLAGS
{
	EFOF_READ     = _O_RDONLY,  // Open the file for reading
	EFOF_WRITE    = _O_WRONLY,  // Open the file for writing
	EFOF_RW       = _O_RDWR,    // Open the file for both reading and writing
	EFOF_CREATE   = _O_CREAT,   // Create the file if it does not exist
	EFOF_APPEND   = _O_APPEND,  // If the file exists, append to it instead of truncating it
	EFOF_TRUNCATE = _O_TRUNC,   // If the file exists, truncate it
	EFOF_BINARY   = _O_BINARY   // Open the file in binary
};

/** A simple class for reading and writing to files. */
class _FIRE_ENGINE_API_ File : public IFile
{
public:
	//! Default constructor
	File();

	//! Open a file with a given set of flags
	File(const string& filename, u32 flags);

	//! Destructor - closes the file if it is open
	virtual ~File();

	virtual bool isOpen(void) const;

	virtual bool read(void * out, s32 size);

	virtual bool write(const void * data, s32 size);

	virtual bool seek(EFILE_SEEK_POSITION from, s32 offset);

	virtual s32 getCurrentPosition() const;

	virtual bool remove(void);

	virtual bool close(void);

	virtual string toString(void) const;

private:
	s32    mFD;        // The file descriptor for the file
	string mFilename;  // The name of the file
	s32    mSize;      // The size in bytes of the file
};

} // namespace io
} // namespace fire_engine

#endif // FILE_H_INCLUDED
