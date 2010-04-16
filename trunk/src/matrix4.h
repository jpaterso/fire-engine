/**
 * FILE:    matrix4.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: matrix4.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: The matrix4 class is the basis for all the 3-dimensional
 *          transformations in fire engine.
**/

#ifndef MATRIX4_H_INCLUDED
#define MATRIX4_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "Math.h"
#include "String.h"
#include "quaternion.h"
#include "aabbox.h"

namespace fire_engine
{

// Forward declaration of the quaternion class, to avoid header inclusion problems
template <class Real> class quaternion;

/** A class for storing 4x4 matrices. The information is laid out
 in row-major format:
 The following matrix:
 A00 A01 A02 A03
 A10 A11 A12 A13
 A20 A21 A22 A23
 A30 A31 A32 A33
 will be stored as A00 A01 A02 A03 ... A30 A31 A32 A33 in memory. */

template <class Real>
class _FIRE_ENGINE_API_ matrix4
{
public:
	/** The identity matrix, with 1s in the descending left-to-right diagonal,
	 and 0s elsewhere. */
	static const matrix4 IDENTITY_MATRIX;

	/** Initializes a matrix4 to the values of another matrix, which is
	 the identity matrix by default. */
	matrix4(const matrix4<Real>& other = IDENTITY_MATRIX)
	{
		memcpy(mMatrix, other.v(), 16*sizeof(Real));
	}

	/** Construct a matrix4 from all its components. */
	matrix4(Real m00, Real m01, Real m02, Real m03,
			Real m10, Real m11, Real m12, Real m13,
			Real m20, Real m21, Real m22, Real m23,
			Real m30, Real m31, Real m32, Real m33)
	{
		mMatrix[0]  = m00; mMatrix[1]  = m01; mMatrix[2]  = m02;
		mMatrix[3]  = m03; mMatrix[4]  = m10; mMatrix[5]  = m11;
		mMatrix[6]  = m12; mMatrix[7]  = m13; mMatrix[8]  = m20;
		mMatrix[9]  = m21; mMatrix[10] = m22; mMatrix[11] = m23;
		mMatrix[12] = m30; mMatrix[13] = m31; mMatrix[14] = m32;
		mMatrix[15] = m33;
	}

	/** Construct a matrix4 from values laid out in memory.
	 \param values      The 16 values to read in
	 \param colum_major Set this to true if the information is laid out in
	                    column major format: A00 A10 A20 A30 ... A03 A13 A23 A33 */
	matrix4(Real * values, bool colum_major = false)
	{
		memcpy(mMatrix, values, 16*sizeof(Real));
		if (colum_major)
			transpose();
	}

	/** Destructor */
	~matrix4()
	{
	}

	/** Transposes the current matrix, swapping elements at [i, j] to [j, i]. */
	void transpose()
	{
		for (s32 i=0; i<4; i++)
		{
			for (s32 j = 0; j < i; j++)
			{
				Real tmp = _mMatrix[i][j];
				_mMatrix[i][j] = _mMatrix[j][i];
				_mMatrix[j][i] = tmp;
			}
		}
	}

	/** Returns the transpose of the matrix. */
	matrix4<Real> getTranspose() const
	{
		u32 i, j;
		matrix4<Real> r;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[j][i];
		return r;
	}

	/** Loads the identity matrix into the matrix. */
	void loadIdentity()
	{
		*this = IDENTITY_MATRIX;
	}

	/** Loads a scaling matrix into the current matrix.
	 \param s The vector to scale the matrix by. */
	void loadScale(const vector3<Real>& s)
	{
		loadIdentity();
		mMatrix[0]  = s.getX();
		mMatrix[5]  = s.getY();
		mMatrix[10] = s.getZ();
	}

	/** Scales the matrix by a vector.
	 \param s The vector to scale the matrix by. */
	void scale(const vector3<Real>& s)
	{
		matrix4<Real> tmp;
		tmp.loadScale(s);
		*this = tmp * *this;
	}

