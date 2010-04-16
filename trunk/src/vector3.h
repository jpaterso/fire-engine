/**
 *  $Id: vector3.h 118 2007-10-06 00:45:08Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  3-Dimensional vectors, with heaps of cool functions
**/

#ifndef	VECTOR3_H_INCLUDED
#define	VECTOR3_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Math.h"
#include "dimension2.h"
#include "String.h"
#include <String.h>

namespace fire_engine
{

/** The name of an axis. */
enum EAXIS
{
	EX_AXIS,
	EY_AXIS,
	EZ_AXIS
};

/** A 3-dimensional vector. */
template<class Real>
class _FIRE_ENGINE_API_ vector3
{
public:
	/** Default constructor. */
	vector3()
		: mX(0), mY(0), mZ(0), m_is_normalized(false)
	{
	}

	/** Construct a vector from its 3 points. */
	vector3(Real x, Real y, Real z)
		: mX(x), mY(y), mZ(z), m_is_normalized(false)
	{
	}

	/** Construct a vector3 from an Array of 3 points */
	vector3(const Real * values)
		: m_is_normalized(false)
	{
		memcpy((void*)m_points, (const void*)values, 3*sizeof(Real));
	}

	/** Construct a vector3 from 3 points, setting the normalized flag. This can
	 be dangerous! Set is_normalized to true only when 100% sure! */
	vector3(Real x, Real y, Real z, bool is_normalized)
		: mX(x), mY(y), mZ(z), m_is_normalized(is_normalized)
	{
	}

	/** Construct a vector3 from an Array of points, and set the is_normalized
	 flag. Use carefully! */
	vector3(const Real * values, bool is_normalized)
		: m_is_normalized(is_normalized)
	{
		memcpy((void*)m_points, (const void*)values, 3*sizeof(Real));
	}

	/** Destructor. */
	~vector3()
	{
	}

	/** Sets the new points for the vector. */
	inline void set(Real x, Real y, Real z)
	{
		mX = x;
		mY = y;
		mZ = z;
		m_is_normalized = false;
	}

	/** Sets the vertices for the vector.
	 \param points A pointer to the new points. */
	inline void set(const Real * points)
	{
		memcpy(m_points, points, 3*sizeof(Real));
	}

	/** Sets a new vector for this vector. */
	inline void set(const vector3<Real>& vec)
	{
		*this = vec;
	}

	/** Returns the X value for this vector. */
	inline Real getX() const
	{
		return mX;
	}

	/** Returns the Y value for this vector. */
	inline Real getY() const
	{
		return mY;
	}

	/** Returns the Z value for this vector. */
	inline Real getZ() const
	{
		return mZ;
	}

	/** Sets the X value for this vector. */
	inline void setX(Real x)
	{
		mX = x;
	}

	/** Sets the Y value for this vector. */
	inline void setY(Real y)
	{
		mY = y;
	}

	/** Sets the Z value for this vector. */
	inline void setZ(Real z)
	{
		mZ = z;
	}

	/** Normalize the vector.
	 \param fast Set this to true if a fast approximation of the length of the vector should be
	             used. */
	void normalize(bool fast = false)
	{
		Real invlen;
		// normalizing is quite slow, so just make sure it hasn't been
		// normalized before
		if (m_is_normalized == false)
		{
			if (fast)
				invlen = Math<Real>::FastInvSq(mX*mX + mY*mY + mZ*mZ);
			else
				invlen = 1 / Math<Real>::Sqrt(mX*mX + mY*mY + mZ*mZ);
			mX = mX*invlen;
			mY = mY*invlen;
			mZ = mZ*invlen;
			m_is_normalized = true;
		}
	}

	/** Returns the dot product between this vector and another. */
	inline Real dot(const vector3<Real>& other) const
	{
		return mX*other.getX() + mY*other.getY() + mZ*other.getZ();
	}

	/** Returns the cross product bwtween this vector and another vector. */
	inline vector3<Real> cross(const vector3<Real>& rhs) const
	{
		return vector3<Real>(mY*rhs.getZ()-mZ*rhs.getY(), mZ*rhs.getX()-mX*rhs.getZ(), mX*rhs.getY()-mY*rhs.getX());
	}

