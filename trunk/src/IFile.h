/**
* FILE:    IFile.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: IFile.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: An interface for a File, something that can be read, or that can
*          be written to.
**/

#ifndef IFILE_H_INCLUDED
#define IFILE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include <stdio.h>

namespace fire_engine
{

class string;

namespace io
{

//! Where to start the seek from
enum EFILE_SEEK_POSITION
{
	EFSP_START   = SEEK_SET,  // From the start of the file
	EFSP_END     = SEEK_END,  // From the end of the file
	EFSP_CURRENT = SEEK_CUR   // From the current position in the file
};

/** An interface for some sort of file: something that can be read or written
 to. */
class _FIRE_ENGINE_API_ IFile
{
public:
	/** Destructor. */
	virtual ~IFile()
	{
	}

	/** Returns whether the file is open or not. */
	virtual bool isOpen() const = 0;

	/** Read a certain number of bytes from the file, into a pre-allocated buffer.
	\param out  The buffer to read into
	\param size The number of bytes to read
	\return     true if the number of bytes read was equal to size, false otherwise */
	virtual bool read(void * out, s32 size) = 0;

	/** Write a certain number of bytes into the file.
	\param data The data to write
	\param size The number of bytes to write
	\return     true if the number of bytes written matches size, false otherwise */
	virtual bool write(const void * data, s32 size) = 0;

	/** Move along in the file.
	\param from   Indicates where to move from in the file.
	\param offset The number of bytes to move from the given position
	\return       true if the seek was successful, false otherwise */
	virtual bool seek(EFILE_SEEK_POSITION from, s32 offset) = 0;

	/** Returns the current seeking position. */
	virtual s32 getCurrentPosition() const = 0;

	/** Remove the file.
	 \return true if the file was correctly removed, false otherwise. */
	virtual bool remove() = 0;

	/** Returns whether an error occurred previously, and clears the error bit. */
	bool fail()
	{
		bool ret = mError;
		mError = false;
		return ret;
	}

	/** Close the file.
	 \return true if the file was closed correctly, false otherwise. */
	virtual bool close() = 0;

	/** Returns a textual representation of the file. */
	virtual string toString() const = 0;

protected:
	bool mError;
};

} // namespace io
} // namespace fire_engine

#endif // IFILE_H_INCLUDED
