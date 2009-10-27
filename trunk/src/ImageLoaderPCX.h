/**
 * FILE:    ImageLoaderPCX.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ImageLoaderPCX.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Image Loader for PCX files.
**/

#ifndef IMAGELOADERPCX_H_INCLUDED
#define IMAGELOADERPCX_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ILoader.h"
#include "IWriter.h"
#include "dimension2.h"
#include <stdio.h>

namespace fire_engine
{

class Image;
class string;

namespace io
{
class IFile;
}

class _FIRE_ENGINE_API_ ImageLoaderPCX : public ILoader<Image>, public IWriter<Image>
{
private:
#pragma pack(push, 4)
	//! The header for a PCX file - exactly 128 bytes
	typedef struct
	{
		u8  magic;                        // magic number, 0x0A
		u8  version;                      // PCX version
		u8  rle;                          // encoding (RLE)
		u8  bpp;                          // bits/pixel (per plane)
		u16 xmin;                         // image dimension: xmin
		u16 ymin;                         // image dimension: ymin
		u16 xmax;                         // image dimension: xmax
		u16 ymax;                         // image dimension: ymax
		u16 horizontal_res;               // horizontal resolution
		u16 vertical_res;                 // vertical resolution
		u8  header_palette[48];           // header palette
		u8  reserved;                     // reserved byte (0x00)
		u8  num_planes;                   // number of planes
		u16 bpline;                       // bytes per line
		u16 header_interpretation;        // 1: color or B&W, 2: grayscale
		u16 vss_horizontal;               // Video screen size (horizontal)
		u16 vss_vertical;                 // Video screen size (vertical)
		u8  blank[54];
	} PCXHeader;
#pragma pack(pop)

public:
	ImageLoaderPCX(void);
	virtual ~ImageLoaderPCX(void);

	virtual Image * load(const string& filename) const;

	virtual bool write(const string& filename, const Image * image) const;

private:
	u8 * decompressRLE(PCXHeader pcxh, io::IFile * file) const;

	typedef struct
	{
		s32 size;
		u8 * data;
	} pcx_rle_data_t;

	/** Compress image data using Run-Length Encoding (RLE)
	 This is currently unused, as it is horrendously slow.
	 \param data The uncompressed data
	 \param size The size, in bytes, of the uncompressed data
	 \return     An pcx_rle_data_t structure, containing the compressed data,
	  along with it's size. It can safely be delete [] ed after use */
	pcx_rle_data_t compressRLE(const u8 * data, s32 size) const;
};

}

#endif // IMAGELOADERPCX_H_INCLUDED
