/**
 *  $Id: triangle3.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  A triangle points to 3 points in space
 *	Many operations are possible on triangles, such as detecting whether
 *	a line hits the triangle.
**/

#ifndef	__TRIANGLE3_H
#define	__TRIANGLE3_H

#include "Types.h"
#include "CompileConfig.h"
#include "plane3.h"
#include "line3.h"
#include "vector3.h"

namespace fire_engine
{

template<class T>
class _FIRE_ENGINE_API_ triangle3
{
	public:
		/**
		 *	Basic constructor
		**/
		triangle3 (void)
		{
		}

		/**
		 *	Construct a triangle from 3 vectors
		 *	@param	a	A pointer to the first point
		 *	@param	b	A pointer to the second point
		 *	@param	c	A pointer to the third point
		**/
		triangle3(vector3<T> * a, vector3<T> * b, vector3<T> * c)
		{
			m_a = *a;
			m_b = *b;
			m_c = *c;
		}

		/**
		 *	Construct a triangle from 3 vectors
		 *	@param	a	The first point
		 *	@param	b	The second point
		 *	@param	c	The third point
		**/
		triangle3(vector3<T> a, vector3<T> b, vector3<T> c)
		{
			m_a = a;
			m_b = b;
			m_c = c;
		}

		/**
		 *	Basic destructor, does nothing
		**/
		~triangle3(void)
		{
		}

		/**
		 *	Set the points for the triangle
		 *	@param	a	A pointer to the first point
		 *	@param	b	A pointer to the second point
		 *	@param	c	A pointer to the third point
		**/
		void set(vector3<T> * a, vector3<T> * b, vector3<T> * c)
		{
			m_a = *a;
			m_b = *b;
			m_c = *c;
		}

		/**
		 *	Set the points for the triangle
		 *	@param	a	The first point
		 *	@param	b	The second point
		 *	@param	c	The third point
		**/
		void set(vector3<T> a, vector3<T> b, vector3<T> c)
		{
			m_a = a;
			m_b = b;
			m_c = c;
		}

		/**
		 *	Getter for the first point of the triangle
		 *	@return	A reference to the first point in the triangle
		**/
		vector3<T>& a(void)
		{
				return (m_a);
		}

		/**
		 *	Getter for the second point of the triangle
		 *	@return	A reference to the second point in the triangle
		**/
		vector3<T>& b(void)
		{
				return (m_b);
		}

		/**
		 *	Getter for the third point of the triangle
		 *	@return	A reference to the third point in the triangle
		**/
		vector3<T>& c(void)
		{
				return (m_c);
		}

		/**
		 *	Getter for the first point of the triangle
		 *	@return	The first point in the triangle
		**/
		const vector3<T> a(void) const { return (m_a); }

		/**
		 *	Getter for the second point of the triangle
		 *	@return	The second point in the triangle
		**/
		const vector3<T> b(void) const { return (m_b); }

		/**
		 *	Getter for the third point of the triangle
		 *	@return	The third point in the triangle
		**/
		const vector3<T> c(void) const { return (m_c); }


		/**
		 *	Display the three vertices of the triangle
		**/
		void display(const char * fmt) const
		{
			m_a.display(fmt);
			m_b.display(fmt);
			m_c.display(fmt);
		}

		/**
		 *	Checks whether a given point is in the triangle
		 *	To do this, we calculate the sum of angles between the point
		 *	and the vertices. The point lies within the triangle if this sum
		 *	is greather than or equal to 2 PI.
		 *	@param	pt	The point
		 *	@return		true if the point is in the triangle, false otherwise
		**/
		bool isPointOnTriangle(const vector3<T>& pt) const
		{
			T angle = (T) 0.0;
			vector3<T> * v1 = new vector3<T>();
			vector3<T> * v2 = new vector3<T>();

			if (pt == m_a || pt == m_b || pt == m_c)
				return (true);

			*v1 = m_a - pt;
			*v2 = m_b - pt;
			angle = angle + v1->angleBetween(*v2);

			*v1 = m_b - pt;
			*v2 = m_c - pt;
			angle = angle + v1->angleBetween(*v2);

			*v1 = m_c - pt;
			*v2 = m_a - pt;
			angle = angle + v1->angleBetween(*v2);

			if (angle >= Math<T>::TWO_PI)
				return (true);
			else
				return (false);
		}

		/**
		 *	Checks whether a line intersects with triangle
		 *	@param	l	The line to look at
		 *	@return		true if the line hits the triangle, false otherwise
		**/
		bool doesLineIntersect(const line3<T>& l) const
		{
			plane3<T> * p = makePlane();
			vector3<T> * pt = new vector3<T>();

			if (p->intersectWithLine(l, *pt)) {
				if (isPointOnTriangle(*pt)) {
					return (true);
				}
			}
			return (false);
		}

		/**
		 *	Construct a plane from the triangle
		 *	@return	A pointer to the plane the triangle is on
		**/
		plane3<T> * makePlane(void)
		{
			return (new plane3<T>(m_a, m_b, m_c));
		}


		/**
		 *	Check whether two triangles are equal
		 *	@param	other	The other triangle to compare with the current one
		 *	@return			true if the triangles are equal, false otherwise
		**/
		bool operator==(const triangle3<T>& other) const
		{
			return (other.a() == m_a && other.b() == m_b && other.c() == m_c);
		}

		/**
		 *	Check whether two triangles are different
		 *	@param	other	The other triangle to compare with the current one
		 *	@return			true if the triangles are different, false otherwise
		**/
		bool operator!=(const triangle3<T>& other) const
		{
			return (other.a() != m_a || other.b() != m_b || other.c() != m_c);
		}

		/**
		 *	Assign a new triangle to the current one
		 *	@param	other	The triangle to assign to the current one
		 *	@return			A reference to the new triangle
		**/
		const triangle3<T>& operator=(const triangle3<T>& other)
		{
			m_a = other.a();
			m_b = other.b();
			m_c = other.c();

			return (*this);
		}

	private:
		vector3<T> m_a, m_b, m_c;
};
	// Useful typedefs
	typedef triangle3<s32> triangle3i;
	typedef triangle3<f32> triangle3f;
} // namespace fire_engine

#endif
