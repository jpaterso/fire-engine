/**
 * FILE:    Image.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Image.cpp 116 2007-10-05 13:22:45Z jpaterso $
 * PURPOSE: Implementation of the Image class
**/

#include "Image.h"
#include "Math.h"
#include "Color.h"
#include "ColorConverter.h"
#include <string.h>

namespace fire_engine
{

Image::Image(void)
	: m_type(EIDT_NONE),
	  m_loaded(false),
	  m_use_alpha_chanel(false),
	  m_data(0),
	  m_texelsize(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Image");
#endif
}

Image::Image(EIMAGE_DATA_TYPE type, dimension2i d, bool useAlphaChanel, void * data)
	: m_type(type),
	  m_loaded(true),
	  m_use_alpha_chanel(useAlphaChanel),
	  m_dim(d)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Image");
#endif
	setTexelSize();
	if (data)
		m_data = data;
	else
		m_data = new u8[m_dim.getWidth()*m_dim.getHeight()*m_texelsize];
}

Image::Image(const Image& other)
	: m_type(other.type()),
	  m_loaded(other.loaded()),
	  m_use_alpha_chanel(other.useAlphaChanel()),
	  m_dim(other.dim())
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Image");
#endif
	setTexelSize();
	m_data        = new u8[m_texelsize*m_dim.getWidth()*m_dim.getHeight()];
	memcpy(m_data, other.data(), m_texelsize*m_dim.getWidth()*m_dim.getHeight());
}

Image::~Image()
{
	if (m_data)
		delete [] (u8*)m_data;
}

s32 Image::width(void) const
{
	return m_dim.getWidth();
}

s32 Image::height(void) const
{
	return m_dim.getHeight();
}

void Image::set(EIMAGE_DATA_TYPE type, void * data, dimension2i dim, bool useAlphaChanel)
{
	m_type             = type;
	m_data             = data;
	m_dim              = dim;
	m_loaded           = true;
	m_use_alpha_chanel = useAlphaChanel;
	setTexelSize();
}

void Image::gammaCorrect(f32 gamma)
{
	f32 invg = 1.0f / gamma;
	u8 * data = static_cast<u8*>(m_data);
	for (s32 i = 0; i < m_dim.getWidth()*m_dim.getHeight()*m_texelsize; i += m_texelsize)
	{
		data[i]   = (u8)Math32::Pow(data[i], invg);
		data[i+1] = (u8)Math32::Pow(data[i+1], invg);
		data[i+2] = (u8)Math32::Pow(data[i+2], invg);
	}
}

void Image::resize(const dimension2i& newdim, EIMAGE_RESIZE_ALGORITHM mode)
{
	switch (mode)
	{
		case EIRA_NEAREST_NEIGHBOUR:
			resize_nearest_neighbour(newdim);
			break;
		case EIRA_BILINEAR:
			if (m_texelsize == 2)
				resize_bilinear16(newdim);
			else if (m_texelsize == 3 || m_texelsize == 4)
				resize_bilinear24_32(newdim);
			break;
	}
}

void Image::resize_nearest_neighbour(const dimension2i& new_dimension)
{
	f32 height_ratio = static_cast<f32>(m_dim.getHeight()) / new_dimension.getHeight();
	f32 width_ratio  = static_cast<f32>(m_dim.getWidth()) / new_dimension.getWidth();
	s32 xtexel, ytexel; // The coordinates of the wanted texel
	u8 * data          = new u8[m_texelsize*new_dimension.getWidth()*new_dimension.getHeight()];
	u8 * original_data = static_cast<u8*>(m_data);
	s32 i, j;
	for (i = 0; i < new_dimension.getHeight(); i++)
	{
		xtexel = static_cast<s32>(i * height_ratio);
		for (j = 0; j < new_dimension.getWidth(); j++)
		{
			ytexel = static_cast<s32>(j * width_ratio);
			memcpy(&data[(i*new_dimension.getWidth()+j)*m_texelsize],
				&original_data[(xtexel*m_dim.getWidth()+ytexel)*m_texelsize], m_texelsize);
		}
	}
	delete [] original_data;
	m_dim       = new_dimension;
	m_data      = (void*)data;
}

void Image::resize_bilinear16(const dimension2i& newdim)
{
}

