/**
 *  $Id: SceneManager.cpp 116 2007-10-05 13:22:45Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Read SceneManager.h for more information
**/

#include "SceneManager.h"
#include "IRenderer.h"
#include "Logger.h"
#include "Image.h"
#include "OpenGLTexture.h"
#include "ISpaceNode.h"
#include "IAnimatedMesh.h"
#include "AnimatedModel.h"
#include "AnimatedModelMD3.h"
#include "Device.h"
#include "Light.h"
#include "LightSpaceNode.h"
#include "MediaManager.h"
#include "FPSCalculator.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "IWindowManager.h"
#include "SkyBox.h"
#include "IModel.h"

namespace fire_engine
{

SceneManager * SceneManager::mInstance = 0;

SceneManager::SceneManager(IRenderer * rd)
	: mRenderer(rd), mActiveCamera(0), mSkyBox(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::SceneManager");
#endif
	if (rd == 0)
		Logger::Get()->log(ES_CRITICAL, "SceneManager", "initializing with NULL renderer");
	mRenderer->grab();
	mSpaceRoot = new ISpaceNode(0);
	mAmbientLight = Color32(0.35f, 0.35f, 0.35f, 1.0f);
	mScreenshotInfo.mWantScreenshot = false;
	mTimer.start();
}

SceneManager * SceneManager::Create(IRenderer * rd)
{
	if (mInstance == 0)
		mInstance = new SceneManager(rd);
	return (mInstance);
}

SceneManager::~SceneManager()
{
	mSpaceRoot->releaseTree();
	mSpaceRoot->drop();
	mRenderer->drop();
}

SceneManager * SceneManager::Get()
{
	return mInstance;
}

void SceneManager::draw()
{
	s32 polys = 0;
	mSpaceRoot->preRender(mTimer.getElapsedTimeSeconds());

	mRenderer->setTransform(EMM_VIEW, matrix4f::IDENTITY_MATRIX);
	mRenderer->setTransform(EMM_MODEL, matrix4f::IDENTITY_MATRIX);
	mRenderer->setTransform(EMM_TEXTURE, matrix4f::IDENTITY_MATRIX);

	if (mActiveCamera)
		mActiveCamera->render(mRenderer);

	if (mSkyBox)
		polys += mSkyBox->render(mRenderer);

	mRenderer->setAmbientLight(mAmbientLight);
	for (s32 i = 0; i < mLights.size(); i++)
		if (mLights.at(i)->isVisible())
			mLights.at(i)->render(mRenderer);

	for (s32 i = 0; i < mSolidNodes.size(); i++)
		if (mSolidNodes.at(i)->isVisible())
			polys += mSolidNodes.at(i)->render(mRenderer);

	// Take a screenshot if one has been requested
	if (mScreenshotInfo.mWantScreenshot)
	{
		Image * im = mRenderer->screenshot();
		MediaManager::Get()->write<Image>(mScreenshotInfo.mScreenshotFilename, im);
		mScreenshotInfo.mWantScreenshot = false;
	}

	Device::Get()->_getFPSCalculator()->registerFrameDrawn(polys);
}

AnimatedModel * SceneManager::addAnimatedMesh(IAnimatedMesh * mesh)
{
	AnimatedModel * model = 0x00;

	switch (mesh->getAnimatedMeshType())
	{
	case EAMT_MD2:
		model = new AnimatedModel(mSpaceRoot, mesh);
		break;

	case EAMT_MD3:
		model = new AnimatedModelMD3(mSpaceRoot, mesh);
		break;
	}

	mSolidNodes.push_back(model);
	return model;
}

LightSpaceNode * SceneManager::addDynamicLight(Light * light)
{
	LightSpaceNode * lsn = new LightSpaceNode(light);
	mRenderer->addDynamicLight(light);
	lsn->setParent(mSpaceRoot);
	mLights.push_back(lsn);
	return lsn;
}

SkyBox * SceneManager::addSkyBox(const string& ft, const string& bk, const string& up,
	const string& dn, const string& lt, const string& rt)
{
	SkyBox * sk = new SkyBox(mRenderer, ft, bk, up, dn, lt, rt);
	if (mSkyBox != 0)
	{
		mSkyBox->setParent(0);
		mSkyBox->drop();
	}
	mSkyBox = sk;
	if (mSkyBox != 0)
		mSkyBox->setParent(mSpaceRoot);

	return mSkyBox;
}

Camera * SceneManager::addCamera(const vector3f& position, const vector3f& target)
{
	Camera * camera = new CameraFPS(mRenderer, mSpaceRoot, IWindowManager::Get()->getCursor(),
		position, target, vector3f(0.0f, 1.0f, 0.0f));
	if (mActiveCamera)
		mActiveCamera->drop();
	mActiveCamera = camera;
	Device::Get()->addEventReceiver(camera);
	return camera;
}

const Camera * SceneManager::getActiveCamera() const
{
	return mActiveCamera;
}

void SceneManager::registerScreenShotDemand(const string& filename)
{
	mScreenshotInfo.mWantScreenshot     = true;
	mScreenshotInfo.mScreenshotFilename = filename;
}

ISpaceNode * SceneManager::getRoot()
{
	return mSpaceRoot;
}

} // namespace fire_engine
