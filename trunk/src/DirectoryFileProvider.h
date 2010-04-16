/**
 * FILE:    DirectoryFileProvider.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: A file provider for directories.
**/

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"
#include "IFileProvider.h"

namespace fire_engine
{
namespace io
{

class IFile;

/** A class that provides files from a specified directory. */
class _FIRE_ENGINE_API_ DirectoryFileProvider : public IFileProvider
{
public:
	/** Construct a file provider from a given directory name, which 
	 can be either a full path or a relative path. */
	DirectoryFileProvider(const String& directory);

	virtual ~DirectoryFileProvider();

	virtual IFile * openFile(const String& filename, bool ignoreCase, u32 flags);

	virtual bool contains(const String& filename, bool ignoreCase);

	virtual bool isReady() const;

private:
	String DirectoryName;
};


}
}