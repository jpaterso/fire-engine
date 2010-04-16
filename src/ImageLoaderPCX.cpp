/**
 * FILE:    ImageLoaderPCX.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ImageLoaderPCX.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Image Loader for PCX files.
**/

#include "ImageLoaderPCX.h"
#include "Image.h"
#include "String.h"
#include "Logger.h"
#include "ColorConverter.h"
#include "ByteConverter.h"
#include "IFile.h"
#include "File.h"
#include "FileSystem.h"
#include <String.h>

#define PCX_MAGIC_NUMBER 0x0A // the first byte of every PCX File
#define PCX_RLE_ENCODED  0xC0 // an encoded RLE packet

namespace fire_engine
{

ImageLoaderPCX::ImageLoaderPCX()
{
}

ImageLoaderPCX::~ImageLoaderPCX()
{
}

Image * ImageLoaderPCX::load(const String& filename, io::IFileProvider * fileProvider) const
{
	u8 *        texel_data = nullptr;
	u8 *        color_map  = nullptr;
	Image *     image      = nullptr;
	io::IFile * file       = io::FileSystem::Get()->openReadFile(filename, false, io::EFOF_READ|io::EFOF_BINARY, fileProvider);
	PCXHeader   pcxh;
	dimension2i dim;

	if (file == nullptr)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX", 
			               "Could not open %s for reading: file not found", filename.c_str());
		return nullptr;
	}

	file->read(&pcxh, sizeof(PCXHeader));
#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	pcxh.xmin = ByteConverter::ByteSwap(pcxh.xmin);
	pcxh.ymin = ByteConverter::ByteSwap(pcxh.ymin);
	pcxh.xmax = ByteConverter::ByteSwap(pcxh.xmax);
	pcxh.ymax = ByteConverter::ByteSwap(pcxh.ymax);
	pcxh.horizontal_res = ByteConverter::ByteSwap(pcxh.horizontal_res);
	pcxh.vertical_res = ByteConverter::ByteSwap(pcxh.vertical_res):
	pcxh.bpline = ByteConverter::ByteSwap(pcxh.bpline);
	pcxh.header_interpretation = ByteConverter::ByteSwap(pcxh.header_interpretation);
	pcxh.vss_horizontal = ByteConverter::ByteSwap(pcxh.vss_horizontal);
	pcxh.vss_vertical = ByteConverter::ByteSwap(pcxh.vss_vertical);
#endif

	if (pcxh.magic != PCX_MAGIC_NUMBER)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"%s is not a valid PCX file: the magic number (%d) is not the expected %d",
			file->getFilename().c_str(), pcxh.magic, PCX_MAGIC_NUMBER);
		return 0;
	}

#if defined(_FIRE_ENGINE_DEBUG_PCX_)
	printf("Statistics for %s\n", file->getFilename().c_str());
	printf("---------------");
	for (s32 i = 0; i < file->getFilename().length(); i++)
		putchar('-');
	putchar('\n');
	printf("Magic number:               %d\n", pcxh.magic);
	printf("Version:                    %d\n", pcxh.version);
	printf("RLE:                        %d\n", pcxh.version);
	printf("Bits per pixel (plane):     %d\n", pcxh.bpp);
	printf("Image dimension (xmin):     %d\n", pcxh.xmin);
	printf("Image dimension (ymin):     %d\n", pcxh.ymin);
	printf("Image dimension (xmax):     %d\n", pcxh.xmax);
	printf("Image dimension (ymax):     %d\n", pcxh.ymax);
	printf("Horizontal resolution:      %d\n", pcxh.horizontal_res);
	printf("Vertical resolution:        %d\n", pcxh.vertical_res);
	printf("Number of planes:           %d\n", pcxh.num_planes);
	printf("Bytes per line:             %d\n", pcxh.bpline);
	printf("Header interpretation:      %d\n", pcxh.header_interpretation);
	printf("Video screen size (horiz):  %d\n", pcxh.vss_horizontal);
	printf("Video screen size (vert):   %d\n\n", pcxh.vss_vertical);
#endif

	dim = dimension2i(pcxh.xmax-pcxh.xmin+1, pcxh.ymax-pcxh.ymin+1);

	//TODO Add support for other PCX types
	if (pcxh.bpp != 8)
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"%d bit-per-pixel-per-plane not supported", pcxh.bpp);
		return 0;
	}

	if (pcxh.rle != 0)
		texel_data = decompressRLE(pcxh, file);

	switch (pcxh.num_planes)
	{
	case 1:
		if (texel_data == 0)
		{
			texel_data = new u8[dim.getWidth()*dim.getHeight()];
			file->read(texel_data, dim.getWidth()*dim.getHeight());
		}
		color_map  = new u8[256*3];
		// Make sure we read the color map data from the right offset
		file->seek(io::EFSP_END, -256*3);
		file->read(color_map, 256*3);
		image = new Image(Image::EIDT_R8G8B8, dim, false);
		ColorConverter::convert8BitTo24Bit(texel_data, (u8*)image->data(), dim,
			color_map, false, 3, 0, false);
		delete [] texel_data;
		delete [] color_map;
		break;

	case 3:
		if (texel_data == 0)
		{
			texel_data = new u8[dim.getWidth()*dim.getHeight()*3];
			file->read(texel_data, dim.getWidth()*dim.getHeight()*3);
		}
		image = new Image(Image::EIDT_R8G8B8, dim, false, (void*)texel_data);
		break;

	default:
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"Invalid number of planes: %d", pcxh.num_planes);
		return 0;
	}

	if (file->fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"An error occurred while loading %s", file->getFilename().c_str());
		delete image;
		image = nullptr;
	}
	delete file;
	return image;
}

