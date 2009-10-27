/**
 * FILE:    Light.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Light.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A light in the 3D world.
**/

#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "ISpaceNode.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "Color.h"
#include "Object.h"

namespace fire_engine
{

class IRenderer;

enum ELIGHT_TYPE
{
	/** ELT_DIRECTIONAL represents a light where all the components come from
	 a given direction, regardless of where you are in the world. It simulates
	 a light source that is an infinite distance away, like the sun. */
	ELT_DIRECTIONAL,
	/** ELT_POINT represents a light coming from a given location, and pointing
	 towards a specific location. */
	ELT_POINT,
};

/** A class representing a Light in the 3D world. */
class _FIRE_ENGINE_API_ Light : public Object
{
public:
	/** Constructor. */
	Light(ELIGHT_TYPE type = ELT_POINT,
		vector3f position  = vector3f(1.0f, 1.0f, 0.0f),
		vector3f direction = vector3f(-1.0f, -1.0f, -1.0f),
		Color32 ambient    = Color32(0.3f, 0.3f, 0.3f, 1.0f),
		Color32 diffuse    = Color32(1.0f, 1.0f, 1.0f, 1.0f),
		Color32 specular   = Color32(1.0f, 1.0f, 1.0f, 1.0f),
		f32 radiusDegrees  = 180.0f);

	Light(ELIGHT_TYPE type,
		vector3f direction = vector3f(-1.0f, -1.0f, -1.0f),
		Color32 ambient    = Color32(0.3f, 0.3f, 0.3f, 1.0f),
		Color32 diffuse    = Color32(1.0f, 1.0f, 1.0f, 1.0f),
		Color32 specular   = Color32(1.0f, 1.0f, 1.0f, 1.0f));

	/** Destructor. */
	virtual ~Light();

	/** Returns the type of light contained. */
	inline ELIGHT_TYPE getType() const;

	/** Returns the ambient color emitted from the Light. */
	inline Color32 getAmbient() const;

	/** Returns the diffuse color emitted from the Light. */
	inline Color32 getDiffuse() const;

	/** Returns the specular color emitted from the Light. */
	inline Color32 getSpecular() const;

	/** Returns the position of the Light. */
	inline vector3f getPosition() const;

	/** Returns the direction the Light is pointing in.*/
	inline vector3f getDirection() const;

	inline f32 getRadius() const;

	inline void setRadius(f32 radiusDegrees);

	/** Sets the type of Light contained. */
	inline void setType(ELIGHT_TYPE type);

	/** Sets the ambient color that the Light emits. */
	inline void setAmbient(Color32 ambient);

	/** Sets the diffuse color that Light emits. */
	inline void setDiffuse(Color32 diffuse);

	/** Sets the specular color that the Light emits. */
	inline void setSpecular(Color32 specular);

	/** Sets the position of the Light. */
	inline void setPosition(vector3f position);

	/** Sets the direction that the Light points in. */
	inline void setDirection(vector3f direction);

	/** Calculate a hash for the Light. */
	static u32 hash(const Light& light);

	/** Returns whether two Lights are equal or not. */
	bool operator==(const Light& rhs) const;

	/** Returns whether two Lights are different or not. */
	bool operator!=(const Light& rhs) const;

	/** Copy a different light onto this one. */
	Light& operator=(const Light& rhs);
private:
	ELIGHT_TYPE m_ltype;
	vector3f    m_position;
	vector3f    m_direction;
	Color32     m_ambient;
	Color32     m_diffuse;
	Color32     m_specular;
	f32         mRadiusDegrees;

};

inline ELIGHT_TYPE Light::getType() const
{
	return m_ltype;
}

inline Color32 Light::getAmbient() const
{
	return m_ambient;
}

inline Color32 Light::getDiffuse() const
{
	return m_diffuse;
}

inline Color32 Light::getSpecular() const
{
	return m_specular;
}

inline vector3f Light::getPosition() const
{
	return m_position;
}

inline vector3f Light::getDirection() const
{
	return m_direction;
}

inline f32 Light::getRadius() const
{
	return mRadiusDegrees;
}

inline void Light::setType(ELIGHT_TYPE type)
{
	m_ltype = type;
}

inline void Light::setAmbient(Color32 ambient)
{
	m_ambient = ambient;
}

inline void Light::setDiffuse(Color32 diffuse)
{
	m_diffuse = diffuse;
}

inline void Light::setSpecular(Color32 specular)
{
	m_specular = specular;
}

inline void Light::setPosition(vector3f position)
{
	m_position = position;
}

inline void Light::setDirection(vector3f direction)
{
	m_direction = direction;
}

inline void Light::setRadius(f32 radiusDegrees)
{
	mRadiusDegrees = radiusDegrees;
}

}

#endif // LIGHT_H_INCLUDED
