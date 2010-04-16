/**
 * FILE:    ImageLoaderTGA.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ImageLoaderTGA.cpp 116 2007-10-05 13:22:45Z jpaterso $
 * PURPOSE: Implementation of a file loader/writer for TGA (.TGA) files.
**/

#include "ImageLoaderTGA.h"
#include "ByteConverter.h"
#include "dimension2.h"
#include "ColorConverter.h"
#include "Image.h"
#include "Logger.h"
#include "IFile.h"
#include "File.h"
#include "FileSystem.h"
#include <String.h>

namespace fire_engine
{

ImageLoaderTGA::ImageLoaderTGA()
{
}

ImageLoaderTGA::~ImageLoaderTGA()
{
}

Image * ImageLoaderTGA::load(const String& filename, io::IFileProvider * fileProvider) const
{
	Image * image    = nullptr;
	c8 *    id       = nullptr;
	void *  data     = nullptr;
	io::IFile * file = io::FileSystem::Get()->openReadFile(filename, false,  io::EFOF_READ|io::EFOF_BINARY, fileProvider);
	
	dimension2i  dim;
	tga_header_t header;
	u8           bpp;

	if (file == nullptr)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderTGA", 
			               "could not open file %s: file not found", filename.c_str());
		return nullptr;
	}

	file->read(&header, sizeof(tga_header_t));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	header.cmapOrigin = ByteConverter::ByteSwap(header.cmapOrigin);
	header.cmapLength = ByteConverter::ByteSwap(header.cmapLength);
	header.xOrigin = ByteConverter::ByteSwap(header.xOrigin);
	header.yOrigin = ByteConverter::ByteSwap(header.yOrigin);
	header.imageWidth = ByteConverter::ByteSwap(header.imageWidth);
	header.imageHeight = ByteConverter::ByteSwap(header.imageHeight);
#endif

#if defined(_FIRE_ENGINE_DEBUG_TGA_)
	printf("Statistics for %s\n", filename.c_str());
	printf("---------------");
	for (s32 i = 0; i < filename.length(); i++)
		putchar('-');
	putchar('\n');
	printf("Color map type        =  %u\n", header.cmapType);
	printf("TGA Image Type        =  %u\n", header.imageType);
	printf("Color map origin      =  %u\n", header.cmapOrigin);
	printf("Color map length      =  %u\n", header.cmapLength);
	printf("Color map entry size  =  %u\n", header.cmapEntrySize);
	printf("X origin              =  %u\n", header.xOrigin);
	printf("Y origin              =  %u\n", header.yOrigin);
	printf("Width                 =  %u\n", header.imageWidth);
	printf("Height                =  %u\n", header.imageHeight);
	printf("Image pixel size      =  %u\n", header.pixelSize);
	printf("Image descriptor byte =  %u\n\n", header.imageDescriptor);
#endif

	dim.set(header.imageWidth, header.imageHeight);
	bpp = header.pixelSize / 8;

	// Read in the header file
	if (header.idFieldSize > 0)
	{
		id = new c8[header.idFieldSize + 1];
		file->read(id, header.idFieldSize);
		id[header.idFieldSize] = '\0';
#if defined(_FIRE_ENGINE_DEBUG_TGA_)
		printf("Identification field = %s\n", id);
#endif
	}

	switch (header.imageType)
	{
		case ETGADT_NO_DATA:
			return 0;
			break;

		case ETGADT_UC_COLOR_MAPPED:
		case ETGADT_RLE_COLOR_MAPPED:
			Logger::Get()->log(ES_HIGH, "ImageLoaderTGA", "color mapped data not supported");
			delete file;
			return 0;
			break;

		case ETGADT_UC_RGB:
		case ETGADT_UC_BW:
			data = new u8[bpp*dim.getWidth()*dim.getHeight()];
			file->read(data, bpp*dim.getWidth()*dim.getHeight());
			break;

		case ETGADT_RLE_RGB:
		case ETGADT_RLE_BW:
			data = decompressRLE(header, file, dim);
			break;

		default:
			Logger::Get()->log(ES_HIGH, "ImageLoaderTGA", "%s: invalid TGA header", file->getFilename().c_str());
			delete file;
			return nullptr;
			break;
	}

	switch (header.pixelSize)
	{
		case 8:
			Logger::Get()->log(ES_HIGH, "ImageLoaderTGA", "8 bit color mapped data not supported");
			return nullptr;
			break;

		//TODO: test with 16 bit data
		case 16:
			image = new Image(Image::EIDT_A1R5G5B5, dim, true);
			ColorConverter::convert16BitTo16Bit((const u16 *)data, (u16 *)image->data(), dim, 0, true);
			break;

		case 24:
			image = new Image(Image::EIDT_R8G8B8, dim, false);
			ColorConverter::convert24BitTo24Bit((const u8 *)data, (u8 *)image->data(), dim, 0, true, true);
			break;

		case 32:
			image = new Image(Image::EIDT_A8R8G8B8, dim, true);
			ColorConverter::convert32BitTo32Bit((const s32 *)data, (s32 *)image->data(), dim, 0, false);
			break;

		default:
			Logger::Get()->log(ES_HIGH, "ImageLoaderTGA",
				"invalid pixel size %d in file %s", header.pixelSize, file->getFilename().c_str());
			break;
	}
	delete [] static_cast<u8*>(data);
	delete file;
	return image;
}

