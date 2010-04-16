/**
 *  $Id: ImageLoaderBMP.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  File loader for Bitmap (*.bmp) files.
**/

#ifndef __IMAGE_LOADER_BMP_H
#define __IMAGE_LOADER_BMP_H

#include <stdio.h>

#include "Types.h"
#include "CompileConfig.h"
#include "ILoader.h"
#include "IWriter.h"
#include "String.h"

namespace fire_engine
{

class Image;

namespace io
{
class IFile;
class IFileProvider;
}

class _FIRE_ENGINE_API_ ImageLoaderBMP : public ILoader<Image>, public IWriter<Image>
{
private:
#pragma pack(push, 1)
	//! First part of the header of a Bitmap file
	typedef struct
	{
		u16	bfType;
		u32	bfSize;
		u16	bfReserved1;
		u16	bfReserved2;
		u32	bfOffBits;
	} BitmapFileHeader;

	//! Second part of the header of a Bitmap File
	typedef struct
	{
		u32	biSize;
		s32	biWidth;
		s32	biHeight;
		u16	biPlanes;
		u16	biBitCount;
		u32	biCompression;
		u32	biSizeImage;
		s32	biXPelsPerMeter;
		s32	biYPelsPerMeter;
		u32	biClrUsed;
		u32	biClrImportant;
	} BitmapInfoHeader;
#pragma pack(pop)

public:
	//! Default ctor
	ImageLoaderBMP(void);

	//! Default dtor
	virtual ~ImageLoaderBMP(void);

	//! Implementation for ILoader
	virtual Image * load(const String& filename, io::IFileProvider * fileProvider) const;

	virtual bool write(const String& filename, const Image * image) const;

protected:
};

} // namespace fire_engine

#endif