	/** Loads a translation matrix.
	 \param s The translation that this matrix should represent. */
	void loadTranslation(const vector3<Real>& s)
	{
		loadIdentity();
		mMatrix[3]  = s.getX();
		mMatrix[7]  = s.getY();
		mMatrix[11] = s.getZ();
	}

	/** Translate the current matrix by a vector.
	 \param s The translation to add to the current vector. */
	void translate(const vector3<Real>& s)
	{
		_mMatrix[0][3] += s.getX();
		_mMatrix[1][3] += s.getY();
		_mMatrix[2][3] += s.getZ();
	}

	/** Sets the translation that this matrix represents. */
	void setTranslation(const vector3<Real>& tr)
	{
		_mMatrix[0][3] = tr.getX();
		_mMatrix[1][3] = tr.getY();
		_mMatrix[2][3] = tr.getZ();
	}

	/** Loads a matrix4 from 3x3 data, making it a homogeneous matrix.
	 \param data         A pointer to the data.
	 \param column_major Set this to true if the data is in column major
	                     format. */
	void loadFrom3x3(const Real * data, bool column_major = false)
	{
		loadIdentity();
		if (!column_major)
		{
			memcpy(&_mMatrix[0][0], &data[0], 3*sizeof(Real));
			memcpy(&_mMatrix[1][0], &data[3], 3*sizeof(Real));
			memcpy(&_mMatrix[2][0], &data[6], 3*sizeof(Real));
		}
		else
		{
			_mMatrix[0][0] = data[0];
			_mMatrix[1][0] = data[1];
			_mMatrix[2][0] = data[2];

			_mMatrix[0][1] = data[3];
			_mMatrix[1][1] = data[4];
			_mMatrix[2][1] = data[5];

			_mMatrix[0][2] = data[6];
			_mMatrix[1][2] = data[7];
			_mMatrix[2][2] = data[8];
		}
	}

	/** Builds a matrix4 from 3x3 data.
	 \param data         A pointer to the data.
	 \param column_major Set this to true if the data is stored in column major
	                     format. */
	static matrix4<Real> BuildFrom3x3(const Real * data, bool column_major = false)
	{
		matrix4<Real> result;
		result.loadFrom3x3(data, column_major);
		return result;
	}

	/** Returns the determinant of the matrix. */
	Real determinant() const
	{
		Real d = (mMatrix[0] * mMatrix[5] - mMatrix[4] * mMatrix[1]) *
			(mMatrix[10] * mMatrix[15] - mMatrix[14] * mMatrix[11])	-
			(mMatrix[0] * mMatrix[9] - mMatrix[8] * mMatrix[1]) *
			(mMatrix[6] * mMatrix[15] - mMatrix[14] * mMatrix[7]) +
			(mMatrix[0] * mMatrix[13] - mMatrix[12] * mMatrix[1]) *
			(mMatrix[6] * mMatrix[11] - mMatrix[10] * mMatrix[7]) +
			(mMatrix[4] * mMatrix[9] - mMatrix[8] * mMatrix[5]) *
			(mMatrix[2] * mMatrix[15] - mMatrix[14] * mMatrix[3]) -
			(mMatrix[4] * mMatrix[13] - mMatrix[12] * mMatrix[5]) *
			(mMatrix[2] * mMatrix[11] - mMatrix[10] * mMatrix[3]) +
			(mMatrix[8] * mMatrix[13] - mMatrix[12] * mMatrix[9]) *
			(mMatrix[2] * mMatrix[7] - mMatrix[6] * mMatrix[3]);
		return d;
	}

