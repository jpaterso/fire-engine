/**
* FILE:    Camera.cpp
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: Camera.cpp 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: An implementation of the Camera class
**/

#include "Camera.h"
#include "Math.h"
#include "Device.h"
#include "IRenderer.h"
#include "MouseEvent.h"
#include "IWindowManager.h"

namespace fire_engine
{

Camera::Camera(IRenderer * renderer, INode * parent, const vector3f& pos, const vector3f& lookat,
	const vector3f& up)
	: IModel(parent), mZNear(1.0f), mZFar(10000.0f), mFOV(Math32::PI_ON_3),
	  mAspectRatio(4.0f/3.0f), mForwardVector(lookat-pos), mUpVector(up),
	  mAutoTrack(false), mProjectionNeedsRecalculated(true), mViewNeedsRecalculated(true)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Camera");
#endif
	mRelativePosition = pos;
	if (renderer)
		mAspectRatio = static_cast<f32>(renderer->getViewport().getWidth())/renderer->getViewport().getHeight();
	Device::Get()->getWindowManager()->addResizable(this);
}

Camera::~Camera()
{
	Device::Get()->getWindowManager()->removeResizable(this);
}

void Camera::recalculateProjectionMatrix()
{
	mProjectionMatrix.loadProjectionMatrixFOV(mFOV, mAspectRatio, mZNear, mZFar);
	mProjectionNeedsRecalculated = false;
}

void Camera::recalculateViewMatrix()
{
	mViewMatrix.loadLookAtMatrixRH(mRelativePosition, mRelativePosition+mForwardVector, mUpVector);
	mBoundingBox.reset();
	mBoundingBox.reset(mRelativePosition-vector3f(0.5f, 0.5f, 0.5f), mRelativePosition+vector3f(0.5f, 0.5f, 0.5f));
	mViewNeedsRecalculated = false;
}

void Camera::updateTransforms()
{
	ISpaceNode::updateTransforms();
	if (mProjectionNeedsRecalculated || mViewNeedsRecalculated)
		recalculateFrustumPlanes();
	if (mProjectionNeedsRecalculated)
		recalculateProjectionMatrix();
	if (mViewNeedsRecalculated)
		recalculateViewMatrix();
}

s32 Camera::render(IRenderer * renderer)
{
	renderer->setTransform(EMM_PROJECTION, mProjectionMatrix);
	renderer->setTransform(EMM_VIEW, mViewMatrix);
	ISpaceNode::render(renderer);
	return 0;
}

void Camera::recalculateFrustumPlanes()
{
	vector3f zAxis = -mForwardVector;
	vector3f xAxis = mUpVector*zAxis;
	zAxis.normalize();
	xAxis.normalize();
	vector3f yAxis = zAxis*xAxis;
	vector3f nearCenter = -zAxis*mZNear;

	mPlanes[EFP_NEAR].set(nearCenter+mRelativePosition, -zAxis);
	mPlanes[EFP_FAR].set(-zAxis*mZFar+mRelativePosition, zAxis);

	f32 hNearHeight = mZNear*Math32::Tan(0.5f*mFOV);
	f32 hNearWidth = hNearHeight*mAspectRatio;
	vector3f normal, tmp;

	tmp = nearCenter + xAxis*hNearWidth;// - mRelativePosition;
	normal = yAxis * tmp;
	normal.normalize();
	mPlanes[EFP_RIGHT].set(tmp+mRelativePosition, normal);

	tmp = nearCenter - xAxis*hNearWidth;// - mRelativePosition;
	normal = tmp * yAxis;
	normal.normalize();
	mPlanes[EFP_LEFT].set(tmp+mRelativePosition, normal);

	tmp = nearCenter + yAxis*hNearHeight;// - mRelativePosition;
	normal = tmp * xAxis;
	normal.normalize();
	mPlanes[EFP_TOP].set(tmp+mRelativePosition, normal);

	tmp = nearCenter - yAxis*hNearHeight;// - mRelativePosition;
	normal = xAxis * tmp;
	normal.normalize();
	mPlanes[EFP_BOTTOM].set(tmp+mRelativePosition, normal);
}

void Camera::onResize(const dimension2i& newSize)
{
	this->setAspectRatio(static_cast<f32>(newSize.getWidth())/newSize.getHeight());
}

const AABoundingBoxf& Camera::getBoundingVolume() const
{
	return mBoundingBox;
}

}
