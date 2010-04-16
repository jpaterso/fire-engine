/**
 * FILE:    Color.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Color.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A class template for storing a single RGBA color.
**/

#ifndef	COLOR_H_INCLUDED
#define	COLOR_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"
#include <String.h>

namespace fire_engine
{

template <class Real>
class _FIRE_ENGINE_API_ Color
{
public:
	/** The white color. */
	static const Color<Real> WHITE;
	/** The black color. */
	static const Color<Real> BLACK;
	/** The red color. */
	static const Color<Real> RED;
	/** The green color. */
	static const Color<Real> GREEN;
	/** The blue color. */
	static const Color<Real> BLUE;
	/** The yellow color. */
	static const Color<Real> YELLOW;
	/** The orange color. */
	static const Color<Real> ORANGE;
	/** The grey color. */
	static const Color<Real> GREY;

public:
	Color() : mRed(0), mGreen(0), mBlue(0), mAlpha(0)
	{
	}

	Color(Real red, Real green, Real blue, Real alpha)
		 : mRed(red), mGreen(green), mBlue(blue), mAlpha(alpha)
	{
	}

	Color(const Real * components)
	{
		set(components);
	}

	inline Real red() const
	{
		return mRed;
	}

	inline Real green() const
	{
		return mGreen;
	}

	inline Real blue()  const
	{
		return mBlue;
	}

	inline Real alpha() const
	{
		return mAlpha;
	}

	inline void setRed(Real red)
	{
		mRed = red;
	}

	inline void setGreen(Real green)
	{
		mGreen = green;
	}

	inline void setBlue(Real blue)
	{
		mBlue = blue;
	}

	inline void setAlpha(Real alpha)
	{
		mAlpha = alpha;
	}

	inline const Real * v() const
	{
		return mColor;
	}

	inline void set(const Real * components)
	{
		memcpy(mColor, components, 4*sizeof(components));
	}

	inline void set(Real red, Real green, Real blue, Real alpha)
	{
		mRed = red;
		mGreen = green;
		mBlue = blue;
		mAlpha = alpha;
	}

	Color<Real> operator*(f32 val) const
	{
		return Color<Real>((Real)(mRed*val), (Real)(mGreen*val), 
			(Real)(mBlue*val), (Real)(mAlpha*val));
	}

	Color<Real>& operator*=(f32 val)
	{
		mRed *= val;
		mGreen *= val;
		mBlue *= val;
		mAlpha *= val;
		return *this;
	}

	Color<Real>  operator+(const Color<Real>& other) const
	{
		return Color<Real>(mRed+other.red(), mGreen+other.green(),
			mBlue+other.blue(), mAlpha+other.alpha());
	}

	Color<Real>& operator+=(const Color<Real>& other)
	{
		mRed += other.red();
		mGreen += other.green();
		mBlue += other.blue();
		mAlpha += other.alpha();
		return *this;
	}

	inline bool operator==(const Color<Real>& rhs) const
	{
		return memcmp(mColor, rhs.v(), 4*sizeof(Real)) == 0;
	}

	inline bool operator!=(const Color<Real>& rhs) const
	{
		return memcmp(mColor, rhs.v(), 4*sizeof(Real)) != 0;
	}

	const Color<Real>& operator=(const Color<Real>& other)
	{
		memcpy(mColor, other.v(), 4*sizeof(Real));
		return *this;
	}

	String toString() const
	{
		return String("Color[ ") + mRed + ", " + mGreen + ", " + mBlue
		+ ", " + mAlpha + " ]";
	}

	static u32 hash(const Color<Real>& key)
	{
		return static_cast<u32>(key.red()+key.green()+key.blue()+key.alpha());
	}
private:
	union
	{
		Real mColor[4];
		struct
		{
			Real mRed;
			Real mGreen;
			Real mBlue;
			Real mAlpha;
		};
	};
};

/** Some useful typedefs. */
typedef Color<u8>  Color8;
typedef Color<f32> Color32;

} // namespace fire_engine

#endif // COLOR_H_INCLUDED
