/**
 * FILE:    vector2.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: vector2.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A 2-dimensional vector class
**/

#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"
#include <String.h>

namespace fire_engine
{

/** A two-dimensional vector, similar to the dimension2 class, but has x and y
 parameters instead of width and height. */
template <class Real>
class _FIRE_ENGINE_API_ vector2
{
public:
	/** Default constructor. */
	vector2()
		: mX(0), mY(0)
	{
	}

	/** Construct a 2-dimensional vector from 2 reals. */
	vector2(Real x, Real y)
		: mX(x), mY(y)
	{
	}

	/** Construct a 2-dimensional vector from an array of values. */
	vector2(const Real * values)
	{
		memcpy((void*)m_points, (const void*)values, 2*sizeof(Real));
	}

	/** Destructor. */
	~vector2()
	{
	}

	/** Returns the x component of the vector. */
	inline Real getX() const
	{
		return mX;
	}

	/** Returns the y component of the vector. */
	inline Real getY() const
	{
		return mY;
	}

	/** Sets the x component of the vector. */
	inline void setX(Real x)
	{
		mX = x;
	}

	/** Sets the y component of the vector. */
	inline void setY(Real y)
	{
		mY = y;
	}

	/** Sets the values for this vector. */
	inline void set(Real x, Real y)
	{
		mX = x;
		mY = y;
	}

	/** Returns a pointer to the values in the vector. */
	inline const Real * v(void) const
	{
		return m_points;
	}

	/** Returns whether the two vectors are equal. */
	bool operator==(const vector2& rhs)
	{
		return memcmp(m_points, rhs.v(), 2*sizeof(Real)) == 0;
	}

	/** Returns whether the two vectors are different. */
	bool operator!=(const vector2& rhs)
	{
		return memcmp(m_points, rhs.v(), 2*sizeof(Real)) != 0;
	}

	/** Returns the result of multiplying this vector by some constant factor. */
	vector2<Real> operator*(Real factor) const
	{
		return vector2<Real>(mX*factor, mY*factor);
	}

	/** Returns the result of subtracting some vector from this vector. */
	vector2<Real> operator-(const vector2<Real>& vec) const
	{
		return vector2<Real>(mX-vec.getX(), mY-vec.getY());
	}

	/** Multiplies this vector by some constant. */
	const vector2<Real>& operator*=(Real factor)
	{
		mX *= factor;
		mY *= factor;
		return *this;
	}

	/** Returns the result of adding some vector to this vector. */
	vector2<Real> operator+(const vector2<Real>& vec) const
	{
		return vector2<Real>(mX+vec.getX(), mY+vec.getY());
	}

	/** Adds some vector to this vector. */
	const vector2<Real>& operator+=(const vector2<Real>& vec)
	{
		mX += vec.getX();
		mY += vec.getY();
		return *this;
	}

	/** Returns a human-readable version of this vector. */
	String toString() const
	{
		return String("vector2[ ") + mX + ", " + mY + " ]";
	};

private:
	union
	{
		Real m_points[2];
		struct
		{
			Real mX;
			Real mY;
		};
	};
};

typedef vector2<f32> vector2f;
typedef vector2<f64> vector2d;

template class _FIRE_ENGINE_API_ vector2<f32>;
template class _FIRE_ENGINE_API_ vector2<f64>;


}
#endif