	/** Calculates the inverse of the matrix.
	 \param s A place to store the calculated inverse.
	 \return  Whether the inverse could be calculated or not. */
	bool getInverse(matrix4& s) const
	{
		Real d = determinant();
		if (d == 0)
			return (false);
		d = 1.0f / d;

		s(0, 0) = d * (mMatrix[5] * (mMatrix[10] * mMatrix[15] -
			mMatrix[14] * mMatrix[11]) +
			mMatrix[9] * (mMatrix[14] * mMatrix[7] -
			mMatrix[6] * mMatrix[15]) + mMatrix[13] *
			(mMatrix[6] * mMatrix[11] - mMatrix[10] * mMatrix[7]));
		s(1, 0) = d * (mMatrix[6] * (mMatrix[8] * mMatrix[15] -
			mMatrix[12] * mMatrix[11]) +
			mMatrix[10] * (mMatrix[12] * mMatrix[7] -
			mMatrix[4] * mMatrix[15]) + mMatrix[14] *
			(mMatrix[4] * mMatrix[11] - mMatrix[8] * mMatrix[7]));
		s(2, 0) = d * (mMatrix[7] * (mMatrix[8] * mMatrix[13] -
			mMatrix[12] * mMatrix[9]) +
			mMatrix[11] * (mMatrix[12] * mMatrix[5] -
			mMatrix[4] * mMatrix[13]) + mMatrix[15] *
			(mMatrix[4] * mMatrix[9] - mMatrix[8] * mMatrix[5]));
		s(3, 0) = d * (mMatrix[4] * (mMatrix[13] * mMatrix[10] -
			mMatrix[9] * mMatrix[14]) +
			mMatrix[8] * (mMatrix[5] * mMatrix[14] -
			mMatrix[13] * mMatrix[6]) + mMatrix[12] *
			(mMatrix[9] * mMatrix[6] - mMatrix[5] * mMatrix[10]));
		s(0, 1) = d * (mMatrix[9] * (mMatrix[2] * mMatrix[15] -
			mMatrix[14] * mMatrix[3]) +
			mMatrix[13] * (mMatrix[10] * mMatrix[3] -
			mMatrix[2] * mMatrix[11]) + mMatrix[1] *
			(mMatrix[14] * mMatrix[11] - mMatrix[10] * mMatrix[15]));
		s(1, 1) = d * (mMatrix[10] * (mMatrix[0] * mMatrix[15] -
			mMatrix[12] * mMatrix[3]) +
			mMatrix[14] * (mMatrix[8] * mMatrix[3] -
			mMatrix[0] * mMatrix[11]) + mMatrix[2] *
			(mMatrix[12] * mMatrix[11] - mMatrix[8] * mMatrix[15]));
		s(2, 1) = d * (mMatrix[11] * (mMatrix[0] * mMatrix[13] -
			mMatrix[12] * mMatrix[1]) +
			mMatrix[15] * (mMatrix[8] * mMatrix[1] -
			mMatrix[0] * mMatrix[9]) + mMatrix[3] *
			(mMatrix[12] * mMatrix[9] - mMatrix[8] * mMatrix[13]));
		s(3, 1) = d * (mMatrix[8] * (mMatrix[13] * mMatrix[2] -
			mMatrix[1] * mMatrix[14]) +
			mMatrix[12] * (mMatrix[1] * mMatrix[10] -
			mMatrix[9] * mMatrix[2]) + mMatrix[0] *
			(mMatrix[9] * mMatrix[14] - mMatrix[13] * mMatrix[10]));
		s(0, 2) = d * (mMatrix[13] * (mMatrix[2] * mMatrix[7] -
			mMatrix[6] * mMatrix[3]) +
			mMatrix[1] * (mMatrix[6] * mMatrix[15] -
			mMatrix[14] * mMatrix[7]) + mMatrix[5] *
			(mMatrix[14] * mMatrix[3] - mMatrix[2] * mMatrix[15]));
		s(1, 2) = d * (mMatrix[14] * (mMatrix[0] * mMatrix[7] -
			mMatrix[4] * mMatrix[3]) +
			mMatrix[2] * (mMatrix[4] * mMatrix[15] -
			mMatrix[12] * mMatrix[7]) + mMatrix[6] *
			(mMatrix[12] * mMatrix[3] - mMatrix[0] * mMatrix[15]));
		s(2, 2) = d * (mMatrix[15] * (mMatrix[0] * mMatrix[5] -
			mMatrix[4] * mMatrix[1]) +
			mMatrix[3] * (mMatrix[4] * mMatrix[13] -
			mMatrix[12] * mMatrix[5]) + mMatrix[7] *
			(mMatrix[12] * mMatrix[1] - mMatrix[0] * mMatrix[13]));
		s(3, 2) = d * (mMatrix[12] * (mMatrix[5] * mMatrix[2] -
			mMatrix[1] * mMatrix[6]) +
			mMatrix[0] * (mMatrix[13] * mMatrix[6] -
			mMatrix[5] * mMatrix[14]) + mMatrix[4] *
			(mMatrix[1] * mMatrix[14] - mMatrix[13] * mMatrix[2]));
		s(0, 3) = d * (mMatrix[1] * (mMatrix[10] * mMatrix[7] -
			mMatrix[6] * mMatrix[11]) +
			mMatrix[5] * (mMatrix[2] * mMatrix[11] -
			mMatrix[10] * mMatrix[3]) + mMatrix[9] *
			(mMatrix[6] * mMatrix[3] - mMatrix[2] * mMatrix[7]));
		s(1, 3) = d * (mMatrix[2] * (mMatrix[8] * mMatrix[7] -
			mMatrix[4] * mMatrix[11]) +
			mMatrix[6] * (mMatrix[0] * mMatrix[11] -
			mMatrix[8] * mMatrix[3]) + mMatrix[10] *
			(mMatrix[4] * mMatrix[3] - mMatrix[0] * mMatrix[7]));
		s(2, 3) = d * (mMatrix[3] * (mMatrix[8] * mMatrix[5] -
			mMatrix[4] * mMatrix[9]) +
			mMatrix[7] * (mMatrix[0] * mMatrix[9] -
			mMatrix[8] * mMatrix[1]) + mMatrix[11] *
			(mMatrix[4] * mMatrix[1] - mMatrix[0] * mMatrix[5]));
		s(3, 3) = d * (mMatrix[0] * (mMatrix[5] * mMatrix[10] -
			mMatrix[9] * mMatrix[6]) +
			mMatrix[4] * (mMatrix[9] * mMatrix[2] -
			mMatrix[1] * mMatrix[10]) + mMatrix[8] *
			(mMatrix[1] * mMatrix[6] - mMatrix[5] * mMatrix[2]));

		return true;
	}

