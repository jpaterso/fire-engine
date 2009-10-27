/**
 * FILE:    FileSystem.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: FileSystem.cpp 116 2007-10-05 13:22:45Z jpaterso $
**/

#include "FileSystem.h"
#include "ZipFileReader.h"

#ifdef _FIRE_ENGINE_WIN32_
#	include <io.h>
#	include <direct.h>
#endif

#include "Logger.h"

namespace fire_engine
{
namespace io
{

FileSystem * FileSystem::mInstance = 0;

FileSystem::FileSystem()
{
	mZipFileReaders = new List<ZipFileReader*>();
}

FileSystem * FileSystem::Create()
{
	if (mInstance == nullptr)
	{
		mInstance = new FileSystem();
	}
	return mInstance;
}

FileSystem * FileSystem::Get()
{
	return mInstance;
}

FileSystem::~FileSystem()
{
	mInstance = 0;
	for (List<ZipFileReader*>::iterator it = mZipFileReaders->begin(); it != mZipFileReaders->end(); it++)
	{
		delete *it;
	}
	delete mZipFileReaders;
}

Array<FileInfo> * FileSystem::listFiles()
{
	Array<FileInfo> * files;
#ifdef _FIRE_ENGINE_WIN32_
	files = new Array<FileInfo>();
	struct _finddata_t tmp_file;
	char wd[_MAX_PATH+1];
	long handle;

	_getcwd(wd, _MAX_PATH+1);
	if ((handle = _findfirst("*", &tmp_file)) > 0)
	{
		do
		{
			files->push_back(FileInfo(tmp_file.name, wd, tmp_file.size, (tmp_file.attrib&_A_SUBDIR) != 0));
		} while (_findnext(handle, &tmp_file) == 0);
		_findclose(handle);
	}
#else
	//TODO: Implement for non-Win32 systems
	files = 0;
#endif
	return files;
}

IFile * FileSystem::openReadFile(const string& filename, bool ignore_case, bool ignore_dirs, u32 flags)
{
	s32 index;
	for (List<ZipFileReader*>::iterator it = mZipFileReaders->begin(); it != mZipFileReaders->end(); it++)
	{
		if ((index = it->indexOf(filename, ignore_case, ignore_dirs)) != -1)
		{
			return it->getFile(index);
		}
	}
	IFile * ret = new File(filename, flags);
	if (!ret->isOpen())
	{
		delete ret;
		return nullptr;
	}
	return ret;
}

bool FileSystem::exists(const string& filename) const
{
	bool fileExists = false;
	for (List<ZipFileReader*>::iterator it = mZipFileReaders->begin(); it != mZipFileReaders->end(); it++)
	{
		if (it->indexOf(filename, false, false) > 0)
		{
			fileExists = true;
			break;
		}
	}
	if (!fileExists)
	{
		IFile * ret = new File(filename, EFOF_READ);
		if (!ret->fail())
		{
			fileExists = true;
		}
		delete ret;
	}
    return fileExists;
}

bool FileSystem::changeDirectory(const string& newdir)
{
#ifdef _FIRE_ENGINE_WIN32_
	return _chdir(newdir.c_str()) == 0;
#else
	return false;
#endif
}

bool FileSystem::addZipArchive(const string& filename)
{
	ZipFileReader * zip_reader = new ZipFileReader(filename);
	if (!zip_reader->isOpen())
	{
	    Logger::Get()->log(ES_HIGH, "FileSystem",
	                       "Could not add %s to the zip archive list\n", filename.c_str());
		delete zip_reader;
		return false;
	}
	mZipFileReaders->push_back(zip_reader);
	return true;
}

} // namespace io
} // namespace fire_engine