void Image::resize_bilinear24_32(const dimension2i& newdim)
{
	u8 * data = new u8[newdim.getWidth()*newdim.getHeight()*m_texelsize];
	u8 * original_data = static_cast<u8 *>(m_data);
	f32 height_ratio = static_cast<f32>(m_dim.getHeight()) / newdim.getHeight();
	f32 width_ratio  = static_cast<f32>(m_dim.getWidth()) / newdim.getWidth();
	f32 u, v; // The values to interpolate the pixels with
	s32 i, j, tr, tl, br, bl;
	s32 texel_w, texel_h;
	u8 r00, r01, r10, r11;
	u8 g00, g01, g10, g11;
	u8 b00, b01, b10, b11;
	u8 a00, a01, a10, a11;
	u8 * rgbfinal = new u8[m_texelsize];
	for (i = 0; i < newdim.getHeight(); i++)
	{
		texel_h = static_cast<s32>(i*height_ratio);
		u       = i*height_ratio - texel_h;
		for (j = 0; j < newdim.getWidth(); j++)
		{
			texel_w = static_cast<s32>(j*width_ratio);
			v       = j*width_ratio - texel_w;
			tl = (texel_h*m_dim.getWidth()+texel_w)*m_texelsize;
			tr = (texel_h*m_dim.getWidth()+texel_w+1)*m_texelsize;
			bl = ((texel_h+1)*m_dim.getWidth()+texel_w)*m_texelsize;
			br = ((texel_h+1)*m_dim.getWidth()+texel_w+1)*m_texelsize;
			if ((texel_h != m_dim.getHeight()-1) && (texel_w != m_dim.getWidth()-1))
			{
				r00 = original_data[tl]; r01 = original_data[tr]; r10 = original_data[bl]; r11 = original_data[br];
				g00 = original_data[tl+1]; g01 = original_data[tr+1]; g10 = original_data[bl+1]; g11 = original_data[br+1];
				b00 = original_data[tl+2]; b01 = original_data[tr+2]; b10 = original_data[bl+2]; b11 = original_data[br+2];
				rgbfinal[0] = static_cast<u8>((1.0-u)*(1.0-v)*r00 + (1.0-u)*v*r01 + u*(1.0-v)*r10 + u*v*r11);
				rgbfinal[1] = static_cast<u8>((1.0-u)*(1.0-v)*g00 + (1.0-u)*v*g01 + u*(1.0-v)*g10 + u*v*g11);
				rgbfinal[2] = static_cast<u8>((1.0-u)*(1.0-v)*b00 + (1.0-u)*v*b01 + u*(1.0-v)*b10 + u*v*b11);
				if (m_texelsize > 3)
				{
					a00 = original_data[tl+3]; a01 = original_data[tr+3]; a10 = original_data[bl+3]; a11 = original_data[br+3];
					rgbfinal[3] = static_cast<u8>((1.0-u)*(1.0-v)*a00 + (1.0-u)*v*a01 + u*(1.0-v)*a10 + u*v*a11);
				}
			}
			else if ((texel_h == m_dim.getHeight()-1) && (texel_w != m_dim.getWidth()-1))
			{
				r00 = original_data[tl]; r01 = original_data[tr];
				g00 = original_data[tl+1]; g01 = original_data[tr+1];
				b00 = original_data[tl+2]; b01 = original_data[tr+2];
				rgbfinal[0] = static_cast<u8>((1.0-v)*r00 + v*r01);
				rgbfinal[1] = static_cast<u8>((1.0-v)*g00 + v*g01);
				rgbfinal[2] = static_cast<u8>((1.0-v)*b00 + v*b01);
				if (m_texelsize > 3)
				{
					a00 = original_data[tl+3]; a01 = original_data[tr+3];
					rgbfinal[3] = static_cast<u8>((1.0-v)*a00 + v*a01);
				}
			}
			else if ((texel_h != m_dim.getHeight()-1) && (texel_w == m_dim.getWidth()-1))
			{
				r00 = original_data[tl]; r10 = original_data[bl];
				g00 = original_data[tl+1]; g10 = original_data[bl+1];
				b00 = original_data[tl+2]; b10 = original_data[bl+2];
				rgbfinal[0] = static_cast<u8>((1.0-u)*r00 + u*r10);
				rgbfinal[1] = static_cast<u8>((1.0-u)*g00 + u*g10);
				rgbfinal[2] = static_cast<u8>((1.0-u)*b00 + u*b10);
				if (m_texelsize > 3)
				{
					a00 = original_data[tl+3]; a10 = original_data[bl+3];
					rgbfinal[3] = static_cast<u8>((1.0-u)*a00 + u*a10);
				}
			}
			else
			{
				rgbfinal[0] = original_data[tl];
				rgbfinal[1] = original_data[tl+1];
				rgbfinal[2] = original_data[tl+2];
				if (m_texelsize > 3)
					rgbfinal[3] = original_data[tl+3];
			}
			memcpy(&data[(i*newdim.getWidth()+j)*m_texelsize], rgbfinal, m_texelsize);
		}
	}
	delete [] original_data;
	delete [] rgbfinal;
	m_data = data;
	m_dim = newdim;
}

void Image::setTexelSize(void)
{
	switch (m_type)
	{
	case EIDT_NONE:
		m_texelsize = 0; break;
	case EIDT_A1R5G5B5:
	case EIDT_R5G6B5:
		m_texelsize = 2; break;
	case EIDT_R8G8B8:
		m_texelsize = 3; break;
	case EIDT_A8R8G8B8:
		m_texelsize = 4; break;
	}
}

array<Image *> * Image::generateMipmaps(void) const
{
	s32 weakest_link         = (m_dim.getWidth() > m_dim.getHeight()) ?
		m_dim.getHeight() : m_dim.getWidth(); // the weakest (ie. smallest) dimension
	s32 num_mipmaps          = 1;
	array<Image *> * mipmaps = 0;
	while ((weakest_link >>= 1) > 0)
		num_mipmaps++;
	mipmaps = new array<Image *>(num_mipmaps);
	mipmaps->push_back(new Image(*this));
	for (s32 i = 1; i < num_mipmaps; i++)
		mipmaps->push_back(mipmaps->at(i-1)->generateNextMipmap());
	mipmaps->reverse();
	return mipmaps;
}

Image * Image::generateNextMipmap(void) const
{
	Image * newimage = new Image(*this);
	newimage->resize(dimension2i(m_dim.getWidth()/2, m_dim.getHeight()/2),
		Image::EIRA_BILINEAR); // use bilerp algorithm when resizing
	return newimage;
}

bool Image::isPowerOfTwo(void) const
{
	s32 d;
	for (d = 1; d < m_dim.getHeight(); d *= 2)
		;
	if (d != m_dim.getHeight())
		return false;
	for (d = 1; d < m_dim.getWidth(); d *= 2)
		;
	if (d != m_dim.getWidth())
		return false;
	return true;
}
}
