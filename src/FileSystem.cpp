/**
 * FILE:    FileSystem.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: FileSystem.cpp 116 2007-10-05 13:22:45Z jpaterso $
**/

#include "FileSystem.h"
#include "FileUtils.h"
#include "ZipFileReader.h"
#include "DirectoryFileProvider.h"
#include "File.h"

#ifdef _FIRE_ENGINE_WIN32_
#	include <io.h>
#	include <direct.h>
#endif

#include "Logger.h"

namespace fire_engine
{
namespace io
{

FileSystem * FileSystem::Instance = 0;

FileSystem::FileSystem()
{
}

FileSystem * FileSystem::Create()
{
	if (Instance == nullptr)
	{
		Instance = new FileSystem();
		// Add current directory for loading files.
		Instance->addDirectory(".");
	}
	return Instance;
}

FileSystem * FileSystem::Get()
{
	return Instance;
}

FileSystem::~FileSystem()
{
	Instance = 0;
	for (List<IFileProvider*>::iterator it = FileProviders.begin(); it != FileProviders.end(); it++)
	{
		it->drop();
	}
}

IFile * FileSystem::openReadFile(const String& filename, bool ignoreCase, u32 flags, IFileProvider * preferedFileProvider)
{
	String filenamePathFixed = filename;
	FileUtils::ConvertPath(filenamePathFixed);
	if (preferedFileProvider != nullptr)
	{
		if (preferedFileProvider->contains(filenamePathFixed, ignoreCase))
		{
			return preferedFileProvider->openFile(filenamePathFixed, ignoreCase, flags);
		}
		Logger::Get()->log(ES_DEBUG, "FileSystem", 
			"Warning: Could not load %s in specified file provider", filenamePathFixed.c_str());
	}
	for (List<IFileProvider*>::iterator it = FileProviders.begin(); it != FileProviders.end(); it++)
	{
		if (it->contains(filenamePathFixed, ignoreCase))
		{
			return it->openFile(filenamePathFixed, ignoreCase, flags);
		}
	}
	return nullptr;
}

bool FileSystem::exists(const String& filename) const
{
	String filenamePathFixed = filename;
	FileUtils::ConvertPath(filenamePathFixed);
	bool fileExists = false;
	for (List<IFileProvider*>::iterator it = FileProviders.begin(); it != FileProviders.end(); it++)
	{
		if (it->contains(filenamePathFixed, false))
		{
			fileExists = true;
			break;
		}
	}
	return fileExists;
}

IFileProvider * FileSystem::addArchive(const String& filename)
{
	IFileProvider * zip_reader = new ZipFileReader(filename);
	if (!zip_reader->isReady())
	{
	    Logger::Get()->log(ES_HIGH, "FileSystem",
	                       "Could not add %s to the zip archive list\n", filename.c_str());
		delete zip_reader;
		return nullptr;
	}
	FileProviders.push_back(zip_reader);
	return zip_reader;
}

IFileProvider * FileSystem::addDirectory(const String& directoryName)
{
	IFileProvider * directoryReader = new DirectoryFileProvider(directoryName);
	FileProviders.push_back(directoryReader);
	return directoryReader;
}

} // namespace io
} // namespace fire_engine
