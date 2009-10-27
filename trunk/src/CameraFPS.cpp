/**
* FILE:    CameraFPS.cpp
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: CameraFPS.cpp 116 2007-10-05 13:22:45Z jpaterso $
* PURPOSE: A First Person Shooter (FPS) style camera.
**/

#include "CameraFPS.h"
#include "KeyEvent.h"
#include "IWindowManager.h"
#include "vector2.h"

namespace fire_engine
{

CameraFPS::CameraFPS(IRenderer * renderer, INode * parent, ICursor * cursor, const vector3f& pos,
	const vector3f& lookat, const vector3f& up)
	: Camera(renderer, parent, pos, lookat, up), mCumulativeRotations(0.0f, 0.0f, 0.0f),
	  mCursor(cursor), mKeyToMouvement(0)
{
	mMoveSpeed = 1.5f;
	mRotationSpeed = 1.5f;
	mOriginalFwdVector = mForwardVector;
	mOriginalFwdVector.normalize();
	mPitchRotationMin = (-Math32::PI_ON_2+0.05f)/mRotationSpeed;
	mPitchRotationMax = (Math32::PI_ON_2-0.05f)/mRotationSpeed;

	if (mCursor)
		mCursor->grab();

	{
		mMoveFunction[ 0] = &CameraFPS::moveForward;
		mMoveFunction[ 1] = &CameraFPS::moveBackward;
		mMoveFunction[ 2] = &CameraFPS::moveLeft;
		mMoveFunction[ 3] = &CameraFPS::moveRight;
		mMoveFunction[ 4] = &CameraFPS::strafeLeft;
		mMoveFunction[ 5] = &CameraFPS::strafeRight;
		mMoveFunction[ 6] = &CameraFPS::lookUp;
		mMoveFunction[ 7] = &CameraFPS::lookDown;
		mMoveFunction[ 8] = &CameraFPS::leanLeft;
		mMoveFunction[ 9] = &CameraFPS::leanRight;
		mMoveFunction[10] = &CameraFPS::jump;
	}

	allKeysUp();
}

CameraFPS::~CameraFPS()
{
	if (mKeyToMouvement)
		delete mKeyToMouvement;
	if (mCursor)
		mCursor->drop();
}

void CameraFPS::onKeyEvent(KeyEvent& kevent)
{
	if (mKeyToMouvement != 0)
	{
		s32 * index = (s32*)mKeyToMouvement->find(kevent.getKey());
		if (index != 0)
			mMovementKeys[*index] = true;
	}
}

void CameraFPS::onMouseEvent(MouseEvent& mevent)
{
	return;
}

void CameraFPS::updateTransforms()
{
	if (mCursor)
	{
		if (mCursor->getRelativePosition() != vector2f(0.5f, 0.5f))
		{
			vector2f relative = mCursor->getRelativePosition() - vector2f(0.5f, 0.5f);
			vector3f rot_angles(-relative.getY(), -relative.getX(), 0.0f);
			mCumulativeRotations += rot_angles;
			mCumulativeRotations.clamp(EX_AXIS, mPitchRotationMin, mPitchRotationMax);
			mForwardVector = matrix4f::BuildEulerRotationMatrix(mCumulativeRotations*mRotationSpeed).applyTransformation(mOriginalFwdVector);
			mCursor->setRelativePosition(vector2f(0.5f, 0.5f));
			mViewNeedsRecalculated = true;
		}
	}
	if (mKeyToMouvement != 0)
	{
		for (s32 i = 0; i < EMT_MOVEMENT_COUNT; i++)
		{
			if (mMovementKeys[i])
			{
				mMovementKeys[i] = false;
				// Nice syntax!
				(this->*mMoveFunction[i])();
			}
		}
	}
	Camera::updateTransforms();
}

void CameraFPS::allKeysUp()
{
	for (s32 i = 0; i < EMT_MOVEMENT_COUNT; i++)
		mMovementKeys[i] = false;
}

void CameraFPS::moveForward()
{
	vector3f direction = mForwardVector;
	direction.setY(0.0f);
	direction.normalize();
	direction *= mMoveSpeed;
	mRelativePosition += direction;
	mViewNeedsRecalculated = true;
}

void CameraFPS::moveBackward()
{
	vector3f direction = mForwardVector;
	direction.setY(0.0f);
	direction.normalize();
	direction *= mMoveSpeed;
	mRelativePosition -= direction;
	mViewNeedsRecalculated = true;
}

void CameraFPS::moveLeft()
{
}

void CameraFPS::moveRight()
{
}

void CameraFPS::strafeLeft()
{
	vector3f forward = mForwardVector;
	vector3f right = forward.cross(mUpVector);
	right.setY(0.0f);
	right.normalize();
	mRelativePosition -= right * mMoveSpeed;
	mViewNeedsRecalculated = true;
}

void CameraFPS::strafeRight()
{
	vector3f forward = mForwardVector;
	vector3f right = forward.cross(mUpVector);
	right.setY(0.0f);
	right.normalize();
	mRelativePosition += right * mMoveSpeed;
	mViewNeedsRecalculated = true;
}

void CameraFPS::lookUp()
{
}

void CameraFPS::lookDown()
{
}

void CameraFPS::leanLeft()
{
}

void CameraFPS::leanRight()
{
}

void CameraFPS::jump()
{
	mRelativePosition += mUpVector*mMoveSpeed;
	mViewNeedsRecalculated = true;
}

}
