/**
 * FILE:    File.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: File.cpp 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: Implementation of the file class.
**/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#include "File.h"
#include "string.h"
#include <sys/types.h>
#include <io.h>

namespace fire_engine
{
namespace io
{


File::File(void)
	: mFD(-1), mFilename(""), mSize(0)
{
	mError = false;
}

File::File(const string& filename, u32 flags)
	: mFilename(filename)
{
	// Open the file, and if created, allow read/write access
	mFD = _open(filename.c_str(), flags, _S_IREAD | _S_IWRITE);
	mError = mFD < 0;
	if (!mError)
	{
		mSize = _lseek(mFD, 0L, SEEK_END);
		_lseek(mFD, 0L, SEEK_SET);
	}
}

File::~File(void)
{
	this->close();
}

bool File::isOpen(void) const
{
	return mFD > 0;
}

bool File::read(void * out, s32 size)
{
	if (mFD < 0)
		return false;
	if (_read(mFD, out, size) != size)
	{
		mError = true;
		return false;
	}
	return true;
}

bool File::write(const void * data, s32 size)
{
	if (mFD < 0)
		return false;
	if (_write(mFD, data, size) != size)
	{
		mError = true;
		return false;
	}
	return true;
}

bool File::seek(EFILE_SEEK_POSITION from, s32 offset)
{
	if (mFD < 0)
		return false;
	if (_lseek(mFD, offset, from) < 0)
	{
		mError = true;
		return false;
	}
	return true;
}

s32 File::getCurrentPosition() const
{
	return _tell(mFD);
}

bool File::remove(void)
{
	if (mFD > 0)
	{
		_close(mFD);
		mFD = -1;
	}
	return _unlink(mFilename.c_str()) == 0;
}

bool File::close(void)
{
	if (mFD < 0)
		return false;
	s32 fd = mFD;
	mFD = -1;
	return _close(fd) >= 0;
}

string File::toString() const
{
	return string("File[ \"") + mFilename + "\" size = " + static_cast<f32>(mSize)/1024 + "kb ]";
}

}
}
