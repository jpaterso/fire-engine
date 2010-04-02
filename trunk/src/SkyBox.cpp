/**
 * FILE:    SkyBox.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: SkyBox.cpp 117 2007-10-06 00:31:15Z jpaterso $
 * PURPOSE:
**/

#include "SkyBox.h"
#include "IRenderer.h"
#include "ITexture.h"
#include "string.h"
#include "Logger.h"
#include "Camera.h"

namespace fire_engine
{

SkyBox::SkyBox()
	: mTextures(0), mTexturesLoaded(false), mVertices(0), mIndices(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::SkyBox");
#endif
}

SkyBox::SkyBox(const IRenderer * renderer, const string& ft, const string& bk, const string& up,
	const string& dn, const string& lt, const string& rt)
	: mTexturesLoaded(true)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::SkyBox");
#endif
	mTextures = new ITexture*[6];
	{
		mTextures[0] = renderer->createTexture(ft, nullptr);
		mTextures[1] = renderer->createTexture(bk, nullptr);
		mTextures[2] = renderer->createTexture(up, nullptr);
		mTextures[3] = renderer->createTexture(dn, nullptr);
		mTextures[4] = renderer->createTexture(lt, nullptr);
		mTextures[5] = renderer->createTexture(rt, nullptr);
	}
	for (s32 i = 0; i < 6; i++)
	{
		if (mTextures[i] == 0)
		{
			mTexturesLoaded = false;
			break;
		}
	}
	if (!mTexturesLoaded)
	{
		Logger::Get()->log(ES_HIGH, "SkyBox", "An error occurred while loading sky box textures");
	}

	// Create SkyBox vertices
	{
		mVertices = new Vertex3[24];

		// Cube doesn't work if it's a unit cube, so make it slightly bigger
		f32 one = 10.0f;

		// Front
		mVertices[ 0] = Vertex3(vector3f(-one, -one, -one), vector3f(0.0f, 0.0f, 1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));
		mVertices[ 1] = Vertex3(vector3f(one, -one, -one), vector3f(0.0f, 0.0f, 1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[ 2] = Vertex3(vector3f(one, one, -one), vector3f(0.0f, 0.0f, 1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));
		mVertices[ 3] = Vertex3(vector3f(-one, one, -one), vector3f(0.0f, 0.0f, 1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));

		// Back
		mVertices[ 4] = Vertex3(vector3f(one, -one, one), vector3f(0.0f, 0.0f, -1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));;
		mVertices[ 5] = Vertex3(vector3f(-one, -one, one), vector3f(0.0f, 0.0f, -1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[ 6] = Vertex3(vector3f(-one, one, one), vector3f(0.0f, 0.0f, -1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));
		mVertices[ 7] = Vertex3(vector3f(one, one, one), vector3f(0.0f, 0.0f, -1.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));;

		// Top
		mVertices[ 8] = Vertex3(vector3f(one, one, one), vector3f(0.0f, -1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));
		mVertices[ 9] = Vertex3(vector3f(-one, one, one), vector3f(0.0f, -1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));
		mVertices[10] = Vertex3(vector3f(-one, one, -one), vector3f(0.0f, -1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[11] = Vertex3(vector3f(one, one, -one), vector3f(0.0f, -1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));

		// Bottom
		mVertices[12] = Vertex3(vector3f(-one, -one, one), vector3f(0.0f, 1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));
		mVertices[13] = Vertex3(vector3f(one, -one, one), vector3f(0.0f, 1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));
		mVertices[14] = Vertex3(vector3f(one, -one, -one), vector3f(0.0f, 1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[15] = Vertex3(vector3f(-one, -one, -one), vector3f(0.0f, 1.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));

		// Left
		mVertices[20] = Vertex3(vector3f(-one, -one, one), vector3f(1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));
		mVertices[21] = Vertex3(vector3f(-one, -one, -one), vector3f(1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[22] = Vertex3(vector3f(-one, one, -one), vector3f(1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));
		mVertices[23] = Vertex3(vector3f(-one, one, one), vector3f(1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));

		// Right
		mVertices[16] = Vertex3(vector3f(one, -one, -one), vector3f(-1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 1.0f));
		mVertices[17] = Vertex3(vector3f(one, -one, one), vector3f(-1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 1.0f));
		mVertices[18] = Vertex3(vector3f(one, one, one), vector3f(-1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(1.0f, 0.0f));
		mVertices[19] = Vertex3(vector3f(one, one, -one), vector3f(-1.0f, 0.0f, 0.0f), Color8(0xFF, 0xFF, 0xFF, 0xFF), vector2f(0.0f, 0.0f));
	}

	// Create the indices for the sky box
	{
		mIndices = new u32[4];
		mIndices[0] = 0;
		mIndices[1] = 1;
		mIndices[2] = 2;
		mIndices[3] = 3;
	}

	mMaterial.setMaterialProperty(EMP_LIGHTING, false);
	mMaterial.setMaterialProperty(EMP_WRITE_TO_Z_BUFFER, false);
}

SkyBox::~SkyBox()
{
	if (mTextures != 0)
	{
		for (s32 i = 0; i < 6; i++)
		{
			if (mTextures[i] != 0)
			{
				mTextures[i]->drop();
			}
		}
	}
	delete [] mTextures;
	if (mVertices != 0)
		delete [] mVertices;
	if (mIndices != 0)
		delete [] mIndices;
}

s32 SkyBox::render(IRenderer * renderer)
{
	const Camera * camera = SceneManager::Get()->getActiveCamera();

	//TODO: define nullptr
	if (camera == 0x00)
		return 0x00;

	mWorldTransform.setTranslation(camera->getRelativePosition());
	renderer->setTransform(EMM_MODEL, mWorldTransform);

	renderer->setMaterial(mMaterial);

	for (s32 i = 0; i < 6; i++)
	{
		renderer->setTexture(0, mTextures[i]);
		renderer->drawIndexedPrimitiveList(EPT_TRIANGLE_FAN, 4, &mVertices[i*4], mIndices);
	}
	return 12;
}

}