	/** Invert the current matrix.
	 \return Whether the matrix could be inverted (non-singular), or not
	         (singular). */
	bool invert()
	{
		matrix4<Real> tmp;
		if (getInverse(tmp))
		{
			*this = tmp;
			return true;
		}
		return false;
	}

	/** Build a rotation matrix using euler angles ('pitch', 'yaw', 'roll')
	 The up axis ('yaw') is the y axis, the forward axis ('roll') is the -z axis,
	 and the side-to-side axis ('pitch') is the x axis. */
	static matrix4<Real> BuildEulerRotationMatrix(const vector3<Real>& angles)
	{
		Real temp[16];
		Real cosp = Math<Real>::Cos(angles.getX());
		Real sinp = Math<Real>::Sin(angles.getX());
		Real cosy = Math<Real>::Cos(angles.getY());
		Real siny = Math<Real>::Sin(angles.getY());
		Real cosr = Math<Real>::Cos(angles.getZ());
		Real sinr = Math<Real>::Sin(angles.getZ());

		temp[0] = cosy*cosr;
		temp[1] = -cosy*sinr*cosp + siny*sinp;
		temp[2] = cosy*sinr*sinp + siny*cosp;
		temp[3] = 0;

		temp[4] = sinr;
		temp[5] = cosr*cosp;
		temp[6] = -cosr*sinp;
		temp[7] = 0;

		temp[8] = -siny*cosr;
		temp[9] = siny*sinr*cosp + cosy*sinp;
		temp[10] = -siny*sinr*sinp + cosy*cosp;
		temp[11] = 0;

		temp[12] = 0;
		temp[13] = 0;
		temp[14] = 0;
		temp[15] = 1;

		return matrix4<Real>(temp);
	}

