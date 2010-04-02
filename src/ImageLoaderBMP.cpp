/**
 *  $Id: ImageLoaderBMP.cpp 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Definitions for the ImageLoaderBMP class declared in ImageLoaderBMP.h
**/

#include "ImageLoaderBMP.h"
#include "ByteConverter.h"
#include "Logger.h"
#include "Image.h"
#include "dimension2.h"
#include "ColorConverter.h"
#include "IFile.h"
#include "File.h"
#include "FileSystem.h"
#include <string.h>

#define BMP_MAGIC_NUMBER 0x4D42

namespace fire_engine
{

ImageLoaderBMP::ImageLoaderBMP()
{
}

ImageLoaderBMP::~ImageLoaderBMP()
{
}

Image * ImageLoaderBMP::load(const string& filename, io::IFileProvider * fileProvider) const
{
	Image * image   = nullptr;
	u8 *    data    = nullptr;
	u8 *    palette = nullptr;
	io::IFile * file = io::FileSystem::Get()->openReadFile(filename, false, 
		io::EFOF_READ|io::EFOF_BINARY, fileProvider);

	dimension2i      dim;
	u8               padding;
	BitmapFileHeader bmfh;
	BitmapInfoHeader bmih;

	if (file == nullptr)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderBMP", 
			               "Could not open %s for reading: file not found", filename.c_str());
		return nullptr;
	}

	file->read(&bmfh, sizeof(BitmapFileHeader));
	file->read(&bmih, sizeof(BitmapInfoHeader));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	bmfh.bfType = ByteConverter::ByteSwap(bmfh.bfType);
	bmfh.bfSize = ByteConverter::ByteSwap(bmfh.bfSize);
	bmfh.bfReserved1 = ByteConverter::ByteSwap(bmfh.bfReserved1);
	bmfh.bfReserved2 = ByteConverter::ByteSwap(bmfh.bfReserved2);
	bmfh.bfOffBits = ByteConverter::ByteSwap(bmfh.bfOffBits);
	bmih.biSize = ByteConverter::ByteSwap(bmih.biSize);
	bmih.biWidth = ByteConverter::ByteSwap(bmih.biWidth);
	bmih.biHeight = ByteConverter::ByteSwap(bmih.biHeight);
	bmih.biPlanes = ByteConverter::ByteSwap(bmih.biPlanes);
	bmih.biBitCount = ByteConverter::ByteSwap(bmih.biBitCount);
	bmih.biCompression = ByteConverter::ByteSwap(bmih.biCompression);
	bmih.biSizeImage = ByteConverter::ByteSwap(bmih.biSizeImage);
	bmih.biXPelsPerMeter = ByteConverter::ByteSwap(bmih.biXPelsPerMeter );
	bmih.biYPelsPerMeter = ByteConverter::ByteSwap(bmih.biYPelsPerMeter);
	bmih.biClrUsed = ByteConverter::ByteSwap(bmih.biClrUsed);
	bmih.biClrImportant = ByteConverter::ByteSwap(bmih.biClrImportant);
#endif

	if (bmfh.bfType != BMP_MAGIC_NUMBER)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderBMP",
			"Invalid BMP magic number %d in %s", bmfh.bfType, file->getFilename().c_str());
		return 0;
	}

#ifdef	_FIRE_ENGINE_DEBUG_BMP_
	printf("Statistics for %s\n", file->getFilename().c_str());
	printf("---------------");
	for (s32 i = 0; i < file->getFilename().length(); i++)
		putchar('-');
	putchar('\n');
	printf("Image type              = %u\n", bmfh.bfType);
	printf("Image size              = %u\n", bmfh.bfSize);
	printf("Reserved field 1        = %u\n", bmfh.bfReserved1);
	printf("Reserved field 2        = %u\n", bmfh.bfReserved2);
	printf("Bitmap data starts at   = %u\n", bmfh.bfOffBits);
	printf("Bitmap info header size = %u\n", bmih.biSize);
	printf("Width                   = %d\n", bmih.biWidth);
	printf("Height                  = %d\n", bmih.biHeight);
	printf("Number of planes        = %u\n", bmih.biPlanes);
	printf("Bits per pixel          = %u\n", bmih.biBitCount);
	printf("Compression used        = %u\n", bmih.biCompression);
	printf("Image size (adjusted)   = %u\n", bmih.biSizeImage);
	printf("Horizontal resolution   = %d\n", bmih.biXPelsPerMeter);
	printf("Vertical resolution     = %d\n", bmih.biYPelsPerMeter);
	printf("# colors in bitmap      = %u\n", bmih.biClrUsed);
	printf("# important colors      = %u\n\n", bmih.biClrImportant);
