/**
* FILE:    SkyBox.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: SkyBox.h 116 2007-10-05 13:22:45Z jpaterso $
* PURPOSE: A SkyBox, simulating a world around the camera.
**/

#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ISpaceNode.h"
#include "aabbox.h"
#include "Vertex3.h"
#include "Material.h"
#include "SceneManager.h"

namespace fire_engine
{

class IRenderer;
class ITexture;
class String;

/** A SkyBox: a cube surrounding the entire world, with optional internal textures that
 can simulate a world beyond the scene. */
class _FIRE_ENGINE_API_ SkyBox : public ISpaceNode
{
public:
	/** Constructor. */
	SkyBox();

	/** Construct a sky box with 6 textures. */
	SkyBox(const IRenderer * renderer, const String& ft, const String& bk, const String& up,
		const String& dn, const String& lt, const String& rt);

	/** Destructor. */
	virtual ~SkyBox();

	/** Renders the sky box. */
	virtual s32 render(IRenderer * renderer);

private:
	Material    mMaterial;
	ITexture ** mTextures;
	bool        mTexturesLoaded;
	Vertex3 *   mVertices;
	u32 *       mIndices;
};

}

#endif // SKYBOX_H_INCLUDED