	/** Load a project matrix in the current matrix.
	 This is very similar to the gluPerspective() function.
	 \param fovy   The field of view, in radians.
	 \param aspect The width to height ratio, in pixels
	 \param zNear  The near z-clipping plane.
	 \param zFar   The far z-clipping plane. */
	void loadProjectionMatrixFOV(Real fovyRadians, Real aspect, Real zNear, Real zFar)
	{
		memset(mMatrix, 0, 16*sizeof(Real));
		Real f = Math<Real>::Cot(fovyRadians/static_cast<Real>(2.0), false);
		Real invZNZF = static_cast<Real>(1.0/(zNear-zFar));
		mMatrix[0]  = f/aspect;
		mMatrix[5]  = f;
		mMatrix[10] = (zNear+zFar)*invZNZF;
		mMatrix[11] = static_cast<Real>(2.0*zFar*zNear*invZNZF);
		mMatrix[14] = static_cast<Real>(-1.0);
	}

	/** Builds a projection matrix, and returns it. */
	static matrix4<Real> BuildProjectionMatrixFOV(Real fovyRadians, Real aspect, Real zNear, Real zFar)
	{
		Real f = Math<Real>::Cot(fovyRadians/2, false);
		Real invZNZF = static_cast<Real>(1.0/(zNear-zFar));
		return matrix4<Real>(f/aspect, 0, 0,                    0,
			0,        f, 0,                    0,
			0,        0, (zNear+zFar)*invZNZF, 2*zFar*zNear*invZNZF,
			0,        0, -1,                   0);
	}

	/** Load a lookat matrix, for a right-handed system.
	 \param eye    The 'center' of the camera.
	 \param center What the camera is looking at.
	 \param up     The upwards direction for the camera. */
	void loadLookAtMatrixRH(const vector3<Real>& eye, const vector3<Real>& center,
		const vector3<Real>& up)
	{
		vector3<Real> zaxis = eye-center;
		vector3<Real> xaxis = up.cross(zaxis);
		zaxis.normalize(); xaxis.normalize();
		vector3<Real> yaxis = zaxis.cross(xaxis);
		mMatrix[0]  = xaxis.getX();
		mMatrix[1]  = xaxis.getY();
		mMatrix[2]  = xaxis.getZ();
		mMatrix[3]  = -(xaxis.dot(eye));

		mMatrix[4]  = yaxis.getX();
		mMatrix[5]  = yaxis.getY();
		mMatrix[6]  = yaxis.getZ();
		mMatrix[7]  = -(yaxis.dot(eye));

		mMatrix[8]  = zaxis.getX();
		mMatrix[9]  = zaxis.getY();
		mMatrix[10] = zaxis.getZ();
		mMatrix[11] = -(zaxis.dot(eye));

		mMatrix[12] = static_cast<Real>(0.0);
		mMatrix[13] = static_cast<Real>(0.0);
		mMatrix[14] = static_cast<Real>(0.0);
		mMatrix[15] = static_cast<Real>(1.0);
	}

	/** Loads a LookAt right-handed matrix from the eye, the center, and the up vector. */
	void loadLookAtMatrixRH(Real eyeX, Real eyeY, Real eyeZ,
		Real centerX, Real centerY, Real centerZ,
		Real upX, Real upY, Real upZ)
	{
		loadLookAtMatrixRH(vector3<Real>(eyeX, eyeY, eyeZ),
			vector3<Real>(centerX, centerY, centerZ), vector3<Real>(upX, upY, upZ));
	}

	/** Loads an axis-angle rotation into the matrix. */
	void loadFromAxisAngleRotation(const vector3<Real>& n, Real aRad)
	{
		Real cosAngle = Math<Real>::Cos(aRad, false);
		Real sinAngle = Math<Real>::Sin(aRad, false);
		_mMatrix[0][0] = cosAngle+(1-cosAngle)*n.getX()*n.getX();
		_mMatrix[0][1] = (1-cosAngle)*n.getX()*n.getY()-sinAngle*n.getZ();
		_mMatrix[0][2] = (1-cosAngle)*n.getX()*n.getZ()+sinAngle*n.getY();
		_mMatrix[0][3] = 0;

		_mMatrix[1][0] = (1-cosAngle)*n.getX()*n.getY()+sinAngle*n.getZ();
		_mMatrix[1][1] = cosAngle+(1-cosAngle)*n.getY()*n.getY();
		_mMatrix[1][2] = (1-cosAngle)*n.getY()*n.getZ()-sinAngle*n.getX();
		_mMatrix[1][3] = 0;

		_mMatrix[2][0] = (1-cosAngle)*n.getX()*n.getZ()-sinAngle*n.getY();
		_mMatrix[2][1] = (1-cosAngle)*n.getY()*n.getZ()+sinAngle*n.getX();
		_mMatrix[2][2] = cosAngle+(1-cosAngle)*n.getZ()*n.getZ();
		_mMatrix[2][3] = 0;

		_mMatrix[3][0] = 0;
		_mMatrix[3][1] = 0;
		_mMatrix[3][2] = 0;
		_mMatrix[3][3] = 1;
	}

