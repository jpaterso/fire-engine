/**
 * FILE:    Material.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Material.cpp 111 2007-09-25 01:03:34Z jpaterso $
 * PURPOSE: Definition of the Material class, used for setting the visual
 *          appearance of Objects as they are rendered.
**/

#include "Material.h"
#include "ITexture.h"
#include <stdio.h>

namespace fire_engine
{

Material::Material(const string& name)
	: m_name(name), m_ambient(0.4f, 0.4f, 0.4f, 1.0f), m_diffuse(0.8f, 0.8f, 0.8f, 1.0f),
	  m_specular(0.0f, 0.0f, 0.0f, 1.0f), m_emissive(0.0f, 0.0f, 0.0f, 1.0f),
	  m_shininess(0.0f), mUseAlphaBlending(true), mLightingEnabled(true),
	  mWireframe(false), mWriteToZBuffer(true)
{
	memset(Textures, 0, MATERIAL_MAX_NUM_TEXTURES*sizeof(ITexture*));
}

Material::Material(const Material& other)
{
	memset(Textures, 0, MATERIAL_MAX_NUM_TEXTURES*sizeof(ITexture *));
	*this = other;
}

Material::~Material()
{
	for (s32 i = 0; i < MATERIAL_MAX_NUM_TEXTURES; i++)
	{
		if (Textures[i] != 0)
		{
			Textures[i]->drop();
		}
	}
}

void Material::setMaterialProperty(EMATERIAL_PROPERTY emp, bool isset)
{
	switch (emp)
	{
	case EMP_ALPHA_BLENDING:
		mUseAlphaBlending = isset;
		break;
	case EMP_LIGHTING:
		mLightingEnabled = isset;
		break;
	case EMP_WIREFRAME:
		mWireframe = isset;
		break;
	case EMP_WRITE_TO_Z_BUFFER:
		mWriteToZBuffer = isset;
		break;
	}
}


bool Material::getMaterialProperty(EMATERIAL_PROPERTY emp) const
{
	bool prop = false;
	switch (emp)
	{
	case EMP_ALPHA_BLENDING:
		prop = mUseAlphaBlending;
		break;
	case EMP_LIGHTING:
		prop = mLightingEnabled;
		break;
	case EMP_WIREFRAME:
		prop = mWireframe;
		break;
	case EMP_WRITE_TO_Z_BUFFER:
		prop = mWriteToZBuffer;
		break;
	}
	return prop;
}

const Material& Material::operator=(const Material& other)
{
	m_ambient     = other.getAmbient();
	m_diffuse     = other.getDiffuse();
	m_specular    = other.getSpecular();
	m_emissive    = other.getEmissive();
	m_name        = other.getName();
	m_shininess   = other.getShininess();
	mUseAlphaBlending = other.mUseAlphaBlending;
	mLightingEnabled = other.mLightingEnabled;
	mWireframe = other.mWireframe;
	mWriteToZBuffer = other.mWriteToZBuffer;
	for (s32 i = 0; i < MATERIAL_MAX_NUM_TEXTURES; i++)
	{
		if (Textures[i] != 0)
		{	
			Textures[i]->drop();
		}
	}
	for (s32 i = 0; i < MATERIAL_MAX_NUM_TEXTURES; i++)
	{
		Textures[i] = other.Textures[i];
		if (Textures[i])
		{
			Textures[i]->grab();
		}
	}

	return *this;
}

void Material::setTexture(s32 level, ITexture * tex)
{
	if (level >= MATERIAL_MAX_NUM_TEXTURES)
	{
		return;
	}
	if (Textures[level])
	{
		Textures[level]->drop();
	}
	Textures[level] = tex;
	if (Textures[level])
	{	
		Textures[level]->grab();
	}
}

const ITexture * Material::getTexture(s32 level) const
{
	if (level >= MATERIAL_MAX_NUM_TEXTURES)
	{
		return nullptr;
	}
	return Textures[level];
}

}
