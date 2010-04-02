/**
 * FILE:    IFileProvider.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: An interface for something that can be used to open files.
**/

#ifndef IFILEPROVIDER_H_INCLUDED
#define IFILEPROVIDER_H_INCLUDED


#include "CompileConfig.h"
#include "Object.h"
#include "string.h"

namespace fire_engine
{
namespace io
{
class IFile;

/** An interface for something that can provide files when given a filename 
 (which will include the path). 
 An example of this is a directory on the user's machine.*/
class _FIRE_ENGINE_API_ IFileProvider : public Object
{
public:

	/** Destructor - does nothing. */
	virtual ~IFileProvider() {}

	/** Attempts to open a file.
	 \param filename The full path of the file. 
	 \param ignoreCase Whether to ignore case when searching for the file.
	 \return A pointer to the file if it exists, nullptr otherwise. */
	virtual IFile * openFile(const string& filename, bool ignoreCase, u32 flags) = 0;

	/** Checks whether a file is available within the IFileProvider.
	 \param filename The full path of the file. 
	 \param ignoreCase Whether to ignore case when searching for the file.
	 \return Whether the IFileProvider contains access to the file. */
	virtual bool contains(const string& filename, bool ignoreCase) = 0;

	/** Checks whether the file provider is 'ready' to provide files.
	 This usually involves checking whether a directory was correctly opened,
	 or for example if a Zip Archive was properly opened.
	 \return Whether the file provider is 'ready' to provide files. */
	virtual bool isReady() const = 0;
};

}
}

#endif // IFILEPROVIDER_H_INCLUDED
