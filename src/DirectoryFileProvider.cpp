/**
 * FILE:    DirectoryFileProvider.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: Implementation of the DirectoryFileProvider class.
**/

#include "DirectoryFileProvider.h"
#include "stdio.h"
#include "File.h"

#ifdef _FIRE_ENGINE_WIN32_
#	include <io.h>
#	include <direct.h>
#endif

namespace fire_engine
{
namespace io
{

DirectoryFileProvider::DirectoryFileProvider(const String& directory)
	: DirectoryName(directory)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::io::DirectoryFileProvider");
#endif
}

DirectoryFileProvider::~DirectoryFileProvider()
{
}

IFile * DirectoryFileProvider::openFile(const String& filename, bool ignoreCase, u32 flags)
{
	c8 currentDirectory[FILENAME_MAX];
	_getcwd(currentDirectory, sizeof(currentDirectory));

	_chdir(DirectoryName.c_str());

	IFile * file = new File(filename.c_str(), flags);
	if (!file->isOpen())
	{
		delete file;
		file = nullptr;
	}

	_chdir(currentDirectory);

	return file;
}

bool DirectoryFileProvider::contains(const String& filename, bool ignoreCase)
{
	bool fileExists = false;
	c8 currentDirectory[FILENAME_MAX];

	_getcwd(currentDirectory, sizeof(currentDirectory));
	_chdir(DirectoryName.c_str());
	fileExists = _access_s(filename.c_str(), 0) == 0;
	_chdir(currentDirectory);

	return fileExists;
}

bool DirectoryFileProvider::isReady() const
{
	return true;
}

}
}