	/** Builds a matrix representing an axis-angle rotation. */
	static matrix4<Real> BuildFromAxisAngleRotationMatrix(const vector3<Real>& n, Real aRad)
	{
		Real tmp[16];
		Real cosAngle = Math<Real>::Cos(aRad, false);
		Real sinAngle = Math<Real>::Sin(aRad, false);

		tmp[0]  = cosAngle+(1-cosAngle)*n.getX()*n.getX();
		tmp[1]  = (1-cosAngle)*n.getX()*n.getY()-sinAngle*n.getZ();
		tmp[2]  = (1-cosAngle)*n.getX()*n.getZ()+sinAngle*n.getY();
		tmp[3]  = 0;

		tmp[4]  = (1-cosAngle)*n.getX()*n.getY()+sinAngle*n.getZ();
		tmp[5]  = cosAngle+(1-cosAngle)*n.getY()*n.getY();
		tmp[6]  = (1-cosAngle)*n.getY()*n.getZ()-sinAngle*n.getX();
		tmp[7]  = 0;

		tmp[8]  = (1-cosAngle)*n.getX()*n.getZ()-sinAngle*n.getY();
		tmp[9]  = (1-cosAngle)*n.getY()*n.getZ()+sinAngle*n.getX();
		tmp[10] = cosAngle+(1-cosAngle)*n.getZ()*n.getZ();
		tmp[11] = 0;

		tmp[12] = 0;
		tmp[13] = 0;
		tmp[14] = 0;
		tmp[15] = 1;
		return matrix4<Real>(tmp);
	}

	/** Returns a vector transformed by the matrix.
	 \param in The vector to transform. */
	inline vector3<Real> applyTransformation(const vector3<Real>& in) const
	{
		return vector3<Real>(in.getX()*_mMatrix[0][0]+in.getY()*_mMatrix[0][1]+in.getZ()*_mMatrix[0][2]+_mMatrix[0][3],
			in.getX()*_mMatrix[1][0] + in.getY()*_mMatrix[1][1] + in.getZ()*_mMatrix[1][2]+_mMatrix[1][3],
			in.getX()*_mMatrix[2][0] + in.getY()*_mMatrix[2][1] + in.getZ()*_mMatrix[2][2]+_mMatrix[2][3]);
	}

	/** Transform a given vector. */
	inline void transform(vector3<Real>& in) const
	{
		in.setX(in.getX()*_mMatrix[0][0]+in.getY()*_mMatrix[0][1]+in.getZ()*_mMatrix[0][2]+_mMatrix[0][3]);
		in.setY(in.getX()*_mMatrix[1][0]+in.getY()*_mMatrix[1][1]+in.getZ()*_mMatrix[1][2]+_mMatrix[1][3]);
		in.setZ(in.getX()*_mMatrix[2][0]+in.getY()*_mMatrix[2][1]+in.getZ()*_mMatrix[2][2]+_mMatrix[2][3]);
	}

	/** Returns the transform of a given bounding box. */
	aabbox<Real> applyTransformation(const aabbox<Real>& in) const
	{
		aabbox<Real> out = in;
		transform(out);
		return out;
	}

