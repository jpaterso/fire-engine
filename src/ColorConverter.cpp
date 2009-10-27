/**
 *  $Id: ColorConverter.cpp 87 2007-08-05 11:57:43Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Definition of the ColorConver class, useful for converting image data
**/

#include "ColorConverter.h"
#include "ByteConverter.h"
#include <stdio.h>
#include <string.h>

namespace fire_engine
{

u8 * ColorConverter::m_buffer = new u8[4];

const u8 * ColorConverter::RGB16(u8 r, u8 g, u8 b)
{
	m_buffer[0] = (r & 0xF8) | ((g & 0xF8) >> 5);
	m_buffer[1] = ((g & 0xF8) << 3) | ((b & 0xF8) >> 2);
	return m_buffer;
}

const u8 * ColorConverter::RGBA16(u8 r, u8 g, u8 b, u8 a)
{
	m_buffer[0] = (r & 0xF8) | ((g & 0xF8) >> 5);
	m_buffer[1] = ((g & 0xF8) << 3) | ((b & 0xF8) >> 2) | ((a & 0x80) >> 7);
	return m_buffer;
}

const u8 * ColorConverter::RGB24(u8 r, u8 g, u8 b)
{
	m_buffer[0] = r;
	m_buffer[1] = g;
	m_buffer[2] = b;
	return m_buffer;
}

const u8 * ColorConverter::RGBA32(u8 r, u8 g, u8 b, u8 a)
{
	m_buffer[0] = r;
	m_buffer[1] = g;
	m_buffer[2] = b;
	m_buffer[3] = a;
	return m_buffer;
}

void ColorConverter::R8G8B8toB8G8R8(const u8 * in, u8 * out)
{
	out[0] = in[2];
	out[1] = in[1];
	out[2] = in[0];
}

void ColorConverter::B8G8R8toR8G8B8(const u8 * in, u8 * out)
{
	out[0] = in[2];
	out[1] = in[1];
	out[2] = in[0];
}

u32 ColorConverter::A8R8G8B8toB8G8R8A8(u32 in)
{
	return ((in & 0xFF000000) >> 24) | ((in & 0xFF0000) >> 8)
		| ((in & 0xFF00) << 8) | ((in & 0xFF) << 24);
}

u32 ColorConverter::B8G8R8A8toA8R8G8B8(u32 in)
{
	return ((in & 0xFF000000) >> 24) | ((in & 0xFF0000) >> 8)
		| ((in & 0xFF00) << 8) | ((in & 0xFF) << 24);
}

u16 ColorConverter::A1R5G5B5toB5G5R5A1(u16 in)
{
	return ((in & 0x8000) >> 15) | ((in & 0x7C00) >> 9)
		| ((in & 0x03E0) << 1) | ((in & 0x1F) << 11);
}

void ColorConverter::B5G5R5A1toR5G5B5A1(const u8 * in, u8 * out)
{
	out[0] = ((in[1] & 0x3E) << 2) | (in[1] & 0x07);
	out[1] = (in[1] & 0xC0) | ((in[0] & 0xF8) >> 2) | (in[1] | 0x01);
}


u16 ColorConverter::B5G5R5A1toA1R5G5B5(u16 in)
{
	return ((in & 0xF800) >> 11) | ((in & 0x07C0) >> 1)
		| ((in & 0x3E) << 9) | ((in & 0x01) << 15);
}

u16 ColorConverter::A8R8G8B8toA1R5G5B5(u32 in)
{
	return ((in & 0x80000000) >> 16) | ((in & 0xF80000) >> 9)
		| ((in & 0xF800) >> 6) | ((in & 0xF8) >> 3);
}

u32 ColorConverter::A1R5G5B5toA8R8G8B8(u16 in)
{
	return ((in & 0x8000) << 16) | ((in & 0x7C00) << 9) | ((in & 0x03E0) << 6) | ((in & 0x1F) << 3);
}

u8 ColorConverter::Red(u16 argb)
{
	return (argb & 0x7C00) >> 10;
}

u8 ColorConverter::Green(u16 argb)
{
	return (argb & 0x03E0) >> 5;
}

u8 ColorConverter::Blue(u16 argb)
{
	return argb & 0x001F;
}

u8 ColorConverter::Alpha(u16 argb)
{
	return (argb & 0x8000) >> 15;
}

void ColorConverter::convert8BitTo16Bit(const u8 * data, u16 * out, const dimension2i& dim, const u16 * cmap, u8 padWidth)
{
	s32 i, j;
	u32 index;
	u32 realSize, totalSize;

	realSize = dim.getWidth() * dim.getHeight();
	totalSize = (dim.getWidth() + padWidth) * dim.getHeight();

	for (i = 0; i < dim.getHeight(); i++) {
		index = i * (dim.getWidth() + padWidth);
		for (j = 0; j < dim.getWidth(); j++) {
			out[i * dim.getWidth() + j] = cmap[data[index + j]];
		}
	}
}

void ColorConverter::convert8BitTo24Bit(const u8 * in, u8 * out, const dimension2i& dim,
										const u8 * cmap, bool cmap_bgr, s32 cmap_texelsize,
										u8 padding, bool flip)
{
	if (flip)
		out += dim.getWidth()*dim.getHeight()*3;
	for (s32 i = 0; i < dim.getHeight(); i++)
	{
		if (flip)
			out -= dim.getWidth()*3;
		for (s32 j = 0; j < dim.getWidth(); j++, in++)
		{
			if (!cmap_bgr)
				memcpy((void *)&out[j*3], (const void *)&cmap[*in*cmap_texelsize], 3);
			else
				B8G8R8toR8G8B8(&cmap[*in*cmap_texelsize], &out[j*3]);
		}
		in += padding;
		if (!flip)
			out += dim.getWidth()*3;
	}
}

void ColorConverter::convert8BitTo32Bit(const u8 * data, u32 * out, const dimension2i& dim, const u32 * cmap, u8 padWidth)
{
	s32 i, j;
	u32 index;
	u32 realSize, totalSize;

	realSize = dim.getWidth() * dim.getHeight();
	totalSize = (dim.getWidth() + padWidth) * dim.getHeight();

	for (i = 0; i < dim.getHeight(); i++) {
		index = i * (dim.getWidth() + padWidth);
		for (j = 0; j < dim.getWidth(); j++) {
			out[i * dim.getWidth() + j] = (cmap[data[index + j]]);
		}
	}
}

void ColorConverter::convert16BitTo16Bit(const u16 * in, u16 * out, const dimension2i& dim, u8 padding, bool flip)
{
	if (flip)
		out += dim.getWidth()*dim.getHeight();
	for (s32 i = 0; i < dim.getHeight()-1; i++)
	{
		if (flip)
			out -= dim.getWidth();
		memcpy((void*)out, (const void*)in, dim.getWidth()*sizeof(s16));
		in += dim.getWidth();
		in += padding;
		if (!flip)
			out += dim.getWidth();
	}
}

void ColorConverter::convert24BitTo24Bit(const u8 * in, u8 * out, const dimension2i& dim, u8 padding,
										 bool is_bgr, bool flip)
{
	if (flip)
		out += dim.getHeight()*dim.getWidth()*3;
	for (s32 i = 0; i < dim.getHeight(); i++)
	{
		if (flip)
			out -= dim.getWidth()*3;
		if (is_bgr)
			for (s32 j = 0; j < dim.getWidth(); j++)
			{
				out[j*3+0] = in[j*3+2];
				out[j*3+1] = in[j*3+1];
				out[j*3+2] = in[j*3+0];
			}
		else
			memcpy((void *)out, (const void *)in, 3*dim.getWidth());
		if (!flip)
			out += dim.getWidth()*3;
		in += dim.getWidth()*3;
		in += padding;
	}
}

void ColorConverter::convert32BitTo32Bit(const s32 * in, s32 * out, const dimension2i& dim, u8 padding, bool flip)
{
	if (flip)
		out += dim.getWidth()*dim.getHeight();
	for (s32 i = 0; i < dim.getHeight(); i++)
	{
		if (flip)
			out -= dim.getWidth();
		memcpy((void *)out, (const void *)in, dim.getWidth()*sizeof(s32));
		if (!flip)
			out += dim.getWidth();
		in += dim.getWidth();
		in += padding;
	}
}

} // namespace fe
