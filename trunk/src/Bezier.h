/**
 * FILE:    Bezier.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Bezier.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Bezier class, generating Bezier curves
**/

#ifndef BEZIER_H_INCLUDED
#define BEZIER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Array.h"
#include "vector3.h"

namespace fire_engine
{

class _FIRE_ENGINE_API_ Bezier
{
	public:
		/**
		 * Construct a Bézier curve
		 * @param cpoints   The control points.
		 * @param numpoints The number of points to generate.
		 * @return          An Array containing numpoints, interpolated from the control points.
		**/
		template <class T>
		static Array< vector3<T> > *
		GenerateBezierCurve(const Array< vector3<T> >& cpoints, s32 numpoints);

	private:
		/**
		 * Generate a point on a Bézier curve, given a certain time.
		 * @param cpoints The control points to use.
		 * @param time    The time at which the points is generated.
		 *                Must have 0.0 <= time <= 1.0 .
		 * @return        The point, interpolated between the control points at t = time.
		**/
		template <class T>
		static vector3<T>
		PointOnCurve(const Array< vector3<T> >& cpoints, T time);
};

template <class T>
Array< vector3<T> > *
Bezier::GenerateBezierCurve(const Array< vector3<T> >& cpoints, s32 numpoints)
{
	Array< vector3<T> > * points = new Array< vector3<T> >(numpoints);
	T time = 0.0;
	T time_increment = 1.0 / numpoints;
	while (time < 1.0)
	{
		points->push_back(PointOnCurve(cpoints, time));
		time += time_increment;
	}
	return points;
}

template <class T>
vector3<T>
Bezier::PointOnCurve(const Array< vector3<T> >& cpoints, T time)
{
	vector3<T> point(0.0, 0.0, 0.0);
	T factor = Math<T>::PowInt(1.0-time, cpoints.size());;
	for (s32 i = 0; i < cpoints.size(); i++)
	{
		point  += cpoints.at(i) * factor;
		factor *= (time * (cpoints.size()-i)) / ((1.0 - time) * (i + 1));
	}
	return point;
}

}
#endif