	/** Transforms the given bounding box. */
	void transform(aabbox<Real>& in) const
	{
		Real a, b;
		Real AMin[3], AMax[3];
		Real BMin[3], BMax[3];

		AMin[0] = in.getMinPoint().getX();
		AMin[1] = in.getMinPoint().getY();
		AMin[2] = in.getMinPoint().getZ();
		AMax[0] = in.getMaxPoint().getX();
		AMax[1] = in.getMaxPoint().getY();
		AMax[2] = in.getMaxPoint().getZ();

		BMin[0] = BMax[0] = _mMatrix[0][3];
		BMin[1] = BMax[1] = _mMatrix[1][3];
		BMin[2] = BMax[2] = _mMatrix[2][3];

		for (u32 i = 0; i < 3; i++)
		{
			for (u32 j = 0; j < 3; j++)
			{
				a = _mMatrix[i][j]*AMin[j];
				b = _mMatrix[i][j]*AMax[j];
				if (a < b)
				{
					BMin[i] += a;
					BMax[i] += b;
				}
				else
				{
					BMin[i] += b;
					BMax[i] += a;
				}
			}
		}
		in.reset(vector3<Real>(BMin), vector3<Real>(BMax));
	}

	/** Returns a pointer to the values in the matrix, in row-major form. */
	inline const Real * v() const
	{
		return mMatrix;
	}

	/** Returns a pointer to the values in the matrix, with the values packed
	 suitable for column-major loading */
	inline const Real * getColumnMajor() const
	{
		return getTranspose().v();
	}

	/** Returns a pointer to the values in the matrix, with the values packed
	 suitable for row-major loading.*/
	inline const Real * getRowMajor() const
	{
		return mMatrix;
	}

	/** Returns the trace - the sum of the diagonal elements in the matrix. */
	Real getTrace() const
	{
		return _mMatrix[0][0] + _mMatrix[1][1] + _mMatrix[2][2] + _mMatrix[3][3];
	}

	/** Returns whether the matrix is diagonal. */
	bool isDiagonal() const
	{
		return Math<Real>::Equals(mMatrix[4], 0) && Math<Real>::Equals(mMatrix[8], 0) &&
			Math<Real>::Equals(mMatrix[9], 0) && Math<Real>::Equals(mMatrix[12], 0) &&
			Math<Real>::Equals(mMatrix[13], 0) && Math<Real>::Equals(mMatrix[14], 0);
	}

	/** Returns whether the matrix is orthonormal. */
	bool isOrthonormal() const
	{
		return *this * getTranspose() == IDENTITY_MATRIX;
	}

	/** Builds a scaling matrix.
	 \param factor The scaling factor. */
	static const matrix4<Real> BuildScalingMatrix(const vector3<Real>& factor)
	{
		Real tmp[16];
		memset(tmp, 0, 16*sizeof(Real));
		tmp[0]  = factor.getX();
		tmp[5]  = factor.getY();
		tmp[10] = factor.getZ();
		tmp[15] = static_cast<Real>(1);
		return matrix4<Real>(tmp);
	}

	/** Builds a translation matrix.
	 \param by The translation vector to load. */
	static matrix4<Real> BuildTranslationMatrix(const vector3<Real>& by)
	{
		Real tmp[16];
		tmp[3]  = by.getX();
		tmp[7]  = by.getY();
		tmp[11] = by.getZ();
		tmp[15] = static_cast<Real>(1);
		return matrix4<Real>(tmp);
	}

	/** Returns whether two matrices are equal. */
	bool operator==(const matrix4<Real>& rhs) const
	{
		for (s32 i = 0; i < 16; i++)
			if (!Math<Real>::Equals(mMatrix[i], rhs.mMatrix[i]))
				return false;
		return true;
	}

	/** Returns whether two matrices are different. */
	bool operator!=(const matrix4<Real>& rhs) const
	{
		for (s32 i = 0; i < 16; i++)
			if (!Math<Real>::Equals(mMatrix[i], rhs.mMatrix[i]))
				return true;
		return false;
	}

	/** Returns the [i, j] element of the matrix. */
	inline Real operator()(u32 i, u32 j) const
	{
		return _mMatrix[i][j];
	}

	/** Returns the [i, j] element of the matrix. */
	inline Real& operator()(u32 i, u32 j)
	{
		return _mMatrix[i][j];
	}