#endif

	dim.set(bmih.biWidth, bmih.biHeight);

	padding = 0;
	while ((bmih.biWidth * bmih.biBitCount / 8 + padding) % 4 != 0)
		padding++;

	// Sometimes, the bmih.biSizeImage field is set to 0, so we re-calculate it
	if (bmih.biSizeImage == 0)
		bmih.biSizeImage = dim.getWidth()*dim.getHeight()*bmih.biBitCount/8+padding*bmih.biHeight;
	switch (bmih.biBitCount)
	{
		case 1:
		case 2:
		case 4:
			Logger::Get()->log(ES_HIGH,
				string("ImageLoaderBMP"),
				string("") + bmih.biBitCount + string("bit bitmaps not supported"));
			return nullptr;

		case 8:
			data = new u8[dim.getHeight()*(dim.getWidth()+padding)];
			palette = new u8[256*4];
			file->read(palette, 4*256);
			file->read(data, dim.getHeight()*(dim.getWidth()+padding));
			image = new Image(Image::EIDT_R8G8B8, dim, false);
			ColorConverter::convert8BitTo24Bit((const u8*)data, (u8*)image->data(), dim, palette, true, 4, padding, true);
			break;

		//TODO: test support for 16 bit bitmaps
		case 16:
			data       = new u8[bmih.biSizeImage];
			file->read(data, bmih.biSizeImage);
			image = new Image(Image::EIDT_A1R5G5B5, dim, false);
			ColorConverter::convert16BitTo16Bit((const u16 *)data, (u16 *)image->data(), dim, padding, true);
			break;

		case 24:
			data       = new u8[bmih.biSizeImage];
			file->read(data, bmih.biSizeImage);
			image = new Image(Image::EIDT_R8G8B8, dim, false);
			ColorConverter::convert24BitTo24Bit((u8 *)data, (u8 *)image->data(), dim, padding, true, true);
			break;

		case 32:
			data       = new u8[bmih.biSizeImage];
			file->read(data, bmih.biSizeImage);
			// Even in the case of a 32 bit bitmap we disable alpha channel
			// blending, because it's always set to 0, which makes the
			// picture 100% transparent, ie. invisible.
			image = new Image(Image::EIDT_A8R8G8B8, dim, false);
			ColorConverter::convert32BitTo32Bit((const s32 *)data, (s32 *)image->data(), dim, padding, true);
			break;
	}

	delete [] static_cast<u8*>(data);
	if (palette)
		delete [] palette;

	if (file->fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderBMP",
			"An error occurred while loading %s", file->getFilename().c_str());
		delete image;
		image = nullptr;
	}
	delete file;
	return image;
}

bool ImageLoaderBMP::write(const string& filename, const Image * image) const
{
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;
	s32 padding  = 0;
	u8 * data    = 0;
	s32 datasize = 0;
	io::File f(filename, io::EFOF_CREATE|io::EFOF_TRUNCATE|io::EFOF_WRITE|io::EFOF_BINARY);
	const u8 * imageData = static_cast<const u8*>(image->data());

	memset(static_cast<void *>(&bfh), 0, sizeof(BitmapFileHeader));
	memset(static_cast<void *>(&bih), 0, sizeof(BitmapInfoHeader));

	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderBMP",
			"Could not open %s for writing. Aborting screenshot", filename.c_str());
		return false;
	}

	// Add the padding that bitmaps have
	while((image->width()*3 + padding)%4 != 0)
		padding++;
	datasize = (image->width()*3+padding)*image->height();
	data = new u8[datasize];
	memset(data, 0, datasize);
	for (s32 i = 0, j = 0; i <= datasize - 3; i += 3, j += 3)
	{
		if ((i+padding) % (image->width()+padding) == 0)
			i = i+padding;
		data[i]   = imageData[j+2];
		data[i+1] = imageData[j+1];
		data[i+2] = imageData[j];
	}

	bfh.bfType      = BMP_MAGIC_NUMBER;
	bfh.bfSize      = sizeof(BitmapFileHeader)+sizeof(BitmapInfoHeader)+(image->width()*3+padding)*image->height();
	bfh.bfOffBits   = sizeof(BitmapFileHeader)+sizeof(BitmapInfoHeader);
	bih.biSize      = sizeof(BitmapInfoHeader);
	bih.biWidth     = image->width();
	bih.biHeight    = image->height();
	bih.biBitCount  = 24;
	bih.biSizeImage = datasize;

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	bfh.bfType = ByteConverter::ByteSwap(bfh.bfType);
	bfh.bfSize = ByteConverter::ByteSwap(bfh.bfSize);
	bfh.bfOffBits = ByteConverter::ByteSwap(bfh.bfOffBits);
	bih.biSize = ByteConverter::ByteSwap(bih.biSize);
	bih.biWidth = ByteConverter::ByteSwap(bih.biWidth);
	bih.biHeight = ByteConverter::ByteSwap(bih.biHeight);
	bih.biBitCount = ByteConverter::ByteSwap(bih.biBitCount);
	bih.biSizeImage = ByteConverter::ByteSwap(bih.biSizeImage);
#endif

	f.write(&bfh, sizeof(BitmapFileHeader));
	f.write(&bih, sizeof(BitmapInfoHeader));
	f.write(data, datasize);

	delete [] data;
	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderBMP",
			"An error occurred while writing to %s, aborting", filename.c_str());
		f.remove();
		return false;
	}
	return true;
}

} // namespace fire_engine

