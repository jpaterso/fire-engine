/**
 *  $Id: Image.h 119 2007-12-03 02:12:08Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  An image contains either RGBA16, RGB24 or RGBA32 data. It is useful for
 *  displaying static data on the scene.
**/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "Array.h"
#include "Object.h"

namespace fire_engine
{

class _FIRE_ENGINE_API_ Image : public virtual Object
{
public:
	enum EIMAGE_DATA_TYPE
	{
		EIDT_NONE,     // empty data
		EIDT_A1R5G5B5, // 16 bit A1R5G5B5 pixel packing
		EIDT_R5G6B5,   // 16-bit R5G6B5 pixel packing
		EIDT_R8G8B8,   // 24-bit R8G8B8 pixel packing
		EIDT_A8R8G8B8  // 32-bit A8R8G8B8 pixel packing
	};

	//! The algorithm to use when resizing the image
	enum EIMAGE_RESIZE_ALGORITHM
	{
		EIRA_NEAREST_NEIGHBOUR,
		EIRA_BILINEAR
	};

	/**
	 *	Basic constructor, does nothing
	**/
	Image();

	/**
	 *	Create an Image
	 *	@param	type	The format the data is in
	 *	@param	data	The texture data
	 *	@param	d		The dimensions of the texture
	 *	@param	useAlphaChanel	true to use the alpha chanel, fals
	**/
	Image(EIMAGE_DATA_TYPE type, const dimension2i d, bool useAlphaChanel = false, void * data = 0);

	//! Deep copy of another texture
	Image(const Image& other);

	/**
	 *	Basic destructor. Clears the data
	**/
	~Image();

	//! Set data for a texture
	void set(EIMAGE_DATA_TYPE type, void * data, dimension2i dim, bool useAlphaChanel = false);

	/**
	 *	Get the data for the texture
	 *	@return	A pointer to the data in the texture
	**/
	inline const void * data(void) const;
	inline void * data(void);

	/**
	 *	Get the data format for the texture
	 *	@return	The data format the data is stored in
	**/
	inline EIMAGE_DATA_TYPE type() const;

	/**
	 *	Get the dimension of the texture
	 *	@return	The dimension of the texture
	**/
	inline const dimension2i& dim() const;

	/**
	 *	Get the width of the texture, in pixels
	 *	@return	the width of the texture, in pixels
	**/
	s32 width() const;

	/**
	 *	Get the height of the texture, in pixels
	 *	@return	the height of the imatexturege, in pixels
	**/
	s32 height() const;

	/**
	 *	Check whether a texture is loaded
	 *	@return	true if the image is loaded, false otherwise
	**/
	inline bool loaded() const;

	/**
	 *	Does this image use the alpha chanel ?
	 *	@return	true if the alpha chanel is used in the image,
	 *			false otherwise.
	**/
	inline bool useAlphaChanel() const;

	/**
	 *  Set whether the Alpha chanel will be taken into account or not when rendering.
	 *  @param  uac Set to true if the image uses alpha chanel, false otherwise
	**/
	inline void setUseAlphaChanel(bool uac);

	inline s32 getBytesPerPixel() const;

	//! Apply a gamma value to the image. Should not be needed, but can be used for effects
	void gammaCorrect(f32 gamma);

	//! Resize an image
	void resize(const dimension2i& newdim, EIMAGE_RESIZE_ALGORITHM mode = EIRA_NEAREST_NEIGHBOUR);

	/** Generate the Array of mipmaps for the image. The first element of the
	 Array will be the original Image, and each subsequent Image will have
	 both its dimensions divided by two. The last Image will have at least
	 one of its dimensions equal to 1 */
	Array<Image *> * generateMipmaps() const;

	bool isPowerOfTwo() const;

private:
	EIMAGE_DATA_TYPE m_type;
	bool	         m_loaded;           //! Flag to set if the image is loaded or not
	bool	         m_use_alpha_chanel; //! Flag to set if the alpha chanel is used in the data
	void *	         m_data;             //! Pointer to the actual data
	dimension2i      m_dim;              //! Dimension in pixels of the image
	u8               m_texelsize;        //! The number of bytes per texel


public:
	//! Resize the picture, using the nearest neighbour algorithm
	void resize_nearest_neighbour(const dimension2i& new_dimension);

	/** Bilinear (bilerp for short) resizing of pictures. Instead of simply using
	 the closest texel, one uses the square starting with the closest texel as
	 the top-left corner. A 2-dimensional weighted average is computed between
	 the four texel, and the new texel is set to that value. The end result is
	 much smoother pictures when they are blown up by a larger factor. */
	void resize_bilinear16(const dimension2i& newdim);
	void resize_bilinear24_32(const dimension2i& newdim);

	//! Set the texel size, in bytes
	void setTexelSize();

	//! Generate the next (smallest) mipmap for the image
	Image * generateNextMipmap(void) const;
};

inline const void * Image::data(void) const
{
	return m_data;
}

inline void * Image::data(void)
{
	return m_data;
}

inline Image::EIMAGE_DATA_TYPE Image::type(void) const
{
	return m_type;
}

inline const dimension2i& Image::dim(void) const
{
	return m_dim;
}

inline bool Image::loaded(void) const
{
	return m_loaded;
}

inline s32 Image::getBytesPerPixel(void) const
{
	return m_texelsize;
}

inline bool Image::useAlphaChanel(void) const
{
	return m_use_alpha_chanel;
}

inline void Image::setUseAlphaChanel(bool uac)
{
	m_use_alpha_chanel = uac;
}

} // namespace fire_engine

#endif
