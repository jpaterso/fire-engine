/*
 *  $Id: line3.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Definition for the line3 class
 *  with heaps of useful methods
**/

#ifndef	__LINE3_H
#define	__LINE3_H

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "string.h"

namespace fire_engine
{

template<class T>
class _FIRE_ENGINE_API_ line3
{
public:
	/**
	 *	Basic constructor
	**/
	line3()
		: m_pt(0, 0, 0), m_dir(0, 0, 0)
	{
	}

	/**
	 *	Construct a line from a point and a vector
	 *	@param	pt	A point on the line
	 *	@param	dir	A directional vector for the line
	**/
	line3(const vector3<T>& pt, const vector3<T>& dir)
	{
		m_pt  = pt;
		m_dir = dir;
	}

	/**
	 *	Construct a line from a point and a vector
	 *	@param	pt	A pointer to a point on the line
	 *	@param	dir	A pointer to a directional vector for the line
	**/
	line3(const vector3<T> * pt, const vector3<T> * dir)
	{
		m_pt  = *pt;
		m_dir = *dir;
	}

	/**
	 *	Basic destructor, does nothing
	**/
	~line3(void)
	{
	}

	/**
	 *	Sets a point for the line, and a vector
	 *	@param	pt	A point on the line
	 *	@param	dir	A directional vector for the line
	**/
	void set(const vector3<T> * pt, const vector3<T> * dir)
	{
		m_pt  = *pt;
		m_dir = *dir;
	}

	/**
	 *	Checks whether a point is on the line
	 *	@param	pt	The point to check for
	 *	@return		true if the point is on the line, false otherwise
	**/
	bool isOnLine(const vector3<T>& pt) const
	{
		vector3<T> * v = new vector3<T>();
		v->set(pt.getX()-m_pt.getX(), pt.getY()-m_pt.getY(), pt.getZ()-m_pt.getZ());

		if (v->isColinear(m_dir))
			return (true);
		else
			return (false);
	}

	/**
	 *	Getter for the point in the line
	 *	@return		A reference to the point in the line
	**/
	vector3<T>& point(void)
	{
		return (m_pt);
	}

	/**
	 *	Getter for the direction of the line
	 *	@return		A reference to the vector defining the line
	**/
	vector3<T>& direction(void)
	{
		return (m_dir);
	}

	/**
	 *	Getter for the point in the line
	 *	@return		The point in the line
	**/
	const vector3<T> point(void) const { return (m_pt); }

	/**
	 *	Getter for the direction of the line
	 *	@return		The direction of the line
	**/
	const vector3<T> direction(void) const { return (m_dir); }


	/**
	 *	Check whether two lines are equal
	 *	@param	other	The other line to compare with the current one
	 *	@return			true if the lines are equal, false otherwise
	**/
	bool operator==(const line3<T>& other) const
	{
		return ((m_pt == other.point()) && (m_dir == other.direction()));
	}

	/**
	 *	Check whether two lines are different
	 *	@param	other	The other line to compare with the current one
	 *	@return			true if the lines are different, false otherwise
	**/
	bool operator!=(const line3<T>& other) const
	{
		return ((m_pt != other.point()) || (m_dir != other.direction()));
	}

	/**
	 *	Assign a new line to the current one
	 *	@param	other	The line to assign to the current one
	 *	@return			A reference to the new line
	**/
	const line3<T>& operator=(const line3<T>& other)
	{
		m_pt  = other.point();
		m_dir = other.direction();
		return (*this);
	}

	string toString(void) const
	{
		return string("line3[ ") + m_pt.toString() + ",\n       " +
			m_dir.toString() + " ]";
	}

private:
	//! A point on the line
	vector3<T> m_pt;
	//! The direction the line is going
	vector3<T> m_dir;
};
// Useful typedefs
typedef line3<s32> line3i;
typedef line3<f32> line3f;
typedef line3<f64> line3d;

template class _FIRE_ENGINE_API_ line3<f32>;
template class _FIRE_ENGINE_API_ line3<f64>;
} // namespace fire_engine

#endif
