/**
 * FILE:    Light.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Light.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A light in the 3D world.
**/

#include "Light.h"
#include "IRenderer.h"

namespace fire_engine
{
Light::Light(ELIGHT_TYPE type, vector3f position, vector3f direction, Color32 ambient,
	Color32 diffuse, Color32 specular, f32 radiusDegrees)
	: m_ltype(type), m_position(position), m_direction(direction), m_ambient(ambient),
	  m_diffuse(diffuse), m_specular(specular), mRadiusDegrees(radiusDegrees)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Light");
#endif
}

Light::Light(ELIGHT_TYPE type, vector3f direction, Color32 ambient, Color32 diffuse, Color32 specular)
	: m_ltype(type), m_position(0.0f, 0.0f, 0.0f), m_direction(direction), m_ambient(ambient),
	  m_diffuse(diffuse), m_specular(specular), mRadiusDegrees(0.0f)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Light");
#endif
}

Light::~Light()
{
}

u32 Light::hash(const Light& light)
{
	return Color32::hash(light.getAmbient())+Color32::hash(light.getDiffuse())
		+Color32::hash(light.getSpecular())+vector3f::hash(light.getPosition())
		+vector3f::hash(light.getDirection());
}

bool Light::operator==(const Light& rhs) const
{
	return m_ltype==rhs.getType() && m_ambient==rhs.getAmbient() &&
		m_diffuse==rhs.getDiffuse() && m_specular==rhs.getSpecular() &&
		m_position==rhs.getPosition() && m_direction==rhs.getDirection();
}

bool Light::operator!=(const Light& rhs) const
{
	return m_ltype!=rhs.getType() || m_ambient!=rhs.getAmbient() ||
		m_diffuse!=rhs.getDiffuse() || m_specular!=rhs.getSpecular() ||
		m_position!=rhs.getPosition() || m_direction!=rhs.getDirection();
}

Light& Light::operator=(const Light& rhs)
{
	m_ltype     = rhs.getType();
	m_ambient   = rhs.getAmbient();
	m_diffuse   = rhs.getDiffuse();
	m_specular  = rhs.getSpecular();
	m_position  = rhs.getPosition();
	m_direction = rhs.getDirection();
	return *this;
}

}