u8 * ImageLoaderPCX::decompressRLE(PCXHeader pcxh, io::IFile * file) const
{
	s32  decompressed_size = pcxh.num_planes*(pcxh.xmax-pcxh.xmin+1)*(pcxh.ymax-pcxh.ymin+1);
	u8 * decompressed_data = new u8[decompressed_size];
	s32  current_size      = 0;
	u8   header, footer;

	while (current_size < decompressed_size)
	{
		file->read(&header, 1);
		if (header >= PCX_RLE_ENCODED)
		{
			file->read(&footer, 1);
			for (s32 i = 0; i < (header - PCX_RLE_ENCODED); i++)
				decompressed_data[current_size++] = footer;
		}
		else
			decompressed_data[current_size++] = header;
	}
	return decompressed_data;
}

ImageLoaderPCX::pcx_rle_data_t ImageLoaderPCX::compressRLE(const u8 * data, s32 size) const
{
	pcx_rle_data_t rle;
	s32 current_offset_orig = 0;
	s32 current_offset_rle  = 0;
	u8  count, header;
	rle.data = new u8[2*size]; // At most, it will be twice the original size

	while (current_offset_orig < size)
	{
		header = data[current_offset_orig];
		count  = 1;
		// We can encode at most 63 consecutive same values, so stop after finding 63 of them!
		while ((data[current_offset_orig+count] == header) && ((current_offset_orig+count) < size) && (count < 63))
			count++;
		if (header < PCX_RLE_ENCODED && count == 1)
			rle.data[current_offset_rle++] = header;
		else
		{
			rle.data[current_offset_rle++] = count ^ PCX_RLE_ENCODED; // we the header as being encoded
			rle.data[current_offset_rle++] = header;
		}
		current_offset_orig += count;

	}
	rle.size = current_offset_rle;

	return rle;
}

bool ImageLoaderPCX::write(const String& filename, const Image * image) const
{
	PCXHeader pcxh;
	pcx_rle_data_t rle;
	io::File f(filename, io::EFOF_CREATE|io::EFOF_WRITE|io::EFOF_TRUNCATE|io::EFOF_BINARY);

	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"Could not open %s for writing. Aborting screenshot", filename.c_str());
		return false;
	}

	memset(&pcxh, 0, sizeof(PCXHeader));
	pcxh.magic          = 0x0A;
	pcxh.version        = 5;
	pcxh.rle            = 5;
	pcxh.bpp            = 8;
	pcxh.xmax           = image->width()-1;
	pcxh.ymax           = image->height()-1;
	pcxh.horizontal_res = image->width();
	pcxh.vertical_res   = image->height();
	pcxh.num_planes     = 3;
	pcxh.bpline         = 3*image->width();

	rle = compressRLE((const u8*)image->data(), 3*image->width()*image->height());

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	pcxh.xmin = ByteConverter::ByteSwap(pcxh.xmin);
	pcxh.ymin = ByteConverter::ByteSwap(pcxh.ymin);
	pcxh.xmax = ByteConverter::ByteSwap(pcxh.xmax);
	pcxh.ymax = ByteConverter::ByteSwap(pcxh.ymax);
	pcxh.horizontal_res = ByteConverter::ByteSwap(pcxh.horizontal_res);
	pcxh.vertical_res = ByteConverter::ByteSwap(pcxh.vertical_res):
	pcxh.bpline = ByteConverter::ByteSwap(pcxh.bpline);
	pcxh.header_interpretation = ByteConverter::ByteSwap(pcxh.header_interpretation);
	pcxh.vss_horizontal = ByteConverter::ByteSwap(pcxh.vss_horizontal);
	pcxh.vss_vertical = ByteConverter::ByteSwap(pcxh.vss_vertical);
#endif

	f.write(&pcxh, sizeof(PCXHeader));
	//f.write(image->data(), 3*image->width()*image->height());
	f.write(rle.data, rle.size);
	delete [] rle.data;

	if (f.fail())
	{
		Logger::Get()->log(ES_HIGH, "ImageLoaderPCX",
			"An error occurred while writing to %s, aborting", filename.c_str());
		f.remove();
		return false;
	}
	return true;
}

}
