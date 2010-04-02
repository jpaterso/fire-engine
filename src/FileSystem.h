/**
 * FILE:    FileSystem.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: FileSystem.h 119 2007-12-03 02:12:08Z jpaterso $
**/

#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include "Array.h"
#include "string.h"
#include "List.h"
#include "IFile.h"
#include "IFileProvider.h"
#include <string.h>
#include <stdio.h>

namespace fire_engine
{
namespace io
{

class ZipFileReader;

/** A simple structure for holding information about a given file. */
struct _FIRE_ENGINE_API_ FileInfo
{
	/** Default constructor. */
	FileInfo()
	{
	}

	/** Construct a FileInfo structure from all its components. */
	FileInfo(const string& fullname, const string& path, s32 size, bool is_dir)
		: FullName(fullname), Path(path), Size(size), IsDirectory(is_dir)
	{
	}

	string FullName;
	string Path;
	s32    Size;
	bool   IsDirectory;
};

/** A virtual file system, where file archives can be added, providing easy access
 to files. */
class _FIRE_ENGINE_API_ FileSystem : public Object
{
public:
	/** Creates a FileSystem if it does not already exist, and return it. */
	static FileSystem * Create();

	/** Returns the singleton instance of the FileSystem. */
	static FileSystem * Get();

	/** Destructor. */
	virtual ~FileSystem();

	/** Searches the current file system for a given file. If the file is found
	 within the virtual file system, an IFile pointer to that file is returned.
	 This method takes in an extra argument which is a 'prefered' file provider which is used
	 first when looking for the file. Useful when attempting to locate a file within a particular
	 archive or directory, for example when loading textures associated with a model within a particular
	 .MD3 archive.
	 \param filename    The name of the file to look for.
	 \param ignore_case Whether to ignore the case when looking for the file.
	 \param flags       The flags to open the file with. Ignored if the file is contained
	                    within an archive.
	 \param preferedFileProvider This file provider will be used first when looking for the file.
	 \return            An IFile pointer to either a file found in the virtual file system
	                    if it exists, or an IFile pointing to a file on the OS's file system. */
	IFile * openReadFile(const string& filename, 
		bool ignoreCase = false, 
		u32 flags = EFOF_READ|EFOF_BINARY, 
		IFileProvider * preferedFileProvider = nullptr);

    /** Checks whether a file exists within the file system.
     \param filename The name of the file to look for.
     \return true if the file exists somewhere in the file system, false otherwise. */
    bool exists(const string& filename) const;

	/** Adds a .ZIP archive to the file system.
	 \return A pointer to the IFileProvider created for the .ZIP file. */
	IFileProvider * addArchive(const string& filename);

	/** Adds a directory to the file system.
	 \return A pointer to the IFileProvider created for the directory. */
	IFileProvider * addDirectory(const string& directoryName);

private:
	static FileSystem * Instance;
	List<IFileProvider*> FileProviders;

	/** Constructor - made private to ensure only a singleton instance of the class is
	 ever created. */
	FileSystem();
};

} // namespace sys
} // namespace fire_engine

#endif // FILESYSTEM_H_INCLUDED
