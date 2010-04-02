/**
 * FILE:    MemoryFile.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MemoryFile.cpp 97 2007-08-19 03:19:07Z jpaterso $
 * PURPOSE: Implementation of the MemoryFile class.
**/

#include "MemoryFile.h"
#include "string.h"
#include <string.h>

namespace fire_engine
{
namespace io
{

MemoryFile::MemoryFile()
	: mData(0), mDataSize(0), mCurrentOffset(0), mFreeWhenDestroyed(false)
{
	Filename = "";
	ErrorOccured = false;
}

MemoryFile::MemoryFile(void * data, s32 size, bool free_when_destroyed)
	: mData(data), mDataSize(size), mCurrentOffset(0),
	  mFreeWhenDestroyed(free_when_destroyed)
{
	Filename = "";
	ErrorOccured = false;
}

MemoryFile::~MemoryFile()
{
	if (mFreeWhenDestroyed)
		delete [] (u8*)mData;
}

bool MemoryFile::isOpen() const
{
	return mData != 0x00;
}

bool MemoryFile::read(void * data, s32 size)
{
	if ((mCurrentOffset+size) > mDataSize)
	{
		ErrorOccured = true;
		return false;
	}
	const u8 * dataPtr = (u8*)mData;
	memcpy(data, &dataPtr[mCurrentOffset], size);
	mCurrentOffset = mCurrentOffset+size;
	return true;
}

bool MemoryFile::write(const void * data, s32 size)
{
	ErrorOccured = true;
	return false;
}

bool MemoryFile::seek(EFILE_SEEK_POSITION from, s32 size)
{
	switch (from)
	{
	case EFSP_START:
		if (size > mDataSize)
			return false;
		mCurrentOffset = size;
		break;
	case EFSP_CURRENT:
		if ((mCurrentOffset+size) > mDataSize)
			return false;
		mCurrentOffset = mCurrentOffset+size;
		break;
	case EFSP_END:
		if (size > mDataSize)
			return false;
		mCurrentOffset = mDataSize-size;
		break;
	}
	return true;
}

s32 MemoryFile::getCurrentPosition() const
{
	return mCurrentOffset;
}

bool MemoryFile::remove()
{
	ErrorOccured = true;
	return false;
}

bool MemoryFile::close()
{
	if (mFreeWhenDestroyed)
		delete [] (u8*)mData;
	mData = 0x00;
	return true;
}

string MemoryFile::toString() const
{
	return string("MemoryFile[ size = ") + mDataSize + " ]";
}

}
}
