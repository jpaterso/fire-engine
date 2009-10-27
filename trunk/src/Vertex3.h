/**
 * FILE:    Vertex3.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Vertex3.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A class for storing all information about a given vertex:
 *          it's position in space, it's normal, it's texture coordinates...
**/

#ifndef VERTEX3_H_INCLUDED
#define VERTEX3_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "vector2.h"
#include "Color.h"

namespace fire_engine
{

class _FIRE_ENGINE_API_ Vertex3
{
public:
	/** Constructors: A Vertex3 contains a position in space, a
	 normal, a color, and a set of texture coordinates */
	Vertex3(const vector3f& position = vector3f(0.0f, 0.0f, 0.0f),
		    const vector3f& normal = vector3f(0.0f, 0.0f, 0.0f),
		    const Color8& color = Color8(0xFF, 0xFF, 0xFF, 0xFF), // default Color is all 0xFF, so that a model
		                                                   // that doesn't specify the Color still actually appears
		    const vector2f& tex_coordinates = vector2f(0.0f, 0.0f))
		    : m_position(position), m_normal(normal), m_color(color),
		      m_tex_coordinates(tex_coordinates)
	{
	}

	Vertex3(f32 x, f32 y, f32 z,
            f32 nx, f32 ny, f32 nz,
            u8 r, u8 g, u8 b, u8 a,
            f32 s, f32 t)
            : m_position(x, y, z), m_normal(nx, ny, nz),
              m_color(r, g, b, a), m_tex_coordinates(s, t)
	{
	}

	/** Destructor - does nothing */
	~Vertex3()
	{
	}

	/** Getters and Setters for the various components of Vertex3 objects */
	inline void setPosition(const vector3f position);
	inline void setPosition(f32 x, f32 y, f32 z);
	inline const vector3f& getPosition() const;
	inline vector3f& getPosition();

	inline void setNormal(const vector3f position);
	inline void setNormal(f32 nx, f32 ny, f32 nz);
	inline const vector3f& getNormal() const;
	inline vector3f& getNormal();

	inline void setColor(const Color8 color);
	inline void setColor(u8 r, u8 g, u8 b, u8 a);
	inline const Color8& getColor() const;
	inline Color8& getColor();

	inline void setTextureCoordinates(const vector2f tex_coordinates);
	inline void setTextureCoordinates(f32 s, f32 t);
	inline const vector2f& getTextureCoordinates() const;
	inline vector2f& getTextureCoordinates();

	const Vertex3& operator=(const Vertex3& rhs)
	{
		m_position = rhs.getPosition();
		m_normal = rhs.getNormal();
		m_color = rhs.getColor();
		m_tex_coordinates = rhs.getTextureCoordinates();
		return *this;
	}

private:
	vector3f   m_position;
	vector3f   m_normal;
	Color8     m_color;
	vector2f   m_tex_coordinates;
};

inline void Vertex3::setPosition(const vector3f position)
{
	m_position = position;
}

inline void Vertex3::setPosition(f32 x, f32 y, f32 z)
{
	m_position.set(x, y, z);
}

inline vector3f& Vertex3::getPosition()
{
	return m_position;
}

inline const vector3f& Vertex3::getPosition() const
{
	return m_position;
}

inline void Vertex3::setNormal(const vector3f normal)
{
	m_normal = normal;
}

inline void Vertex3::setNormal(f32 nx, f32 ny, f32 nz)
{
	m_normal.set(nx, ny, nz);
}

inline vector3f& Vertex3::getNormal()
{
	return m_normal;
}

inline const vector3f& Vertex3::getNormal() const
{
	return m_normal;
}

inline void Vertex3::setColor(const Color8 color)
{
	m_color = color;
}

inline void Vertex3::setColor(u8 r, u8 g, u8 b, u8 a)
{
	m_color.set(r, g, b, a);
}

inline Color8& Vertex3::getColor()
{
	return m_color;
}

inline const Color8& Vertex3::getColor() const
{
	return m_color;
}

inline void Vertex3::setTextureCoordinates(const vector2f tex_coordinates)
{
	m_tex_coordinates = tex_coordinates;
}

inline void Vertex3::setTextureCoordinates(f32 s, f32 t)
{
	m_tex_coordinates.set(s, t);
}

inline vector2f& Vertex3::getTextureCoordinates()
{
	return m_tex_coordinates;
}

inline const vector2f& Vertex3::getTextureCoordinates() const
{
	return m_tex_coordinates;
}

}

#endif // VERTEX3_H_INCLUDED
