/**
 *  $Id: dimension2.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Definiton of the dimension2 template, to store 2 dimensional values
**/

#ifndef	__DIMENSION2_H
#define	__DIMENSION2_H

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"

namespace fire_engine{

template<class T>
class _FIRE_ENGINE_API_ dimension2 {
public:
	/**
	 *	Initialize a zero dimension
	**/
	dimension2(void)
		: mWidth(0), mHeight(0)
	{
	}

	/**
	 *	Create a new 2D dimension
	 *	@param	x	The x dimension
	 *	@param	y	The y dimension
	**/
	dimension2(T width, T height)
		: mWidth(width), mHeight(height)
	{
	}

	dimension2(const dimension2<T>& r)
	{
		mWidth = r.getWidth();
		mHeight = r.getHeight();
	}

	/**
	 *	Basic destructor
	**/
	~dimension2(void)
	{
	}

	/**
	 *	Set the x and y dimension
	 *	@param	x	The x dimension
	 *	@param	y	The y dimension
	**/
	void set(T width, T height)
	{
		mWidth  = width;
		mHeight = height;
	}

	/**
	 *	Set the x dimension
	 *	@param	x	The x dimension
	**/
	void setWidth(T width)
	{
		mWidth = width;
	}

	/**
	 *	Set the y dimension
	 *	@param	y	The y dimension
	**/
	void setHeight(T height)
	{
		mHeight = height;
	}

	/**
	 *	Get the x dimension
	 *	@return	The x dimension
	**/
	T getWidth(void) const { return (mWidth); }

	/**
	 *	Get the y dimension
	 *	@return	The y dimension
	**/
	T getHeight(void) const { return (mHeight); }

	/**
	 *	Compute the area of the dimension
	 *	@return	The area of the dimension
	**/
	T area(void) const { return (mWidth * mHeight); }

	/**
	 *	Check whether 2 dimensions are equal
	 *	@param	other	The other dimension
	 *	@return			true if dimensions are equal, false otherwise
	**/
	bool operator==(const dimension2<T>& other) const
	{
		return (mWidth == other.getWidth() && mHeight == other.getHeight());
	}

	/**
	 *	Check whether 2 dimensions are different
	 *	@param	other	The other dimension
	 *	@return			true if dimensions are different, false otherwise
	**/
	bool operator!=(const dimension2<T>& other) const
	{
		return (mWidth != other.getWidth() || mHeight != other.getHeight());
	}

	/**
	 *	Assign a dimension to this one
	 *	@param	other	The dimension to assign to this one
	 *	@return			A reference to the current dimension
	**/
	const dimension2<T>& operator=(const dimension2<T>& other)
	{
		mWidth = other.getWidth();
		mHeight = other.getHeight();
		return *this;
	}

	dimension2<T> operator-(const dimension2<T>& other)
	{
		return dimension2<T>(mWidth-other.getWidth(), mHeight-other.getHeight());
	}

	dimension2<T> operator*(f32 val)
	{
		return dimension2<T>(static_cast<T>(mWidth*val), static_cast<T>(mHeight*val));
	}

	String toString() const
	{
		return String("dimension2[ ") + mWidth + " x " + mHeight + " ]";
	}

private:
	//! The x dimension
	T mWidth, mHeight;
};
// Useful typedefs
typedef dimension2<s32> dimension2i;
typedef dimension2<f32> dimension2f;

// We need this when we build DLLs, to ensure proper instantiation
template class _FIRE_ENGINE_API_ dimension2<s32>;
template class _FIRE_ENGINE_API_ dimension2<f32>;

} // namespace fire_engine

#endif
