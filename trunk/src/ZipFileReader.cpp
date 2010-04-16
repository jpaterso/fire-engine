/**
* FILE:    ZipFileReader.cpp
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: ZipFileReader.cpp 112 2007-09-25 23:37:26Z jpaterso $
**/

#include "ZipFileReader.h"
#include "ByteConverter.h"
#include "MemoryFile.h"
#include "Logger.h"
#include "FileSystem.h"
#include "FileUtils.h"
#include <ctype.h>

#define ZIP_LOCAL_FILE_HEADER_SIG 0x04034b50
#define ZIP_DATA_DESCRIPTOR_FLAG  0x0008

namespace fire_engine
{
namespace io
{

ZipFileReader::ZipFileReader()
	: ZipArchive(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::io::ZipFileReader");
#endif
}

ZipFileReader::ZipFileReader(const String& filename)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::io::ZipFileReader");
#endif
	ZipArchive = FileSystem::Get()->openReadFile(filename, false, io::EFOF_READ|io::EFOF_BINARY);
	if (ZipArchive == nullptr)
	{
        Logger::Get()->log(ES_HIGH, "io::ZipFileReader", "Could not open %s for reading",
                           filename.c_str());
	}
	else
	{
	    while (scanLocalFileHeader())
		{
			;
		}
	}
}

ZipFileReader::~ZipFileReader()
{
	if (ZipArchive != nullptr)
	{
		delete ZipArchive;
	}
}


bool ZipFileReader::isReady() const
{
	return ZipArchive != nullptr;
}

IFile * ZipFileReader::getFile(const String& filename, bool ignore_case, bool ignore_dirs)
{
	return getFile(indexOf(filename, ignore_case, ignore_dirs));
}

IFile * ZipFileReader::openFile(const String& filename, bool ignoreCase, u32 flags)
{
	return getFile(indexOf(filename, ignoreCase, false));
}

bool ZipFileReader::contains(const String& filename, bool ignoreCase)
{
	return indexOf(filename, ignoreCase, false) != -1;
}

IFile * ZipFileReader::getFile(s32 index)
{
	u8 * data;
	IFile * retval;
	if (index < 0 || index >= FileEntries.size())
	{
		return nullptr;
	}

	switch (FileEntries[index].ZipHeader.CompressionMethod)
	{
	case 0x00:
		data = new u8[FileEntries[index].ZipHeader.Descriptor.CompressedSize];
		ZipArchive->seek(EFSP_START, FileEntries[index].Offset);
		ZipArchive->read(data, FileEntries[index].ZipHeader.Descriptor.CompressedSize);
		retval = new MemoryFile(data, FileEntries[index].ZipHeader.Descriptor.CompressedSize, true);
		break;

	default:
		Logger::Get()->log(ES_HIGH, "io::ZipFileReader",
			"Compression method %d not supported in %s", FileEntries[index].ZipHeader.CompressionMethod,
			FileEntries[index].Name.c_str());
		retval = nullptr;
		break;
	}
	return retval;
}

s32 ZipFileReader::indexOf(const String& filename, bool ignore_case, bool ignore_dirs)
{
	s32 index = -1;
	bool (*equals)(const String&, const String&) = String::equals;
	if (ignore_case)
	{
		equals = String::equalsIgnoreCase;
	}
	if (ignore_dirs)
	{
		String filename_dirsIgnored = FileUtils::StripDirectory(filename);
		for (s32 i = 0; i < FileEntries.size(); i++)
		{
			if (equals(filename_dirsIgnored, FileEntries[i].Name))
			{
				index = i;
				break;
			}
		}
	}
	else
	{
		for (s32 i = 0; i < FileEntries.size(); i++)
		{
			if (equals(filename, FileEntries[i].FullName))
			{
				index = i;
				break;
			}
		}
	}
	return index;
}

bool ZipFileReader::contains(const String& filename, bool ignore_case, bool ignore_dirs)
{
	return indexOf(filename, ignore_case, ignore_dirs) != -1;
}

const Array<ZipFileReader::ZipFileEntry> * ZipFileReader::getEntries() const
{
	return &FileEntries;
}

bool ZipFileReader::scanLocalFileHeader()
{
	ZipFileEntry entry;
	ZipArchive->read(&entry.ZipHeader, sizeof(zip_local_file_header_t));
	c8 * name = nullptr;
	c8 * simple_name = nullptr;

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	entry.ZipHeader.Signature = ByteConverter::ByteSwap(entry.ZipHeader.Signature);
	entry.ZipHeader.VersionRequired = ByteConverter::ByteSwap(entry.ZipHeader.VersionRequired);
	entry.ZipHeader.Flags = ByteConverter::ByteSwap(entry.ZipHeader.Flags);
	entry.ZipHeader.CompressionMethod = ByteConverter::ByteSwap(entry.ZipHeader.CompressionMethod);
	entry.ZipHeader.LastModTime = ByteConverter::ByteSwap(entry.ZipHeader.LastModTime);
	entry.ZipHeader.LastModDate = ByteConverter::ByteSwap(entry.ZipHeader.LastModDate);
	entry.ZipHeader.Descriptor.CRC32 = ByteConverter::ByteSwap(entry.ZipHeader.Descriptor.CRC32);
	entry.ZipHeader.Descriptor.CompressedSize = ByteConverter::ByteSwap(entry.ZipHeader.Descriptor.CompressedSize;
	entry.ZipHeader.Descriptor.UncompressedSize = ByteConverter::ByteSwap(entry.ZipHeader.Descriptor.UncompressedSize);
	entry.ZipHeader.FilenameLen = ByteConverter::ByteSwap(entry.ZipHeader.FilenameLen);
	entry.ZipHeader.ExtraFieldLen = ByteConverter::ByteSwap(entry.ZipHeader.ExtraFieldLen);
#endif

	if (entry.ZipHeader.Signature != ZIP_LOCAL_FILE_HEADER_SIG)
		return false;

	// Extract the name
	name = new c8[entry.ZipHeader.FilenameLen+1];
	name[entry.ZipHeader.FilenameLen] = '\0';
	ZipArchive->read(name, entry.ZipHeader.FilenameLen);
	entry.FullName = String(name);
	FileUtils::ConvertPath(entry.FullName);
	entry.Name = FileUtils::StripDirectory(entry.FullName);
	delete [] name;

	ZipArchive->seek(EFSP_CURRENT, entry.ZipHeader.ExtraFieldLen);
	if ((entry.ZipHeader.Flags & ZIP_DATA_DESCRIPTOR_FLAG) != 0)
	{
		ZipArchive->read(&entry.ZipHeader.Descriptor, sizeof(zip_data_descriptor_t));
	}

	entry.Offset = ZipArchive->getCurrentPosition();

#if defined(_FIRE_ENGINE_DEBUG_ZIP_)
    Logger::Get()->log(ES_DEBUG, "io::ZipFileReader", "%s entry read", entry.FullName.c_str());
#endif

	// Seek to the next possible local file header
	ZipArchive->seek(EFSP_CURRENT, entry.ZipHeader.Descriptor.CompressedSize);

	FileEntries.push_back(entry);
	return true;
}

} // namespace io
} // namespace fire_engine

