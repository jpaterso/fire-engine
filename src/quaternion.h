/**
 * FILE:    quaternion.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: quaternion.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of a Quaternion.
**/

#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "matrix4.h"
#include "vector3.h"
#include "Math.h"
#include <String.h>

namespace fire_engine
{

// Forward declaration of the matrix4 class, to avoid header inclusion problems
template <class Real> class matrix4;

/** A Quaternion is an alternative way of representing a rotation in
 3-Dimensional space. It only needs 16 bytes of storage space (4 floats),
 instead of the 64 bytes needed by a 4x4 matrix.
 Quaternions are also handy when interpolation is needed. Spherical
 Linear Interpolation (SLERP) between two quaternions is easy to
 compute, and is relatively unexpensive. */
template <class Real>
class _FIRE_ENGINE_API_ quaternion
{
public:
	/** The identity quaternion, with W = 1, (X, Y, Z) = (0, 0, 0) */
	static const quaternion<Real> IDENTITY_QUATERNION;

	static const quaternion<Real> ZERO_QUATERNION;

	/** Construct a quaternion from it's four components. */
	quaternion(Real w = 1, Real x = 0, Real y = 0, Real z = 0)
	{
		m_points[0] = w;
		m_points[1] = x;
		m_points[2] = y;
		m_points[3] = z;
	}

	/** Extract a quaternion from a rotation matrix. */
	quaternion(const matrix4<Real>& mat)
	{
		*this = mat;
	}

	/** Construct a quaternion given another quaternion. */
	quaternion(const quaternion<Real>& other)
	{
		memcpy(m_points, other.m_points, 4*sizeof(Real));
	}

	/** Destructor. */
	~quaternion()
	{
	}

	/** Sets the four values of the quaternion. */
	inline void set(Real w, Real x, Real y, Real z)
	{
		m_points[0] = w; m_points[1] = x; m_points[2] = y; m_points[3] = z;
	}

	/** Returns a matrix representation of the quaternion. */
	matrix4<Real> getMatrix() const
	{
		return matrix4<Real>(1-2*(mY*mY+mZ*mZ), 2*(mX*mY-mW*mZ), 2*(mX*mZ+mW*mY), 0,
			2*(mX*mY+mZ*mW), 1-2*(mX*mX+mZ*mZ), 2*(mY*mZ-mX*mW), 0,
			2*(mX*mZ-mY*mW), 2*(mY*mZ+mX*mW), 1-2*(mX*mX+mY*mY), 0,
			0,               0,               0,                 1);
	}

	/** Returns the conjugate of the quaternion. */
	inline quaternion<Real> getConjugate() const
	{
		return quaternion(m_points[0], -m_points[1], -m_points[2], -m_points[3]);
	}

	/** Returns the inverse of the quaternion. */
	inline quaternion<Real> getInverse() const
	{
		return getConjugate() * (1 / (m_points[0]*m_points[0] + m_points[1]*m_points[1]
		+ m_points[2]*m_points[2] + m_points[3]*m_points[3]));
	}

	/** Returns the norm (ie. the length) of the quaternion. */
	inline Real norm() const
	{
		return Math<Real>::Sqrt(m_points[0]*m_points[0] + m_points[1]*m_points[1] +
			m_points[2]*m_points[2] + m_points[3]*m_points[3]);
	}

	/** Normalize the quaternion, if it has not already been done. */
	void normalize()
	{
		Real oneOverN = 1/norm();
		mW = mW*oneOverN;
		mX = mX*oneOverN;
		mY = mY*oneOverN;
		mZ = mZ*oneOverN;
	}

	/** Returns the quaternion representing the rotation necessary to go from
	 vector s to vector t. */
	static quaternion<Real> RotateVector(const vector3<Real>& s, const vector3<Real>& t)
	{
		quaternion<Real> retval;
		vector3<Real> u = s.cross(t);
		Real e = Math<Real>::Sqrt(2 + 2 * u.length());
		u.normalize();
		u /= e;
		return quaternion(e / 2, u.getX(), u.getY(), u.getZ());
	}

