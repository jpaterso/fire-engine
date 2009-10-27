/**
 * FILE:    Color.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Color.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A class template for storing a single RGBA color.
**/

#ifndef	COLOR_H_INCLUDED
#define	COLOR_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "string.h"
#include <string.h>

namespace fire_engine
{

template <class Real>
class _FIRE_ENGINE_API_ Color
{
public:
	/** The white color. */
	static const Color<Real> WHITE;
	/** The black color. */
	static const Color<Real> BLACK;
	/** The red color. */
	static const Color<Real> RED;
	/** The green color. */
	static const Color<Real> GREEN;
	/** The blue color. */
	static const Color<Real> BLUE;
	/** The yellow color. */
	static const Color<Real> YELLOW;
	/** The orange color. */
	static const Color<Real> ORANGE;
	/** The grey color. */
	static const Color<Real> GREY;

public:
	Color(Real red = static_cast<Real>(0), Real green = static_cast<Real>(0),
		Real blue = static_cast<Real>(0), Real alpha = static_cast<Real>(0))
	{
		this->set(red, green, blue, alpha);
	}

	Color(const Real * components)
	{
		this->set(components);
	}

	inline Real red()   const
	{
		return m_Color[0];
	}

	inline Real green() const
	{
		return m_Color[1];
	}

	inline Real blue()  const
	{
		return m_Color[2];
	}

	inline Real alpha() const
	{
		return m_Color[3];
	}

	inline void setRed(Real red)
	{
		m_Color[0] = red;
	}

	inline void setGreen(Real green)
	{
		m_Color[1] = green;
	}

	inline void setBlue(Real blue)
	{
		m_Color[2] = blue;
	}

	inline void setAlpha(Real alpha)
	{
		m_Color[3] = alpha;
	}

	inline const Real * v() const
	{
		return m_Color;
	}

	inline void set(const Real * components)
	{
		memcpy(m_Color, components, 4*sizeof(components));
	}

	inline void set(Real red, Real green, Real blue, Real alpha)
	{
		m_Color[0] = red;
		m_Color[1] = green;
		m_Color[2] = blue;
		m_Color[3] = alpha;
	}

	Color<Real>  operator*(f32 val) const
	{
		return Color<Real>(static_cast<Real>(m_Color[0]*val), static_cast<Real>(m_Color[1]*val),
			static_cast<Real>(m_Color[2]*val), static_cast<Real>(m_Color[3]*val));
	}

	Color<Real>& operator*=(f32 val)
	{
		m_Color[0] *= val;
		m_Color[1] *= val;
		m_Color[2] *= val;
		m_Color[3] *= val;
		return *this;
	}

	Color<Real>  operator+(const Color<Real>& other) const
	{
		return Color<Real>(m_Color[0]+other.red(), m_Color[1]+other.green(),
			m_Color[2]+other.blue(), m_Color[3]+other.alpha());
	}

	Color<Real>& operator+=(const Color<Real>& other)
	{
		m_Color[0] += other.red();
		m_Color[1] += other.green();
		m_Color[2] += other.blue();
		m_Color[3] += other.alpha();
		return *this;
	}

	inline bool operator==(const Color<Real>& rhs) const
	{
		return memcmp(m_Color, rhs.v(), 4*sizeof(Real)) == 0;
	}

	inline bool operator!=(const Color<Real>& rhs) const
	{
		return memcmp(m_Color, rhs.v(), 4*sizeof(Real)) != 0;
	}

	const Color<Real>& operator=(const Color<Real>& other)
	{
		memcpy(m_Color, other.v(), 4*sizeof(Real));
		return *this;
	}

	string toString() const
	{
		return string("Color[ ") + m_Color[0] + ", " + m_Color[1] + ", " + m_Color[2]
		+ ", " + m_Color[3] + " ]";
	}

	static u32 hash(const Color<Real>& key)
	{
		return static_cast<u32>(key.red()+key.green()+key.blue()+key.alpha());
	}
private:
	Real m_Color[4];
};

/** Some useful typedefs. */
typedef Color<u8>  Color8;
typedef Color<f32> Color32;

} // namespace fire_engine

#endif // COLOR_H_INCLUDED
