/**
* FILE:    AnimatedModelMD3.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: AnimatedModelMD3.cpp 115 2007-10-05 10:52:32Z jpaterso $
* PURPOSE: Implementation of the MD3 class.
**/

#include "AnimatedModelMD3.h"

namespace fire_engine
{

AnimatedModelMD3::AnimatedModelMD3(INode * parent, IAnimatedMesh * mesh)
	: AnimatedModel(parent, mesh), mMD3Parent(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::AnimatedModelMD3");
#endif
	mInterpolatedTags = new MD3QuaternionTag[getMD3Mesh()->getNumTags()];
}

AnimatedModelMD3::~AnimatedModelMD3()
{
	if (mMD3Parent)
		mMD3Parent->drop();
	delete [] mInterpolatedTags;
}

const AnimatedMeshMD3 * AnimatedModelMD3::getMD3Mesh() const
{
	return dynamic_cast<AnimatedMeshMD3*>(mMesh);
}

bool AnimatedModelMD3::attach(AnimatedModelMD3 * parent, const string& tagname)
{
	if (parent)
	{
		setParent(parent);
		mTagIndex = parent->getMD3Mesh()->getTagIndex(tagname);
		if (mTagIndex < 0)
			return false;
		mMD3Parent = parent;
		mMD3Parent->grab();
	}
	return true;
}

void AnimatedModelMD3::preRender(f64 time)
{
	updateAnimationInfo(time);
	for (s32 i = 0; i < getMD3Mesh()->getNumTags(); i++)
		mInterpolatedTags[i] = getMD3Mesh()->getTagTransform(i, mAnimInfo.mFrameCur,
			mAnimInfo.mFrameNext, mAnimInfo.mIpolTime);
	ISpaceNode::preRender(time);
	recalculateBoundingBox();
}

void AnimatedModelMD3::updateTransforms()
{
	if (mMD3Parent)
	{
		mRelativeTransform = mMD3Parent->mInterpolatedTags[mTagIndex].Rotation.getMatrix()*mRelativeOrientation;
		mRelativeTransform.translate(mMD3Parent->mInterpolatedTags[mTagIndex].Position+mRelativePosition);
	}
	else
	{
		mRelativeTransform = mRelativeOrientation;
		mRelativeTransform.translate(mRelativePosition);
	}
	mRelativeTransform.scale(mRelativeScale);
	if (mParent != 0)
		mWorldTransform = dynamic_cast<ISpaceNode*>(mParent)->getWorldTransform()*mRelativeTransform;
	else
		mWorldTransform = mRelativeTransform;
}

void AnimatedModelMD3::setMaterialProperty(EMATERIAL_PROPERTY prop, bool isset)
{
	AnimatedModel::setMaterialProperty(prop, isset);
	if (mMD3Parent)
		mMD3Parent->setMaterialProperty(prop, isset);
}

}