	/** Returns the result of a sum between two matrices */
	matrix4<Real> operator+(const matrix4<Real>& rhs) const
	{
		u32 i, j;
		matrix4<Real> r;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] + rhs(i, j);
		return r;
	}

	/** Returns the result of adding some constant to the current matrix. */
	matrix4<Real> operator+(Real val) const
	{
		u32 i, j;
		matrix4<Real> r;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] + val;
		return r;
	}

	/** Returns the result of subtracting a matrix from this one. */
	matrix4<Real> operator-(const matrix4<Real>& rhs) const
	{
		u32 i, j;
		matrix4<Real> r;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] - rhs(i, j);
		return r;
	}

	/** Returns the result of subtracting some constant from the current matrix. */
	matrix4<Real> operator-(Real val) const
	{
		u32 i, j;
		matrix4<Real> r;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] - val;
		return r;
	}

	/** Returns the result of diving the matrix by some constant. */
	matrix4<Real> operator/(Real val) const
	{
		u32 i, j;
		matrix4<Real> r;
		Real oneval = 1.0f / val;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] * oneval;
		return r;
	}

	/** Returns the result of multiplying two matrices. */
	matrix4<Real> operator*(const matrix4<Real>& rhs) const
	{
		u32 i, j, k;
		matrix4<Real> r;
		Real val;

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				val = 0;
				for (k = 0; k < 4; k++) {
					val = val + _mMatrix[i][k] * rhs(k, j);
				}
				r(i, j) = val;
			}
		}
		return r;
	}

	/** Returns the result of multiplying the matrix by some constant. */
	matrix4<Real> operator*(Real val) const
	{
		u32 i, j;
		matrix4<Real> r;

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				r(i, j) = _mMatrix[i][j] * val;
		return r;
	}

	/** Add a matrix to the current matrix. */
	const matrix4<Real>& operator+=(const matrix4<Real>& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	/** Add a constant to the current matrix. */
	const matrix4<Real>& operator+=(Real val)
	{
		*this = *this + val;
		return *this;
	}

	/** Subtract a matrix from the current one. */
	const matrix4<Real>& operator-=(const matrix4<Real>& rhs)

	{
		*this = *this - rhs;
		return *this;
	}

	/** Subtract a constant from the current matrix. */
	const matrix4<Real>& operator-=(Real val)
	{
		*this = *this - val;
		return *this;
	}

	/** Divide the matrix by a constant. */
	const matrix4<Real>& operator/=(Real val)
	{
		*this = *this / val;
		return *this;
	}

	/** Multiply the matrix by another matrix. */
	const matrix4<Real>& operator*=(const matrix4<Real>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	/** Multiply the matrix by a constant. */
	const matrix4<Real>& operator*=(Real val)
	{
		*this = *this * val;
		return *this;
	}

	/** Load another matrix into this one. */
	const matrix4<Real>& operator=(const matrix4<Real>& other)
	{
		memcpy(mMatrix, other.v(), 16*sizeof(Real));
		return *this;
	}

	/** Load a quaternion into this matrix. */
	const matrix4<Real>& operator=(const quaternion<Real>& quat)
	{
		*this = quat.getMatrix();
		return *this;
	}

	/** Returns a textual representation of the matrix. */
	String toString() const
	{
		String mt("matrix4[ ");
		for (s32 i = 0; i < 16; i++)
		{
			if (i > 0 && i % 4 == 0)
				mt += "\n         ";
			mt += mMatrix[i];
			mt += " ";
		}
		return mt + "]";
	}

private:
	/** The 4x4 matrix, which is internally represented by a
	 single-dimension Array. We make it a union, so that
	 accessing element [i][j] can be done easily, but it can
	 also be treated as a one-dimensional Array. */
	union
	{
		Real mMatrix[16];
		Real _mMatrix[4][4];
	};
};

typedef matrix4<f32> matrix4f;
typedef matrix4<f64> matrix4d;

template class _FIRE_ENGINE_API_ matrix4<f32>;
template class _FIRE_ENGINE_API_ matrix4<f64>;

} // namespace fire_engine

#endif // MATRIX4_H_INCLUDED