	/** Returns the distance between this vector and another. */
	Real distance(const vector3<Real>& other) const
	{
		Real xd = other.getX()-mX;
		Real yd = other.getY()-mY;
		Real zd = other.getZ()-mZ;
		return Math<Real>::Sqrt(xd*xd + yd*yd + zd*zd);
	}

	/** Returns the length of this vector. */
	inline Real length() const
	{
		return Math<Real>::Sqrt(mX*mX + mY*mY + mZ*mZ);
	}

	/** Inverts the direction of this vector. */
	void invert()
	{
		mX = 0 - mX;
		mY = 0 - mY;
		mZ = 0 - mZ;
	}

	/** Returns the angle between this vector and another. */
	Real angleBetween(const vector3<Real>& other) const
	{
		return Math<Real>::Arccos(this->dot(other) / (this->length()*other.length()));
	}

	/** Returns whether the vector is of unit length or not. */
	inline bool isNormalized() const
	{
		return Math<Real>::Equals(length(), 1);
	}

	/** Returns whether this vector is colinear with another given vector. */
	bool isColinear(const vector3<Real>& other) const
	{
		Real n;
		Real ox = other.getX();
		Real oy = other.getY();
		Real oz = other.getZ();

		if (Math<Real>::Get()->Equals(mX, ox) && Math<Real>::Get()->Equals(mY, oy) && Math<Real>::Get()->Equals(mZ, oz))
			return (true);

		if (Math<Real>::Get()->Equals(mX, 0) && Math<Real>::Get()->Equals(mY, 0) && Math<Real>::Get()->Equals(mZ, 0))
			return (true);

		if (Math<Real>::Get()->Equals(ox, 0) && Math<Real>::Get()->Equals(oy, 0) && Math<Real>::Get()->Equals(oz, 0))
			return (true);

		if ((Math<Real>::Get()->Equals(mX, 0) && !Math<Real>::Get()->Equals(ox, 0))
			|| (Math<Real>::Get()->Equals(mY, 0) && !Math<Real>::Get()->Equals(oy, 0))
			|| (Math<Real>::Get()->Equals(m_points[2], 0) && !Math<Real>::Get()->Equals(oz, 0))) {
				return (false);
		}
		else if ((!Math<Real>::Get()->Equals(mX, 0) && Math<Real>::Get()->Equals(ox, 0))
			|| (!Math<Real>::Get()->Equals(mY, 0) && Math<Real>::Get()->Equals(oy, 0))
			|| (!Math<Real>::Get()->Equals(m_points[2], 0) && Math<Real>::Get()->Equals(oz, 0))) {
				return (false);
		}
		else {
			if (!Math<Real>::Get()->Equals(mX, 0)) {
				n = mX / ox;
				if (!Math<Real>::Get()->Equals(mZ, 0)) {
					if (!Math<Real>::Get()->Equals(n * oy, mY)) {
						return (false);
					}
				}
				if (!Math<Real>::Get()->Equals(mZ, 0)) {
					n = mZ / oz;
					if (!Math<Real>::Get()->Equals(n * oz, mZ)) {
						return (false);
					}
				}
				return (true);
			}
			else if (!Math<Real>::Get()->Equals(mY, 0)) {
				n = mY / oy;
				if (!Math<Real>::Get()->Equals(mZ, 0)) {
					if (!Math<Real>::Get()->Equals(n * oz, mZ)) {
						return (false);
					}
				}
				return (true);
			} else {
				return (true);
			}
		}
	}

	/** Returns a pointer to the 3 coordinates of the vector. */
	inline const Real * v() const
	{
		return m_points;
	}

	/** Clamps an axis to a given range. */
	void clamp(EAXIS axis, Real min, Real max)
	{
		switch (axis)
		{
		case EX_AXIS:
			mX = Math<Real>::Clamp(mX, min, max);
			break;
		case EY_AXIS:
			mY = Math<Real>::Clamp(mY, min, max);
			break;
		case EZ_AXIS:
			mZ = Math<Real>::Clamp(mZ, min, max);
			break;
		}
	}

	/** Returns whether two vectors are equa. */
	bool operator==(const vector3<Real>& other) const
	{
		return mX == other.getX() && mY == other.getY() && mZ == other.getZ();
	}

