/**
 * FILE:    Material.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Material.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Declaration of the Material class, used for setting the visual
 *          appearance of Objects as they are rendered.
**/

#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "String.h"
#include "Color.h"

#define MATERIAL_MAX_NUM_TEXTURES 0x04

namespace fire_engine
{

enum EMATERIAL_COLOR_TYPE
{
	EMP_AMBIENT,
	EMP_DIFFUSE,
	EMP_SPECULAR,
	EMP_EMISSIVE
};

/** The various boolean flags that can be set for a material. */
enum EMATERIAL_PROPERTY
{
	/** Does this material support alpha blending? The default value is true. */
	EMP_ALPHA_BLENDING,
	/** Should this material be affected by the current lighting conditions? Default : true. */
	EMP_LIGHTING,
	/** Should the polygons not be filled, but a wireframe representation of the polygons used
	 instead? Default : false. */
	EMP_WIREFRAME,
	/** Should the polygons rendered using this material update the Z-Buffer? Default : true. */
	EMP_WRITE_TO_Z_BUFFER
};

class ITexture;

/** The Material class holds all the information on how a surface should
 be rendered. These properties included any textures, transparency options,
 material properties, lightmaps, etc... */
class _FIRE_ENGINE_API_ Material
{
public:
	/** Create a Material with a name. */
	Material(const String& name = String("Default Material Name"));

	/** Material constructor from another Material. */
	Material(const Material& other);

	/** Destructor. */
	virtual ~Material();

	/** Returns the ambient color. */
	inline const Color32& getAmbient() const;

	/** Returns the diffuse color. */
	inline const Color32& getDiffuse() const;

	/** Returns the specular color. */
	inline const Color32& getSpecular() const;

	/** Returns the shininess color. */
	inline const Color32& getEmissive() const;

	/** Returns the shininess factor. */
	inline f32	getShininess() const;

	/** Returns the name of the Material */
	inline const String& getName() const;

	/** Sets the ambient color. */
	inline void setAmbient(const Color32& ambient);

	/** Sets the diffuse color. */
	inline void setDiffuse(const Color32& diffuse);

	/** Sets the specular color. */
	inline void setSpecular(const Color32& specular);

	/** Sets the emissive color. */
	inline void setEmissive(const Color32& emissive);

	/** Sets the shininess. */
	inline void setShininess(f32 sh);

	/** Sets the texture for a given level. */
	void setTexture(s32 level, ITexture * tex);

	/** Gets the texture for a given level. */
	const ITexture * getTexture(s32 level) const;

	/** Sets one of the flags for the material. */
	void setMaterialProperty(EMATERIAL_PROPERTY emp, bool isset);

	/** Returns whether a given material property is on or off. */
	bool getMaterialProperty(EMATERIAL_PROPERTY emp) const;

	/** Assign a new Material to this one. */
	const Material& operator=(const Material& other);

private:
	String  m_name;
	Color32 m_ambient;
	Color32 m_diffuse;
	Color32 m_specular;
	Color32 m_emissive;
	f32     m_shininess;
	//u32     mFlags;

	struct
	{
		bool mUseAlphaBlending;
		bool mLightingEnabled;
		bool mWireframe;
		bool mWriteToZBuffer;
	};

	ITexture * Textures[MATERIAL_MAX_NUM_TEXTURES];

};

inline const Color32& Material::getAmbient() const
{
	return m_ambient;
}

inline const Color32& Material::getDiffuse() const
{
	return m_diffuse;
}

inline const Color32& Material::getSpecular() const
{
	return m_specular;
}

inline const Color32& Material::getEmissive() const
{
	return m_emissive;
}

inline f32	Material::getShininess() const
{
	return m_shininess;
}

inline const String& Material::getName() const
{
	return m_name;
}

inline void Material::setAmbient(const Color32& ambient)
{
	m_ambient = ambient;
}

inline void Material::setDiffuse(const Color32& diffuse)
{
	m_diffuse = diffuse;
}

inline void Material::setSpecular(const Color32& specular)
{
	m_specular = specular;
}

inline void Material::setEmissive(const Color32& emissive)
{
	m_emissive = emissive;
}

inline void Material::setShininess(f32 sh)
{
	m_shininess = sh;
}

} // namespace fire_engine

#endif // MATERIAL_H_INCLUDED
