/**
 *  $Id: Math.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Basic mathematical functions and constants
**/

#ifndef	__FEMATH_H
#define	__FEMATH_H

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"

namespace fire_engine
{

/** Mathematics class for the Fire Engine. It provides wrappers on all the
 mathematical functions used, and provides faster implementations of some of
 the computationally expensive ones, such as Cos/Sin/Tan and InvSq.
 Even though all the functions in Math are static, an instance must still be
 created for the search tables for Cos/Sin/Tan to be used. */
template <class Real>
class _FIRE_ENGINE_API_ Math
{
public:
	static const Real PI;
	static const Real PI_ON_2;
	static const Real PI_ON_3;
	static const Real PI_ON_4;
	static const Real PI_ON_6;
	static const Real TWO_PI;
	static const Real TO_RAD;
	static const Real TO_DEG;
	static const Real SQ2;
	static const Real ONE_ON_SQ2;
	static const Real SQ3;
	static const Real TWO_SQ3;
	static const Real EPSILON;
	static const Real ONE_OVER_TWO_FIVE_FIVE;

	/** Creates the singleton instance of the Math class if it does not already
	 exist, and returns it. */
	static Math<Real> * Create()
	{
		if (mInstance == 0)
			mInstance = new Math<Real>();
		return mInstance;
	}

	/** Returns the singleton instance of the Math class. */
	static Math<Real> * Get()
	{
		return mInstance;
	}

	/** Destructor. */
	~Math()
	{
		delete [] mSineTbl;
		delete [] mTanTbl;
	}

	/** Returns the cosine of a given angle in radians.
	 \param useTable Set this to true if the value should be approximated using a lookup table. */
	static Real Cos(Real angle, bool useTable = false)
	{
		if (useTable)
		{
			Real sin_angle = PI_ON_2 - angle;
			s32 index = ((s32) ((sin_angle * (1.0 /  TWO_PI)) * mTrigTblSize)) % mTrigTblSize;
			return (Real) mSineTbl[index];
		}
		else
		{
			return (Real) cos(angle);
		}
	}

	/** Returns the sine of a given angle in radians.
	 \param useTable Set this to true if the value should be approximated using a lookup table. */
	static Real Sin(Real angle, bool useTable = false)
	{
		if (useTable)
		{
			s32 index = ((s32) ((angle * (1.0 / TWO_PI)) * mTrigTblSize)) % mTrigTblSize;
			return (Real) mSineTbl[index];
		}
		else
		{
			return (Real) sin(angle);
		}
	}

	/** Returns the tangent of a given angle in radians.
	 \param useTable Set this to true if the value should be approximated using a lookup table. */
	static Real Tan(Real angle, bool useTable = false)
	{
		if (useTable)
		{
			s32 index = ((s32) ((angle * (1.0 /  TWO_PI)) * mTrigTblSize)) % mTrigTblSize;
			return (Real) mTanTbl[index];
		}
		else
		{
			return (Real) tan(angle);
		}
	}

	/** The Cotangent of an angle (1/tan). */
	static Real Cot(Real angle, bool useTable = false)
	{
		return static_cast<Real>(1.0/Tan(angle, useTable));
	}

	/** Returns the arc cos of an angle, in radians. */
	static Real Arccos(Real angle)
	{
		return (Real) acos(angle);
	}

	/** Returns the arc sin of an angle, in radians. */
	static Real Arcsin(Real angle)
	{
		return (Real) asin(angle);
	}

	/** Returns the arc tan of an angle, in radians. */
	static Real Arctan(Real angle)
	{
		return (Real) atan(angle);
	}

	/** Returns the floor of a given real number. */
	static s32 Floor(Real val)
	{
		return (s32) val;
	}

	/** Returns the ceiling of a real number. */
	static s32 Ceiling(Real val)
	{
		return ((s32) val) + 1;
	}

	/** Returns the absolute value of a given value. */
	static Real Abs(Real val)
	{
		return (Real) (val > 0) ? val : -val;
	}

	/** Returns the square root of a given calue. */
	static Real Sqrt(Real val)
	{
		return (Real) sqrt(val);
	}

