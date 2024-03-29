/**
 * FILE:    aabbox.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: aabbox.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An Axis-Aligned Bounding Box, used for collision detection
 *          between objects.
**/

#ifndef AABBOX_H_INCLUDED
#define AABBOX_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "String.h"

namespace fire_engine
{

/** An Axis-Aligned Bounding Box, useful for collision detection between
 objects. */
template <class Real>
class _FIRE_ENGINE_API_ aabbox
{
public:
	/** Default constructor. */
	aabbox()
		: mMinPoint(0, 0, 0), mMaxPoint(0, 0, 0)
	{
	}

	/** Construct an Axis-Aligned Bounding Box from it's minimum point and
	 it's maximum point. */
	aabbox(const vector3<Real>& min, const vector3<Real>& max)
		: mMinPoint(min), mMaxPoint(max)
	{
	}

	/** Destructor. */
	~aabbox()
	{
	}

	/** Add a new point to the box, and recompute it's extremities. */
	void addInternalPoint(const vector3<Real>& point)
	{
		if (point.getX() < mMinPoint.getX())
			mMinPoint.setX(point.getX());
		else if (point.getX() > mMaxPoint.getX())
			mMaxPoint.setX(point.getX());

		if (point.getY() < mMinPoint.getY())
			mMinPoint.setY(point.getY());
		else if (point.getY() > mMaxPoint.getY())
			mMaxPoint.setY(point.getY());

		if (point.getZ() < mMinPoint.getZ())
			mMinPoint.setZ(point.getZ());
		else if (point.getZ() > mMaxPoint.getZ())
			mMaxPoint.setZ(point.getZ());
	}

	/** Add a new internal bounding box, and recompute the extremities. */
	void addInternalBoundingBox(const aabbox<Real>& box)
	{
		addInternalPoint(box.getMinPoint());
		addInternalPoint(box.getMaxPoint());
	}

	/** Returns whether this box intersects with another box. */
	bool intersectsWith(const aabbox<Real>& box) const;

	/** Returns a textual representation of the box. */
	String toString() const
	{
		return String("aabbox[ ") + mMinPoint.toString() + "\n               " + mMaxPoint.toString() + " ]";
	}

	/** Returns the minimum point of the box. */
	inline const vector3<Real>& getMinPoint() const
	{
		return mMinPoint;
	}

	/** Returns the maximum point of the box. */
	inline const vector3<Real>& getMaxPoint() const
	{
		return mMaxPoint;
	}

	/** Returns the 8 corners of the bounding box. */
	vector3<Real> * getCorners() const
	{
		vector3<Real> * corners = new vector3<Real>[8];
		getCorners(corners);
		return corners;
	}

	/** Sets the 8 corners of the bounding box in a pre-allocated array of points. */
	void getCorners(vector3<Real> * corners) const
	{
		vector3<Real> center = (mMinPoint+mMaxPoint)/2;
		vector3<Real> diag = mMaxPoint-center;

		corners[0] = vector3<Real>(center.getX()-diag.getX(), center.getY()-diag.getY(), center.getZ()-diag.getZ());
		corners[1] = vector3<Real>(center.getX()-diag.getX(), center.getY()-diag.getY(), center.getZ()+diag.getZ());
		corners[2] = vector3<Real>(center.getX()-diag.getX(), center.getY()+diag.getY(), center.getZ()-diag.getZ());
		corners[3] = vector3<Real>(center.getX()-diag.getX(), center.getY()+diag.getY(), center.getZ()+diag.getZ());
		corners[4] = vector3<Real>(center.getX()+diag.getX(), center.getY()-diag.getY(), center.getZ()-diag.getZ());
		corners[5] = vector3<Real>(center.getX()+diag.getX(), center.getY()-diag.getY(), center.getZ()+diag.getZ());
		corners[6] = vector3<Real>(center.getX()+diag.getX(), center.getY()+diag.getY(), center.getZ()-diag.getZ());
		corners[7] = vector3<Real>(center.getX()+diag.getX(), center.getY()+diag.getY(), center.getZ()+diag.getZ());
	}

	/** Returns the result of linear interpolating this box with another box. */
	aabbox<Real> getInterpolate(const aabbox<Real>& box, Real time) const
	{
		return aabbox<Real>(mMinPoint*(1-time)+box.getMinPoint()*time,
			mMaxPoint*(1-time)+box.getMaxPoint()*time);
	}

	/** Resets the aabbox, with the min and max points set to the given values.
	 \param min The new minimum point.
	 \param max The new maximum point. */
	void reset(const vector3<Real>& min = vector3<Real>(0, 0, 0),
		const vector3<Real>& max = vector3<Real>(0, 0, 0))
	{
		mMinPoint = min;
		mMaxPoint = max;
	}

	/** Checks the integrity of the bounding box, and repairs the min point and max point. */
	void repair()
	{
		Real temp;
		if (mMinPoint.getX() > mMaxPoint.getX())
		{
			temp = mMinPoint.getX();
			mMinPoint.setX(mMaxPoint.getX());
			mMaxPoint.setX(temp);
		}
		if (mMinPoint.getY() > mMaxPoint.getY())
		{
			temp = mMinPoint.getY();
			mMinPoint.setY(mMaxPoint.getY());
			mMaxPoint.setY(temp);
		}
		if (mMinPoint.getZ() > mMaxPoint.getZ())
		{
			temp = mMinPoint.getZ();
			mMinPoint.setZ(mMaxPoint.getZ());
			mMaxPoint.setZ(temp);
		}
	}

	/** Returns the center point of the bounding box. */
	vector3<Real> getCenter() const
	{
		return (mMinPoint+mMaxPoint)/2;
	}

	/** Check whether a given point is inside the bounding box.
	 \param point The point to look at.
	 \return Whether the point is inside the bounding box. */
	bool contains(const vector3<Real>& point)
	{
		if ((point.getX() >= mMinPoint.getX() && point.getX() <= mMaxPoint.getX()) &&
			(point.getY() >= mMinPoint.getY() && point.getY() <= mMaxPoint.getY()) &&
			(point.getZ() >= mMinPoint.getZ() && point.getZ() <= mMaxPoint.getZ()))
		{
			return true;
		}
		return false;
	}

private:
	vector3<Real> mMinPoint;
	vector3<Real> mMaxPoint;
};

typedef aabbox<f32> aabboxf;
typedef aabbox<f64> aabboxd;


}

#endif // AABBOX_H_INCLUDED
