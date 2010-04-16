/*
 *  $Id: plane3.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the plane3 class, with intersection and collision
 *	functions.
**/

#ifndef	__PLANE3_H
#define	__PLANE3_H

#include "Types.h"
#include "CompileConfig.h"
#include "line3.h"
#include "vector3.h"
#include "String.h"
#include "Math.h"

namespace fire_engine
{

/** Where a point is in relation to a plane. */
enum ESIDE_OF_PLANE
{
	ESOP_ABOVE,
	ESOP_BEHIND,
	ESOP_ON
};

template <class T>
class _FIRE_ENGINE_API_ plane3 {
public:
	/**
	 *	Default constructor
	**/
	plane3()
		: mPoint(0, 0, 0), mNormal(0, 0, 0)
	{
	}

	/**
	 *	Construct a plane from a point and a normal
	 *	@param	pt		A point on the plane
	 *	@param	normal	The normal to the plane
	**/
	plane3 (const vector3<T>& pt, const vector3<T>& normal)
		: mPoint(pt), mNormal(normal)
	{
		mNormal.normalize();
	}

	/**
	 *	Construct a plane from 3 points
	 *	@param	pt1	The first point
	 *	@param	pt2	The second point
	 *	@param	pt3	The third version
	**/
	plane3(const vector3<T>& pt1, const vector3<T>& pt2, const vector3<T>& pt3)
		: mPoint(pt1)
	{
		mNormal = (pt2 - pt1).cross(pt3 - pt1);
		mNormal.normalize();
	}

	/** Construct a plane from a normal, and the distance of the plane to the
	 origin along the normal. 
	 \param distance The distance of the plane to the origin along the normal
	                 to the plane.
	 \param normal   The normal to the plane. */
	plane3(T distance, const vector3<T>& normal)
		: mNormal(normal)
	{
		mPoint = normal * distance;
	}

	/**
	 *	~plane3 ()
	 *		Basic destructor, does nothing
	**/
	~plane3(void)
	{
	}

	/**
	 *	Construct a plane from a point and the normal to the plane
	 *	@param	pt		A point on the plane
	 *	@param	normal	The normal to the plane
	**/
	void set(const vector3<T>& pt, const vector3<T>& normal)
	{
		mPoint = pt;
		mNormal = normal;
		mNormal.normalize();
	}

	/**
	 *	Construct a plane from 3 points
	 *	@param	pt1	The first point
	 *	@param	pt2	The second point
	 *	@param	pt3	The third version
	**/
	void set(const vector3<T>& pt1, const vector3<T>& pt2, const vector3<T>& pt3)
	{
		mPoint = pt1;
		mNormal = (pt2-pt1).cross(pt3-pt1);
		mNormal.normalize();
	}

	/**
	 *	Getter for the point on the plane
	 *	@return	A reference to the second vector defining the plane
	**/
	const vector3<T>& getPoint()
	{
		return mPoint;
	}

	/**
	 *	Getter for the normal to the plane
	 *	@return	A reference to the normal to the plane
	**/
	const vector3<T>& getNormal()
	{
		return mNormal;
	}

	/**
	 *	Check whether a given point is on the current plane
	 *	@param	point	The point to check
	 *	@return			true if the point is on the plane, false otherwise
	**/
	bool isPointOnPlane(const vector3<T>& point) const
	{
		if (Math<T>::Equals((point - mPoint).dot(mNormal), (T) 0.0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/** Returns the distance from this plane to a given point. */
	inline T distanceFrom(const vector3<T>& point) const
	{
		return mNormal.dot(point-mPoint);
	}

	/** Returns where a point is in relation to this plane. */
	inline ESIDE_OF_PLANE getSideOfPlane(const vector3<T>& point) const
	{
		T distance = this->distanceFrom(point);
		if (distance > 0)
		{
			return ESOP_ABOVE;
		}
		else if (distance < 0)
		{
			return ESOP_BEHIND;
		}
		else
		{
			return ESOP_ON;
		}
	}

	/**
	 *	Find whether a line intersects with a plane
	 *	@param	l		The line to look at
	 *	@param	store	A place to store the intersection point, if it
	 *					exists
	 *	@return			true if a point intersection exists, false otherwise
	**/
	bool intersectWithLine(const line3<T>& l, vector3<T>& store) const
	{
		T a, d;
		T t;

		a = mNormal.dot(mPoint - l.point());
		d = mNormal.dot(l.direction());

		// If the direction is perpendicular to the normal,
		// the line is either _omNormal the plane, or parallel to the plane
		if (Math<T>::Equals((T) d, (T) 0.0)) {
			return (false);
		}
		else {
			t = a / d;
			store =  l.point() + l.direction() * t;
			return (true);
		}
	}

	String toString(void) const
	{
		return String("plane3[ ") + mPoint.toString() + ",\n        " +
			mNormal.toString() + " ]";
	}

private:
	//! A point on the plane
	vector3<T> mPoint;
	//! The normal to the plane
	vector3<T> mNormal;
};
// Useful typedefs
typedef plane3<f32> plane3f;
typedef plane3<f64> plane3d;

template class _FIRE_ENGINE_API_ plane3<f32>;
template class _FIRE_ENGINE_API_ plane3<f64>;

} // namespace fire_engine

#endif