	/** Create a Quaternion that represents a rotation by an angle theta around
	 arbitrary axes. */
	void fromAngleAxis(const vector3<Real>& axis, Real theta)
	{
		Real sinAngle = Math<Real>::Sin(static_cast<Real>(0.5*theta));
		mW = Math<Real>::Cos(static_cast<Real>(0.5*theta));
		mX = sinAngle*axis.getX();
		mY = sinAngle*axis.getY();
		mZ = sinAngle*axis.getZ();
		this->normalize();
	}

	/** Builds an axis angle quaternion from an axis and an angle, given in radians. */
	static quaternion<Real> buildFromAngleAxisQuaternion(const vector3<Real>& axis, Real theta)
	{
		Real fSin = Math<Real>::Sin(static_cast<Real>(0.5*theta));
		return quaternion<Real>(Math<Real>::Cos(static_cast<Real>(0.5*theta)),
			fSin*axis.getX(), fSin*axis.getY(), fSin*axis.getZ());
	}

	/** Returns the result of spherical linear interpolation (SLERP) between this
	 quaternion and another. */
	quaternion<Real> slerp(const quaternion& other, Real time) const
	{
		Real dot_product = this->dot(other);
		quaternion<Real> other_right_way = other;
		if (dot_product < 0)
		{
			dot_product = -dot_product;
			other_right_way = other_right_way*-1;
		}
		if (dot_product >= 0.9995)
		{
			quaternion<Real> result = *this*(1-time) + other_right_way*time;
			result.normalize();
			return result;
		}
		else
		{
			Real theta = Math<Real>::Arccos(dot_product);
			return (*this*Math<Real>::Sin(theta*(1-time))+
				other_right_way*Math<Real>::Sin(theta*time))/Math<Real>::Sin(theta);
		}
	}

	/** Returns the W component of this quaternion. */
	inline Real getW(void) const
	{
		return mW;
	}

	/** Returns the X component of this quaternion. */
	inline Real getX(void) const
	{
		return mX;
	}

	/** Returns the Y component of this quaternion. */
	inline Real getY(void) const
	{
		return mY;
	}

	/** Returns the Z component of this quaternion. */
	inline Real getZ(void) const
	{
		return mZ;
	}

	/** Sets the W component of this quaternion. */
	inline void setW(Real w)
	{
		mW = w;
	}

	/** Sets the X component of this quaternion. */
	inline void setX(Real x)
	{
		mX = x;
	}

	/** Sets the Y component of this quaternion. */
	inline void setY(Real y)
	{
		mY = y;
	}

	/** Sets the Z component of this quaternion. */
	inline void setZ(Real z)
	{
		mZ = z;
	}

	/** Returns the dot product of this quaternion and another. */
	inline Real dot(const quaternion<Real>& rhs) const
	{
		return mW*rhs.getW() + mX*rhs.getX() + mY*rhs.getY() + mZ*rhs.getZ();
	}

	/** Assign a new quaternion to this quaternion. */
	inline const quaternion<Real>& operator=(const quaternion<Real>& r)
	{
		memcpy(m_points, r.m_points, 4*sizeof(Real));
		return *this;
	}

