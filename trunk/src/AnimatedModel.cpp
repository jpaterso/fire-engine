/**
 * FILE:    AnimatedModel.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedModel.cpp 115 2007-10-05 10:52:32Z jpaterso $
 * PURPOSE: Implementation of the AnimatedModel class.
**/

#include "AnimatedModel.h"
#include "IAnimatedMesh.h"
#include "Object.h"
#include "IRenderer.h"
#include "Vertex3.h"
#include "SceneManager.h"
#include "Camera.h"
#include "aabbox.h"

namespace fire_engine
{

AnimatedModel::AnimatedModel(INode * parent, IAnimatedMesh * mesh)
	: IModel(parent), mMesh(mesh)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::AnimatedModel");
#endif
	mAnimInfo.mFrameStart   = -1;
	mAnimInfo.mFrameEnd     = -1;
	mAnimInfo.mFrameCur     = -1;
	if (mMesh)
	{
		mMesh->grab();
		for (s32 i = 0; i < mMesh->getMeshBufferCount(); i++)
		{
			mMaterials.push_back(mMesh->getMeshBuffer(i)->getMaterial());
		}
	}
}

AnimatedModel::~AnimatedModel()
{
	if (mMesh)
	{
		mMesh->drop();
	}
}

void AnimatedModel::preRender(f64 time)
{
	updateAnimationInfo(time);
	ISpaceNode::preRender(time);
	recalculateBoundingBox();
}

void AnimatedModel::updateAnimationInfo(f64 time)
{
	mAnimInfo.mTimeCur = time;
	if (mAnimInfo.mTimeStart < 0.0)
	{
		mAnimInfo.mTimeStart = time;
	}
	else if ((mAnimInfo.mTimeCur-mAnimInfo.mTimeStart) > (1.0/mAnimInfo.mFrameFPS))
	{
		if (mAnimInfo.mFrameCur == mAnimInfo.mFrameEnd)
		{
			if (!mAnimInfo.mLooping)
			{
				mAnimInfo.mFrameCur = -1;
				return;
			}
			mAnimInfo.mFrameCur = mAnimInfo.mFrameStart;
		}
		else
		{
			mAnimInfo.mFrameCur++;
		}
		// Next frame starts after the previous frame
		mAnimInfo.mTimeStart += 1.0/mAnimInfo.mFrameFPS;
	}
	mAnimInfo.mFrameNext = (mAnimInfo.mFrameCur!=mAnimInfo.mFrameEnd) ?
		mAnimInfo.mFrameCur+1 : mAnimInfo.mFrameStart;
	mAnimInfo.mIpolTime = (f32)(mAnimInfo.mTimeCur-mAnimInfo.mTimeStart)*mAnimInfo.mFrameFPS;
}

s32 AnimatedModel::render(IRenderer * rd)
{
	s32 polyCount = 0;
	const Camera * camera = SceneManager::Get()->getActiveCamera();
	if (camera == nullptr)
	{
		return 0;
	}

	if (mAnimInfo.mFrameCur < 0)
	{
		return 0;
	}
	rd->setTransform(EMM_MODEL, mWorldTransform);

	// Check whether mesh is in frustum
	if (camera->calculateIntersection(getTransformedBoundingVolume()) != EFIT_OUTSIDE)
	{
		IMesh * mesh = mMesh->getMesh(mAnimInfo.mFrameCur, mAnimInfo.mFrameNext, mAnimInfo.mIpolTime);
		if (mesh != nullptr)
		{
			IMeshBuffer * imb = nullptr;
			for (s32 i = 0; i < mesh->getMeshBufferCount(); i++)
			{
				imb = mesh->getMeshBuffer(i);
				// Check whether mesh buffer is in frustum
				if (camera->calculateIntersection(mWorldTransform.applyTransformation(imb->getBoundingBox())) != EFIT_OUTSIDE)
				{
					rd->drawMeshBuffer(imb);
					polyCount += imb->getVertexCount();
					if (mShowDebugInformation)
					{
						polyCount += 12;
						rd->drawaabbox(imb->getBoundingBox(), Color32::YELLOW);
					}
				}
			}
		}
	}
	ISpaceNode::render(rd);
	return polyCount;
}

Material& AnimatedModel::getMaterial(s32 nr)
{
	return mMaterials[nr];
}

void AnimatedModel::setFrameLoop(s32 start, s32 end, bool loop)
{
	mAnimInfo.mFrameStart = start;
	mAnimInfo.mFrameCur   = start;
	mAnimInfo.mFrameEnd   = end;
	mAnimInfo.mFrameNext  = (start == end) ? start : start + 1;
	mAnimInfo.mFrameFPS   = mMesh->getFPS(start, end);
	mAnimInfo.mTimeStart  = -1.0;
	mAnimInfo.mTimeCur    = -1.0;
	mAnimInfo.mIpolTime   = 0.0f;
	mAnimInfo.mLooping    = loop;
}

s32 AnimatedModel::getCurrentFrame() const
{
	return mAnimInfo.mFrameCur;
}

void AnimatedModel::setAnimatedMesh(IAnimatedMesh * mesh)
{
	if (mMesh)
	{
		mMesh->drop();
	}
	mMesh = mesh;
	mMesh->grab();
}

IAnimatedMesh * AnimatedModel::getAnimatedMesh()
{
	return mMesh;
}

void AnimatedModel::setMaterialProperty(EMATERIAL_PROPERTY prop, bool isset)
{
	for (s32 i = 0; i < mMaterials.size(); i++)
	{
		mMaterials[i].setMaterialProperty(prop, isset);
	}
}

void AnimatedModel::recalculateBoundingBox()
{
	mBoundingBox = mMesh->getBoundingBox(mAnimInfo.mFrameCur, mAnimInfo.mFrameNext, mAnimInfo.mIpolTime);
}

}
