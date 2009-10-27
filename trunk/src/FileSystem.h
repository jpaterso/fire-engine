/**
 * FILE:    FileSystem.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: FileSystem.h 119 2007-12-03 02:12:08Z jpaterso $
**/

#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Array.h"
#include "string.h"
#include "List.h"
#include "File.h"
#include <string.h>
#include <stdio.h>

namespace fire_engine
{
namespace io
{

class ZipFileReader;
class IFile;

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
class _FIRE_ENGINE_API_ FileSystem
{
public:
	/** Creates a FileSystem if it does not already exist, and return it. */
	static FileSystem * Create();

	/** Returns the singleton instance of the FileSystem. */
	static FileSystem * Get();

	/** Destructor. */
	~FileSystem();

	/** Returns a list of all the files in the current directory. */
	Array<FileInfo> * listFiles();

	/** Searches the current file system for a given file. If the file is found
	 within the virtual file system, an IFile pointer to that file is returned. If
	 it is not found, then an attempt is made to open the file on the OS's file
	 system.
	 The default search parameters for the file are a case-sensitive search that
	 preserves directory information. If the file is found, it is open for reading
	 in binary mode.
	 \param filename    The name of the file to look for.
	 \return            An IFile pointer to either a file found in the virtual file system
	                    if it exists, or an IFile pointing to a file on the OS's file system. */
	inline IFile * openReadFile(const string& filename)
	{
		return openReadFile(filename, false, false, EFOF_READ|EFOF_BINARY);
	}

	/** Searches the current file system for a given file. If the file is found
	 within the virtual file system, an IFile pointer to that file is returned. If
	 it is not found, then an attempt is made to open the file on the OS's file
	 system.
	 \param filename    The name of the file to look for.
	 \param ignore_case Whether to ignore the case when looking for the file.
	 \param ignore_dirs Whether to ignore directories when looking for the file.
	 \param flags       The flags to open the file with. Ignored if the file is contained
	                    within an archive.
	 \return            An IFile pointer to either a file found in the virtual file system
	                    if it exists, or an IFile pointing to a file on the OS's file system. */
	IFile * openReadFile(const string& filename, bool ignore_case = false,
		bool ignore_dirs = false, u32 flags = EFOF_READ | EFOF_BINARY);

    /** Checks whether a file exists within the filesystem.
     \param filename The name of the file to look for.
     \return true if the file exists somewhere in the filesystem, false otherwise. */
    bool exists(const string& filename) const;

	/** Attempts to change the working directory, and returns whether it was
	 successful or not. */
	bool changeDirectory(const string& newdir);

	/** Adds a .ZIP archive to the file system.
	 \return true if the ZIP file was correctly loaded, false otherwise. */
	bool addZipArchive(const string& filename);

private:
	static FileSystem * mInstance;
	List<ZipFileReader*> * mZipFileReaders;

	/** Constructor - made private to ensure only a singleton instance of the class is
	 ever created. */
	FileSystem();
};

} // namespace sys
} // namespace fire_engine

#endif // FILESYSTEM_H_INCLUDED