	/** Fast approximation of the inverse square root function. */
	static Real FastInvSq(Real val);

	/** Fast approximation of the square root function. */
	static Real FastSq(Real val);

	/** Fast approximation of the cos function. */
	static Real FastCos(Real angle)
	{
		return Cos(angle, true);
	}

	/** Fast approximation of the sin function. */
	static Real FastSin(Real angle)
	{
		return Sin(angle, true);
	}

	/** Fast approximation of the tan function. */
	static Real FastTan(Real angle)
	{
		return Tan(angle, true);
	}

	//! Check whether two reals are 'equal'
	static bool Equals(Real a, Real b, Real eps = Math<Real>::EPSILON)
	{
		if (Abs(a - b) < eps)
			return true;
		else
			return false;
	}

	/** Calculates the binomial coefficient (n choose k). */
	static s32 Choose(s32 n, s32 k)
	{
		if (k < 0 || k > n)
			return 0;
		else if (n == k || n == 0)
			return 1;
		s32 fac = 1;
		// Speed things up - don't use silly method
		if (k > n - k)
		{
			for (s32 i = n; i > k; i--)
				fac *= i;
			return fac / Factorial(n-k);
		}
		else
		{
			for (s32 i = n; i > n-k; i--)
				fac *= i;
			return fac / Factorial(k);
		}
	}

	/** Returns the factorial of n. */
	static s32 Factorial(s32 n)
	{
		if (n < 0)
			return 0;
		s32 result = 1;
		while (n > 1)
			result *= n--;
		return result;
	}

	/** Raises a real to a real power. */
	static Real Pow(Real x, Real n)
	{
		return (Real)pow((f64)x, (f64)n);
	}

	/** Raises an real to an integer power. */
	static Real PowInt(Real x, s32 n)
	{
		Real result = 1.0;
		if (n < 0)
		{
			n = -n;
			x = 1 / x;
		}
		while (n-- > 0)
			result *= x;
		return result;
	}

	/** Sets the seed for the random-number generator. */
	static void Seed()
	{
		srand(static_cast<unsigned int>(time(0)));
	}

	/** Sets the seed for the random-number generator. */
	static void Seed(s32 s)
	{
		srand(s);
	}

	/** Returns a random value. */
	static s32 Rand()
	{
		return rand();
	}

	/** Returns a random value between min and max. */
	static Real Rand(f32 min, f32 max)
	{
		return static_cast<Real>(min + (f32) rand() / (RAND_MAX + 1.0) * (max - min));
	}

	/** Clamps a given value and the extremities. */
	static inline Real Clamp(Real value, Real min, Real max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		return value;
	}

private:
	static Real *		 mSineTbl;
	static Real *		 mTanTbl;
	static s32			 mTrigTblSize;
	static Math<Real> *  mInstance;

	/**
	 *	Initialize the class with a given table size
	 *	@param	tblSize	The size of the lookup tables.
	 *					A table size of 4096 is quite good. The values are
	 *					very close to the original ones, and they will only
	 *					only occupy 32Kb of memory. They should stay in the
	 *					cache, which should speed up operations alot.
	**/
	Math(s32 tblSize = 4096)
	{
		mTrigTblSize = tblSize;
		buildTrigTables();
	}

	/** Builds the lookup trigonometric tables. */
	void buildTrigTables()
	{
		mSineTbl = new Real[mTrigTblSize];
		mTanTbl  = new Real[mTrigTblSize];
		Real two_pi_times_one_over_tbl_size = TWO_PI * (1.0f / mTrigTblSize);
		Real angle, i_f;
		s32 i;
		for (i = 0, i_f = 0.0f; i < mTrigTblSize; i++, i_f += 1.0f)
		{
			angle = i_f * two_pi_times_one_over_tbl_size;
			mSineTbl[i] = (Real) sin(angle);
			mTanTbl[i]  = (Real) tan(angle);
		}
	}
};

typedef Math<f32> Math32;
typedef Math<f64> Math64;

template class _FIRE_ENGINE_API_ Math<f32>;
template class _FIRE_ENGINE_API_ Math<f64>;

} // namespace fire_engine

#endif