	/** Extract a quaternion from a rotation matrix. */
	const quaternion<Real>& operator=(const matrix4<Real>& mat)
	{
		Real s;
		Real trace = mat.getTrace();
		if (trace > 0)
		{
			s  = 1/(2*Math<Real>::Sqrt(trace));
			mW = (Real)0.25/s;
			mX = (mat(2, 1)-mat(1, 2))*s;
			mY = (mat(0, 2)-mat(2, 0))*s;
			mZ = (mat(1, 0)-mat(0, 1))*s;
		}
		else
		{
			if (mat(0, 0) > mat(1, 1) && mat(0, 0) > mat(2, 2))
			{
				s  = 1/(Math<Real>::Sqrt(1+mat(0, 0)-mat(1, 1)-mat(2, 2))*2);
				mW = (Real)0.25/s;
				mX = (mat(0, 1)+mat(1, 0))*s;
				mY = (mat(2, 0)+mat(0, 2))*s;
				mZ = (mat(2, 1)+mat(1, 2))*s;
			}
			else if (mat(1, 1) > mat(2, 2))
			{
				s  = 1/(Math<Real>::Sqrt(1+mat(1, 1)-mat(0, 0)-mat(2, 2))*2);
				mW = (mat(0, 2)-mat(2, 0))*s;
				mX = (mat(0, 1)+mat(1, 0))*s;
				mY = (Real)0.25/s;
				mZ = (mat(1, 2)+mat(2, 1))*s;
			}
			else
			{
				s  = 1/(Math<Real>::Sqrt(1+mat(2, 2)-mat(0, 0)-mat(1, 1))*2);
				mW = (mat(1, 0)-mat(0, 1))*s;
				mX = (mat(2, 0)+mat(0, 2))*s;
				mY = (mat(1, 2)+mat(2, 1))*s;
				mZ = (Real)0.25/s;
			}
		}
		normalize();
		return *this;
	}

	/** Returns whether two quaternions are equal. */
	inline bool operator==(const quaternion<Real>& other) const
	{
		return memcmp(m_points, other.m_points, 4*sizeof(Real)) == 0;
	}

	/** Returns whether two quaternions are different. */
	inline bool operator!=(const quaternion<Real>& other) const
	{
		return memcmp(m_points, other.m_points, 4*sizeof(Real)) != 0;
	}

	/** Returns the result of adding a quaternion to the current quaternion. */
	inline quaternion<Real> operator+(const quaternion<Real>& other) const
	{
		return quaternion<Real>(mW+other.getW(), mX+other.getX(), mY+other.getY(), mZ+other.getZ());
	}


	/** Returns the result of subtracting a quaternion from the current quaternion. */
	inline quaternion<Real> operator-(const quaternion<Real>& other) const
	{
		return quaternion<Real>(mW-other.getW(), mX-other.getX(), mY-other.getY(), mZ-other.getZ());
	}

	/** Returns the result of multiplying the quaternion by a scalar. */
	inline quaternion<Real> operator*(Real scalar) const
	{
		return quaternion<Real>(scalar*mW, scalar*mX, scalar*mY, scalar*mZ);
	}

	/** Returns the result of dividing the quaternion by a scalar. */
	inline quaternion<Real> operator/(Real scalar) const
	{
		Real invs = 1/scalar;
		return quaternion<Real>(invs*mW, invs*mX, invs*mY, invs*mZ);
	}

	/** Returns the result of multiplying this quaternion by another quaternion. */
	quaternion<Real> operator*(const quaternion<Real>& r) const
	{
		Real w = mW*r.getW() - mX*r.getX() - mY*r.getY() - mZ*r.getZ();
		Real x = mW*r.getX() + mX*r.getW() + mY*r.getZ() - mZ*r.getY();
		Real y = mW*r.getY() + mY*r.getW() + mZ*r.getX() - mX*r.getZ();
		Real z = mW*r.getZ() + mZ*r.getW() + mX*r.getY() - mY*r.getX();
		return quaternion<Real>(w, x, y, z);
	}

	/** Multiplies this quaternion by a scalar. */
	const quaternion<Real>& operator*=(Real scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	/** Multiplies this quaternion by another quaternion. */
	const quaternion<Real>& operator*=(const quaternion<Real>& r)
	{
		*this = *this * r;
		return *this;
	}

	/** Adds another quaternion to this quaternion. */
	const quaternion<Real>& operator+=(const quaternion<Real>& r)
	{
		*this = *this + r;
		return *this;
	}

private:
	union
	{
		Real m_points[4];
		struct
		{
			Real mW;
			Real mX;
			Real mY;
			Real mZ;
		};
	};
};

typedef quaternion<f32> quaternionf;
typedef quaternion<f64> quaterniond;

template class _FIRE_ENGINE_API_ quaternion<f32>;
template class _FIRE_ENGINE_API_ quaternion<f64>;

}

#endif // QUATERNION_H_INCLUDED
