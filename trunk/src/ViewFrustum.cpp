/**
* FILE:    ViewFrustum.cpp
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: ViewFrustum.cpp 105 2007-09-11 05:08:21Z jpaterso $
* PURPOSE: A view frustum, defining a visible volume in 3-dimensional space.
**/

#include "ViewFrustum.h"
#include "Math.h"

namespace fire_engine
{

ViewFrustum::ViewFrustum()
{
}

ViewFrustum::ViewFrustum(const plane3f& near, const plane3f& far, const plane3f& top,
	const plane3f& bottom, const plane3f& left, const plane3f& right)
{
	mPlanes[EFP_NEAR] = near;
	mPlanes[EFP_FAR] = far;
	mPlanes[EFP_TOP] = top;
	mPlanes[EFP_BOTTOM] = bottom;
	mPlanes[EFP_LEFT] = left;
	mPlanes[EFP_RIGHT] = right;
}

ViewFrustum::ViewFrustum(const plane3f * planes)
{
	for (s32 i = 0; i < EFP_PLANECOUNT; i++)
		mPlanes[i] = planes[i];
}

ViewFrustum::~ViewFrustum()
{
}

EFRUSTUM_INTERSECTION_TYPE ViewFrustum::calculateIntersection(const AABoundingBoxf& box) const
{
	s32 totalInCount = 0;
	s32 inCount, allPointsIn;
	vector3f points[8];
	box.getCorners(points);

	for (s32 i = 0; i < EFP_PLANECOUNT; i++)
	{
		inCount = 8;
		allPointsIn = 1;
		for (s32 j = 0; j < 8; j++)
		{
			if (mPlanes[i].getSideOfPlane(points[j]) != ESOP_ABOVE)
			{
				allPointsIn = 0;
				inCount--;
			}
		}
		if (inCount == 0)
			return EFIT_OUTSIDE;
		totalInCount += allPointsIn;
	}
	if (totalInCount == EFP_PLANECOUNT)
		return EFIT_INSIDE;
	return EFIT_INTERSECT;
}

}
