/**
 * FILE:    ITexture.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ITexture.h 112 2007-09-25 23:37:26Z jpaterso $$
 * PURPOSE: Interface for a texture
**/

#ifndef ITEXTURE_H_INCLUDED
#define ITEXTURE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "Image.h"
#include "Object.h"

namespace fire_engine
{

/** Flags to use when creating a texture */
enum ETEXTURE_CREATION_FLAGS
{
	ETCF_HAS_MIPMAPS = 0x01,
	ETCF_AUTO_GENERATE_MIPMAPS = 0x02
};

class _FIRE_ENGINE_API_ ITexture : public virtual Object
{
public:
	virtual ~ITexture(void)
	{
		if (mImage != 0x00)
		{
			mImage->drop();
		}
	}

	virtual void update(const dimension2i& dim) = 0;

protected:
	Image *     mImage;
	dimension2i mDimension;
	u32         mCreationFlags;
};

}

#endif // ITEXTURE_H_INCLUDED
