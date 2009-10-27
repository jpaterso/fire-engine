/**
 *  $Id: ColorConverter.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  the ColorConverter class contains methods for converting different data
 *  types.
**/

#ifndef __COLOR_CONVERTER_H
#define __COLOR_CONVERTER_H

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "Color.h"

namespace fire_engine
{

class _FIRE_ENGINE_API_ ColorConverter
{
public:
	/** Color packers. Note that in the case of 16 byte colors, where
	 values have to be clamped, only the highest bytes will be kept. For
	 example, to have an alpha of 1, the hex value 0x80 (128) must be used. */
	static const u8 * RGB16(u8 r, u8 g, u8 b);
	static const u8 * RGBA16(u8 r, u8 g, u8 b, u8 a);
	static const u8 * RGB24(u8 r, u8 g, u8 b);
	static const u8 * RGBA32(u8 r, u8 g, u8 b, u8 a);

	//! Extract components from a A1R5G5B5 packed pixel
	static u8 Red(u16 argb);
	static u8 Green(u16 argb);
	static u8 Blue(u16 argb);
	static u8 Alpha(u16 argb);

	/**
	 *	Convert a 24 bit R8G8B8 entry to an B8G8R8 entry
	 *	@param	in	The R8G8B8 entry
	 *	@param	out	A place to store the B8G8R8 entry
	**/
	static void R8G8B8toB8G8R8(const u8 * in, u8 * out);

	/**
	 *	Convert a 24 bit B8G8R8 entry to an R8G8B8 entry
	 *	@param	in	The B8G8R8 entry
	 *	@param	out	A place to store the R8G8B8 entry
	**/
	static void B8G8R8toR8G8B8(const u8 * in, u8 * out);

	/**
	 *	Convert a 32 bit A8R8G8B8 entry to an B8G8R8A8 entry
	 *	@param	in	The A8R8G8B8 entry
	 *	@return		The B8G8R8A8 entry
	**/
	static u32 A8R8G8B8toB8G8R8A8(u32 in);

	/**
	 *	Convert a 32 bit B8G8R8A8 entry to an A8R8G8B8 entry
	 *	@param	in	The B8G8R8A8 entry
	 *	@return		The A8R8G8B8 entry
	**/
	static u32 B8G8R8A8toA8R8G8B8(u32 in);

	/**
	 *	Convert a 16 bit A1R5G5B5 entry to an B5G5R5A1 entry
	 *	@param	in	The A1R5G5B5 entry
	 *	@return		The B5G5R5A1 entry
	**/
	static u16 A1R5G5B5toB5G5R5A1(u16 in);

	static void B5G5R5A1toR5G5B5A1(const u8 * in, u8 * out);

	/**
	 *	Convert a 16 bit B5G5R5A1 entry to an A1R5G5B5 entry
	 *	@param	in	The B5G5R5A1 entry
	 *	@return		The A1R5G5B5 entry
	**/
	static u16 B5G5R5A1toA1R5G5B5(u16 in);

	/**
	 *	Convert a 32 bit A8R8G8B8 entry to a 16 bit A1R5G5B5  entry
	 *	@param	in	The A8R8G8B8 entry
	 *	@return		The A1R5G5B5 entry
	**/
	static u16 A8R8G8B8toA1R5G5B5(u32 in);

	/**
	 *	Convert a 16bit A1R5G5B5 entry to a 32 bit A8R8G8B8 entry
	 *	@param	in	The A1R5G5B5 entry
	 *	@return		The A8R8G8B8 entry
	**/
	static u32 A1R5G5B5toA8R8G8B8(u16 in);

	/**
	 *	Convert an 8 bit image with a 16 bit color map to 16 bit data
	 *	image
	 *	@param	data		The raw data from the image file
	 *	@param	out			A place to store the data in
	 *	@param	dim			The real dimensions of the image, width × height
	 *	@param	cmap		The color map in 16 bit A1R5G5B5 format
	 *	@param	padWidth	A value to add to the actual width, because
	 *						some image files use padding to adjust to some
	 *						boundary.
	**/
	static void convert8BitTo16Bit(const u8 * data, u16 * out, const dimension2i& dim, const u16 * cmap, u8 padWidth);

	//static void convert4BitTo24Bit(const u8 * data, u8 * out, const dimension2i& dim, const u8 * cmap, bool fourbyte, bool cmap_bgr, u8 padding);
	/**
	 *	Convert an 8 bit image with a 32 bit color map to 32 bit data
	 *	image
	 *	@param	data		The raw data from the image file
	 *	@param	out			A place to store the data in
	 *	@param	dim			The real dimensions of the image, width × height
	 *	@param	cmap		The color map in 32 bit A8R8G8B8 format
	 *	@param	padWidth	A value to add to the actual width, because
	 *						some image files use padding to adjust to some
	 *						boundary.
	**/
	static void convert8BitTo32Bit(const u8 * data, u32 * out, const dimension2i& dim, const u32 * cmap, u8 padWidth);

	/** Convert 8 bit indexed color mapped data to 24 bit RGB data.
	 \param in             The indexed data.
	 \param out            A place to store the un-indexed data.
	 \param dim            The final dimension of the image.
	 \param cmap           The color map.
	 \param cmap_bgr       Set this to true if the color map is in BGR format,
	                       instead of RGB.
	 \param cmap_texelsize The texel size of the color map. Can be 3 or 4. If it
	                       is 4, then the final component will be ignored.
	 \param padding        The amount of padding on every scan-line of the input
	                       data.
	 \param flip           Set this to true if the data should be flipped */
	static void convert8BitTo24Bit(const u8 * in, u8 * out, const dimension2i& dim, const u8 * cmap, bool cmap_bgr, s32 cmap_texelsize, u8 padding, bool flip);

	static void convert16BitTo16Bit(const u16 * in, u16 * out, const dimension2i& dim, u8 padding, bool flip);

	static void convert24BitTo24Bit(const u8 * data, u8 * out, const dimension2i& dim,
		u8 padding, bool is_bgr, bool flip);

	static void convert32BitTo32Bit(const s32 * in, s32 * out, const dimension2i& dim, u8 padding, bool flip);

private:
	static u8 * m_buffer;
};

} // namespace fire_engine


#endif
