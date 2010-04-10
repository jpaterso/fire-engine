/**
* FILE:    ZipFileReader.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: ZipFileReader.h 112 2007-09-25 23:37:26Z jpaterso $
**/

#ifndef ZIPFILEREADER_H_INCLUDED
#define ZIPFILEREADER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "array.h"
#include "IFileProvider.h"

namespace fire_engine
{

class string;

namespace io
{

class IFile;

/** A simple class for loading and exploring .ZIP files. For now, only files
 that are "stored" (ie. not compressed) are supported.
 For access to some file in the ZIP archive, see the comments on the openFile()
 method. */
class _FIRE_ENGINE_API_ ZipFileReader : public IFileProvider
{
public:
#pragma pack(push, 1)
	typedef struct
	{
		s32 CRC32;
		s32 CompressedSize;
		s32 UncompressedSize;
	} zip_data_descriptor_t;

	typedef struct
	{
		s32 Signature;
		s16 VersionRequired;
		s16 Flags;
		s16 CompressionMethod;
		s16 LastModTime;
		s16 LastModDate;
		zip_data_descriptor_t Descriptor;
		s16 FilenameLen;
		s16 ExtraFieldLen;
	} zip_local_file_header_t;
#pragma pack(pop)

public:
	struct ZipFileEntry
	{
		string Name;
		string FullName;
		s32    Offset;
		zip_local_file_header_t ZipHeader;
	};

public:
	ZipFileReader();

	ZipFileReader(const string& filename);

	virtual ~ZipFileReader();

	/** Attempts to find a file within the archive, and returns it.
	 \param filename    The name of the file to search for
	 \param ignore_case Set this to true if the name is case-insensitive.
	 \param ignore_dirs Set this to true if the directory should be ignored when searching, ie.
	                    that the file can be anywhere in the archive. */
	IFile * getFile(const string& filename, bool ignore_case = true, bool ignore_dirs = false);

	/** Returns a pointer to file at position index in the list of files. */
	IFile * getFile(s32 index);

	/** Returns the index of the file if it is in the archive, or -1 if it's not there. 
	 \param filename    The name of the file to look for.
	 \param ignore_case Whether to perform a case-insensitive search for the file or not.
	 \param ignore_dirs Whether to ignore directory information when searching for the file
	                    or not. Note that if this is true, filename can still contain
						directory information which will be ignored during the search.
	 \return            The index of the file in the archive, or -1 if it's not there. */
	s32 indexOf(const string& filename, bool ignore_case = false, bool ignore_dirs = false);

	/** Returns true if the archive contains the file specified by filename, false otherwise. */
	bool contains(const string& filename, bool ignore_case = false, bool ignore_dirs = false);

	/** Returns an Array containing information about all the files contained within
	 the .ZIP archive. */
	const array<ZipFileEntry> * getEntries() const;

	virtual IFile * openFile(const string& filename, bool ignoreCase, u32 flags);

	virtual bool contains(const string& filename, bool ignoreCase);

	virtual bool isReady() const;

private:
	IFile *             ZipArchive;
	array<ZipFileEntry> FileEntries;

	/** Look for the next local file header, read it, and insert it into the list of files.
	 \return true if a local file header was found, false otherwise. */
	bool scanLocalFileHeader();
};

}
}


#endif
