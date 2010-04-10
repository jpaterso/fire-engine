/**
 * FILE:    ISpaceNode.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ISpaceNode.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of the ISpaceNode class.
**/

#include "ISpaceNode.h"

namespace fire_engine
{

ISpaceNode::ISpaceNode(INode * parent)
	: INode(parent), mWorldTransform(matrix4f::IDENTITY_MATRIX),
	  mRelativeTransform(matrix4f::IDENTITY_MATRIX), mRelativeScale(1.0f, 1.0f, 1.0f),
	  mRelativePosition(0.0f, 0.0f, 0.0f), mRelativeOrientation(matrix4f::IDENTITY_MATRIX),
	  m_animator(0), mShowDebugInformation(false), mVisible(true)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::ISpaceNode");
#endif
}

ISpaceNode::~ISpaceNode()
{
}

void ISpaceNode::attachAnimator(ISpaceNodeAnimator * animator)
{
	//TODO: grab() drop()
	m_animator = animator;
}

void ISpaceNode::detachAnimator()
{
	if (m_animator != 0)
	{}
}

bool ISpaceNode::hasAnimatorAttached() const
{
	return m_animator != 0;
}

ISpaceNodeAnimator * ISpaceNode::getAnimator()
{
	return m_animator;
}

void ISpaceNode::preRender(f64 time)
{
	updateTransforms();
	for (list<INode*>::iterator it = mChildren->begin(); it != mChildren->end(); it++)
		dynamic_cast<ISpaceNode*>(*it)->preRender(time);
}

s32 ISpaceNode::render(IRenderer * renderer)
{
	return 0;
}

void ISpaceNode::setShowDebugInformation(bool sdi)
{
	mShowDebugInformation = sdi;
}

bool ISpaceNode::showDebugInformation() const
{
	return mShowDebugInformation;
}

void ISpaceNode::setVisible(bool v)
{
	mVisible = v;
}

bool ISpaceNode::isVisible() const
{
	return mVisible;
}

const matrix4f& ISpaceNode::getWorldTransform() const
{
	return mWorldTransform;
}

void ISpaceNode::updateTransforms()
{
	mRelativeTransform = mRelativeOrientation;
	mRelativeTransform.translate(mRelativePosition);
	mRelativeTransform.scale(mRelativeScale);
	if (mParent != 0)
		mWorldTransform = dynamic_cast<ISpaceNode*>(mParent)->getWorldTransform()*mRelativeTransform;
	else
		mWorldTransform = mRelativeTransform;
}

void ISpaceNode::setRelativePosition(const vector3f& translation)
{
	mRelativePosition = translation;
}

const vector3f& ISpaceNode::getRelativePosition() const
{
	return mRelativePosition;
}

void ISpaceNode::translate(const vector3f& translation)
{
	mRelativePosition = mRelativePosition+translation;
}

void ISpaceNode::setRelativeScale(const vector3f& factor)
{
	mRelativeScale = factor;
}

const vector3f& ISpaceNode::getRelativeScale() const
{
	return mRelativeScale;
}

void ISpaceNode::scale(const vector3f& factor)
{
	mRelativeScale = mRelativeScale+factor;
}

void ISpaceNode::setRelativeOrientation(const matrix4f& orientation)
{
	mRelativeOrientation = orientation;
}

const matrix4f& ISpaceNode::getRelativeOrientation() const
{
	return mRelativeOrientation;
}

void ISpaceNode::rotate(const matrix4f& rotation)
{
	mRelativeOrientation = mRelativeOrientation*rotation;
}

}
