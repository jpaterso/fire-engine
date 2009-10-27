/**
 * FILE:    Q3MapLoader.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Q3MapLoader.h 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: A loader for Quake III maps (.bsp)
**/

#ifndef Q3MAPLOADER_H_INCLUDED
#define Q3MAPLOADER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ILoader.h"
#include "vector2.h"
#include "vector3.h"
#include "quake3.h"

namespace fire_engine
{

class Q3Map;
class string;
class ITexture;

/** A loader for Quake III BSP maps. Implements the ILoader interface. */
class _FIRE_ENGINE_API_ Q3MapLoader : public ILoader<Q3Map>
{
public:
	virtual ~Q3MapLoader();

	virtual Q3Map * load(const string& filename) const;

private:
	/** Swizzle a Quake III vector: set Y to Z, and set Z to -Y. */
	void swizzle(vector3f& vector) const;

	void swizzle(vector2f& vector) const;

	ITexture ** loadTextures(const q3::bsp_texture_t * q3textures, s32 num_textures) const;
};

}

#endif // Q3MAPLOADER_H_INCLUDED
