/**
* FILE:    ViewFrustum.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: ViewFrustum.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: A view frustum, defining a visible volume in 3-dimensional space.
**/

#ifndef VIEWFRUSTUM_H_INCLUDED
#define VIEWFRUSTUM_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "plane3.h"
#include "aabbox.h"

namespace fire_engine
{

/** The six planes that make up a view frustum. */
enum EFRUSTRUM_PLANE
{
	EFP_NEAR = 0x00,
	EFP_FAR,
	EFP_TOP,
	EFP_BOTTOM,
	EFP_LEFT,
	EFP_RIGHT,
	EFP_PLANECOUNT
};

/** What we can return when checking the intersection with an axis-aligned
 bounding box. */
enum EFRUSTUM_INTERSECTION_TYPE
{
	EFIT_INSIDE,
	EFIT_OUTSIDE,
	EFIT_INTERSECT
};

/** The ViewFrustum defines the visible volume in 3-dimensional space. Anything outside of
 the viewing frustum can be safely clipped, which leads to higher performance. */
class _FIRE_ENGINE_API_ ViewFrustum
{
public:
	/** Default constructor. */
	ViewFrustum();

	/** Construct a frustum from the 6 delimiting planes. */
	ViewFrustum(const plane3f& near, const plane3f& far, const plane3f& top,
		const plane3f& bottom, const plane3f& left, const plane3f& right);

	/** Construct a frustum from an array containing it's six delimiting planes. */
	ViewFrustum(const plane3f * planes);

	/** Destructor. */
	virtual ~ViewFrustum();

	/** Sets one of the planes for the frustum. */
	inline void setPlane(EFRUSTRUM_PLANE p, const plane3f& plane);

	/** Returns the desired plane in the frustum. */
	inline const plane3f& getPlane(EFRUSTRUM_PLANE plane) const;

	/** Calculates the type of intersection between the frustum and a given axis-aligned
	 bounding box. */
	EFRUSTUM_INTERSECTION_TYPE calculateIntersection(const aabboxf& box) const;

protected:
	plane3f  mPlanes[EFP_PLANECOUNT];
};

inline void ViewFrustum::setPlane(EFRUSTRUM_PLANE p, const plane3f& plane)
{
	mPlanes[p] = plane;
}

inline const plane3f& ViewFrustum::getPlane(EFRUSTRUM_PLANE plane) const
{
	return mPlanes[plane];
}

}

#endif // VIEWFRUSTUM_H_INCLUDED
