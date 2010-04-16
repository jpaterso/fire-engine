/**
 * FILE:    ImageLoaderTGA.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ImageLoaderTGA.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Declaration of a file loader/writer for TGA (.TGA) files.
**/

#ifndef IMAGELOADERTGA_H_INCLUDED
#define IMAGELOADERTGA_H_INCLUDED

#include <stdio.h>

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "String.h"
#include "ILoader.h"
#include "IWriter.h"

namespace fire_engine
{

class Image;

namespace io
{
class IFile;
class IFileProvider;
}

//! A Loader/Writer for Targa (.TGA) files
/** Targa files are a type of image file that supports basic lossless
 compression (RLE), color mapped data, and uncompressed data.
 For loading an image:
 <code>
 ImageLoaderTGA * tga_loader = new ImageLoaderTGA();
 Image * file = tga_loader->load("filename.tga");
 </code>
 It is in little endian format. */
class _FIRE_ENGINE_API_ ImageLoaderTGA : public ILoader<Image>, public IWriter<Image>
{
private:
#pragma pack(push, 1) // Structs need to be 1-byte aligned
	//! Structure representing the header of a TGA file
	typedef struct
	{
		u8	idFieldSize;     // Number of characters in identification field
		u8	cmapType;        // Color map type
		u8	imageType;       // TGA Image type
		u16	cmapOrigin;      // Index (lo-hi) of the first color map entry
		u16	cmapLength;      // count (lo-hi) of color map entries
		u8	cmapEntrySize;   // size of each color map entry
		u16	xOrigin;         // X origin of the image (lo-hi)
		u16	yOrigin;         // Y origin of the image (lo-hi)
		u16	imageWidth;      // Width of image (lo-hi)
		u16	imageHeight;     // Height of image (lo-hi)
		u8	pixelSize;       // Image pixel size
		u8	imageDescriptor; // Image descriptor byte
	} tga_header_t;
#pragma pack (pop)

	//! The different types of data a TGA file can contain
	enum ETGA_DATA_TYPES
	{
		ETGADT_NO_DATA          = 0x00,
		ETGADT_UC_COLOR_MAPPED  = 0x01,
		ETGADT_UC_RGB           = 0x02,
		ETGADT_UC_BW            = 0x03,
		ETGADT_RLE_COLOR_MAPPED = 0x09,
		ETGADT_RLE_RGB          = 0x0A,
		ETGADT_RLE_BW           = 0x0B,
		ETGADT_RLE_RLE          = 0x80
	};

public:
	ImageLoaderTGA();

	virtual ~ImageLoaderTGA();

	//! Implementation for ILoader
	virtual Image * load(const String& filename, io::IFileProvider * fileProvider) const;

	//! Implementations for IWriter
	virtual bool write(const String& filename, const Image * image) const;

private:
	/** Decompress the RLE data found in the .TGA file. */
	u8 * decompressRLE(tga_header_t theader, io::IFile * file, const dimension2i& dim) const;

	/** A structure to contain the compressed data. */
	typedef struct
	{
		s32 size;
		u8 * data;
	} tga_rle_data_t;

	/** Compress TGA data using Run-Length Encoding (RLE). This can gain a lot of
	 space if there are numerous identical consecutive pixels.
	 \param data The data to compress.
	 \param bpp  The number of Bytes-per-pixel (2, 3 or 4).
	 \param dim  The dimensions of the data (one cannot compress over a scan-line).
	 \return     The compressed data, along with it's size. */
	tga_rle_data_t compressRLE(const u8 * data, u8 bpp, const dimension2i& dim) const;
};

} // namespace fire_engine

#endif // IMAGELOADERTGA_H_INCLUDED
