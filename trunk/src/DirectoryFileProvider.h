/**
 * FILE:    DirectoryFileProvider.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: A file provider for directories.
**/

#include "Types.h"
#include "CompileConfig.h"
#include "string.h"
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
	DirectoryFileProvider(const string& directory);

	virtual ~DirectoryFileProvider();

	virtual IFile * openFile(const string& filename, bool ignoreCase, u32 flags);

	virtual bool contains(const string& filename, bool ignoreCase);

	virtual bool isReady() const;

private:
	string DirectoryName;
};


}
}