	/** Returns whether two vectors are not equal. */
	bool operator!=(const vector3<Real>& other) const
	{
		return mX != other.getX() || mY != other.getY() || mZ != other.getZ();
	}

	/** Returns the result of adding a vector to this vector. */
	inline vector3<Real> operator+(const vector3<Real>& other) const
	{
		return vector3<Real>(mX+other.getX(), mY+other.getY(), mZ+other.getZ());
	}

	/** Returns the result of adding some constant to this vector. */
	inline vector3<Real> operator+(Real val) const
	{
		return vector3<Real>(mX+val, mY+val, mZ+val);
	}

	/** Returns the result of subtracting a vector from this one. */
	inline vector3<Real> operator-(const vector3<Real>& other) const
	{
		return vector3<Real>(mX-other.getX(), mY-other.getY(), mZ-other.getZ());
	}

	/** Returns the result of negating this vector. */
	inline vector3<Real> operator-() const
	{
		return vector3<Real>(-mX, -mY, -mZ);
	}

	/** Returns the result of subtracting some constant from this vector. */
	inline vector3<Real> operator-(Real val) const
	{
		return vector3<Real>(mX-val, mY-val, mZ-val);
	}

	/** Returns the cross of this vector with a given vector. */
	inline vector3<Real> operator*(const vector3<Real>& other) const
	{
		return this->cross(other);
	}

	/** Returns the result of multiplying the vector by some constant. */
	inline vector3<Real> operator*(Real val) const
	{
		return vector3<Real>(mX*val, mY*val, mZ*val);
	}

	/** Returns the result of dividing this vector by some constant. */
	vector3<Real> operator/(Real val) const
	{
		Real invval = 1 / val;
		return vector3<Real>(mX*invval, mY*invval, mZ*invval);
	}

	/** Divides this vector by a constant. */
	const vector3<Real>& operator/=(Real val)
	{
		Real invval = 1 / val;
		mX *= invval;
		mY *= invval;
		mZ *= invval;
		return *this;
	}

	/** Adds a constant to this vector. */
	const vector3<Real>& operator+=(Real val)
	{
		mX += val;
		mY += val;
		mZ += val;
		return *this;
	}

	/** Adds another vector to this vector. */
	const vector3<Real>& operator+=(const vector3<Real>& v)
	{
		mX += v.getX();
		mY += v.getY();
		mZ += v.getZ();
		return *this;
	}

	/** Subtracts some constant from the vector. */
	const vector3<Real>& operator-=(Real val)
	{
		mX -= val;
		mY -= val;
		mZ -= val;
		return *this;
	}

	/** Subtracts a vector from this vector. */
	const vector3<Real>& operator-=(const vector3<Real>& v)
	{
		mX -= v.getX();
		mY -= v.getY();
		mZ -= v.getZ();
		return *this;
	}

	/** Multiplies the current vector by a constant. */
	const vector3<Real>& operator*=(Real val)
	{
		mX *= val;
		mY *= val;
		mZ *= val;
		return *this;
	}

	/** Sets the coordinates of this vector to those of another vector. */
	const vector3<Real>& operator=(const vector3<Real> other)
	{
		memcpy(m_points, other.v(), 3*sizeof(Real));
		return *this;
	}

	/** Returns a textual representation of the vector3. */
	String toString(void) const
	{
		return String("vector3[ ") + mX + ", " + mY + ", " + mZ + " ]";
	}

	/** Calculates some hash value for the vector3. */
	static u32 hash(const vector3<Real>& key)
	{
		return static_cast<u32>(key.getX()+key.getY()+key.getZ());
	}

	static vector3<Real> load(Real val)
	{
		return vector3<Real>(val, val, val);
	}

	static Real get(const vector3<Real>& vla)
	{
		return vla.getX();
	}

private:
	union
	{
		Real m_points[3];   // the vertices
		struct
		{
			Real mX;
			Real mY;
			Real mZ;
		};
	};
	bool m_is_normalized; // Flag to set when the vector is normalized
};

// Useful typedefs
typedef vector3<f32> vector3f;
typedef vector3<f64> vector3d;

// Properly instantiate templates for building static libraries
template class _FIRE_ENGINE_API_ vector3<f32>;
template class _FIRE_ENGINE_API_ vector3<f64>;

} // namespace fire_engine

#endif // VECTOR3_H_INCLUDED