bool ImageLoaderTGA::write(const String& filename, const Image * image) const
{
	tga_header_t header;
	io::File f(filename.c_str(), io::EFOF_CREATE|io::EFOF_WRITE|io::EFOF_TRUNCATE|io::EFOF_BINARY);
	s32 size  = image->width()*image->height()*3;
	u8 * data = new u8[size];

	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, String("ImageLoaderTGA"),
			String("Could not open %s for writing. Aborting screenshot"), filename.c_str());
		delete [] data;
		return false;
	}

	// TGA uses BGR, not RGB, so flip the data first
	for (s32 i = 0; i < size-3; i += 3)
		ColorConverter::R8G8B8toB8G8R8(&(static_cast<const u8*>(image->data())[i]), &data[i]);

	tga_rle_data_t t = compressRLE(data, 3, image->dim());
	delete [] data;

	memset(&header, 0, sizeof(tga_header_t));
	header.imageType   = ETGADT_UC_RGB;
	header.pixelSize   = 24;
	header.imageWidth  = image->width();
	header.imageHeight = image->height();
	header.imageType = ETGADT_RLE_RGB;
#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	header.imageWidth = ByteConverter::ByteSwap(header.imageWidth);
	header.imageHeight = ByteConverter::ByteSwap(header.imageHeight);
#endif

	f.write(&header, sizeof(tga_header_t));
	f.write(t.data, t.size);

	delete [] t.data;

	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderTGA",
			"An error occurred while writing to %s, aborting", filename.c_str());
		f.remove();
		return false;
	}
	return true;
}

u8 * ImageLoaderTGA::decompressRLE(tga_header_t theader, io::IFile * file, const dimension2i& dim) const
{
	u8   bpp    = theader.pixelSize / 8;
	u8 * data   = new u8[bpp*dim.getWidth()*dim.getHeight()];;
	s32  offset = 0;
	u8   header;
	s32  i, j;

	while (offset < (bpp*dim.getWidth()*dim.getHeight()))
	{
		file->read(&header, 1);
		if (header & ETGADT_RLE_RLE)
		{
			file->read(&data[offset], bpp);
			for (i = 1; i <= (header & 0x7F); i++)
			{
				for (j = 0; j < bpp; j++)
					data[offset + (bpp * i) + j] = data[offset + j];
			}
			offset += bpp * ((header & 0x7F) + 1);
		}
		else
		{
			file->read(&data[offset], bpp*((header&0x7F)+1));
			offset += bpp*((header&0x7F)+1);
		}
	}

	return data;
}

ImageLoaderTGA::tga_rle_data_t ImageLoaderTGA::compressRLE(const u8 * data, u8 bpp, const dimension2i& dim) const
{
	tga_rle_data_t rle;
	rle.data = new u8[2*bpp*dim.getWidth()*dim.getHeight()];
	rle.size = 0;
	u8 * header = new u8[bpp];
	s32 width, height, pcount;
	u32 cur_off;

	for (height = 0; height < dim.getHeight(); height++)
	{
		width = 0;
		while (width < dim.getWidth())
		{
			cur_off = (height*dim.getWidth()+width)*bpp;
			memcpy(header, &data[cur_off], bpp);
			if (width == dim.getWidth()-1) // The last element on the scan-line
			{
				rle.data[rle.size++] = 0x00;
				memcpy(&rle.data[rle.size], header, bpp);
				rle.size = rle.size+bpp;
				width++;
			}
			else // not the last element on the scan-line
			{
				pcount = 0;
				// Count number of consecutive identical pixels
				while ((memcmp(header, &data[cur_off+(pcount+1)*bpp], bpp) == 0)
					&& (width+pcount+1 < dim.getWidth()) && (pcount <= 0x7F))
				{
					pcount++;
				}
				/** If this number is greater than 0 (ie, there is more than 0 that is
				 identical to the first, then compress the data. */
				if (pcount > 0)
				{
					rle.data[rle.size++] = (pcount-1) | ETGADT_RLE_RLE;
					memcpy(&rle.data[rle.size], header, bpp);
					width += pcount;
					rle.size = rle.size+bpp;
				}
				/** Otherwise, count the number of consecutive different pixels, and write
				 them. */
				else
				{
					pcount = 1;
					while ((memcmp(&data[cur_off+pcount*bpp], &data[cur_off+(pcount+1)*bpp], bpp) != 0)
						&& (width+pcount+1 < dim.getWidth()) && (pcount <= 0x7F))
					{
						pcount++;
					}
					rle.data[rle.size++] = pcount-1;
					memcpy(&rle.data[rle.size], &data[cur_off], pcount*bpp);
					width += pcount;
					rle.size = rle.size+pcount*bpp;
				}
			}
		}
	}

	return rle;
}

} // namespace fire_engine
