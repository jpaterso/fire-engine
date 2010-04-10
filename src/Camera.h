/**
* FILE:    Camera.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: Camera.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: A 'default' camera that can be used in the scene.
**/

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "IModel.h"
#include "ISpaceNode.h"
#include "IEventReceiver.h"
#include "vector3.h"
#include "matrix4.h"
#include "dimension2.h"
#include "ViewFrustum.h"
#include "IResizable.h"
#include "aabbox.h"

namespace fire_engine
{

class IRenderer;

/** The various movements that can be performed. */
enum EMOVEMENT_TYPE
{
	EMT_FORWARD = 0x00,
	EMT_BACKWARD,
	EMT_LEFT,
	EMT_RIGHT,
	EMT_STRAFE_LEFT,
	EMT_STRAFE_RIGHT,
	EMT_LOOK_UP,
	EMT_LOOK_DOWN,
	EMT_LEAN_LEFT,
	EMT_LEAN_RIGHT,
	EMT_JUMP,
	EMT_MOVEMENT_COUNT
};

/** A 'Camera' in 3-dimension space, that defines the position that the scene
 is viewed from, as well as the way it is seen (whether perspective is used,
 the field-of-view that can be visible). It inherits from the ViewFrustum class
 so that some basic culling can be performed, which increases performance as
 invisible items are not rendered. */
class _FIRE_ENGINE_API_ Camera : public IModel, public IEventReceiver, public ViewFrustum, public IResizable
{
public:
	/** Constructor. */
	Camera(IRenderer * renderer, INode * parent,
		const vector3f& pos, const vector3f& lookat, const vector3f& up);

	/** Destructor. */
	virtual ~Camera();

	/** Sets the new target that the camera should look at. */
	inline void setNewTarget(const vector3f& newLookAt);

	/** Changes the target that the camera is 'looking at' by a certain amount. */
	inline void translateTarget(const vector3f& by);

	/** Sets the new up vector for the camera. */
	inline void setUp(const vector3f& newUp);

	/** Changes the up vector by a certain amount. */
	inline void translateUp(const vector3f& by);

	/** Sets the projection matrix for this camera. */
	inline void setProjectionMatrix(const matrix4f& projMat);

	/** Returns the projection matrix for this camera. */
	inline const matrix4f& getProjectionMatrix() const;

	/** Sets the cutoff values for this camera. */
	inline void setCutoffValues(f32 zNear, f32 zFar);

	/** Sets the new near value for this camera. */
	inline void setNearValue(f32 zNear);

	/** Sets the new far value for this camera. */
	inline void setFarValue(f32 zFar);

	/** Sets the width of the field of view (FOV) for this camera. The FOV must
	 be given in radians. */
	inline void setFOV(f32 fovRadians);

	/** Returns the current field-of-view of the camera (in radians). */
	inline f32 getFOV() const;

	/** Sets the aspect ratio for this camera. */
	inline void setAspectRatio(f32 aspectRatio);

	/** Sets the aspect ratio for this camera. */
	inline void setAspectRatio(s32 width, s32 height);

	/** Sets auto-tracking on or off. */
	inline void setAutoTrack(bool at);

	/** Returns whether auto-track is enabled or not. */
	inline bool isAutoTrackEnabled() const;

	/** 'renders' the camera - sets up the appropriate transforms in the renderer. */
	virtual s32 render(IRenderer * renderer);

	/** Called when the window is resized. */
	virtual void onResize(const dimension2i& newSize);

	/** Returns the bounding volume for the camera.
	 The bounding volume for a camera depicts the 'volume' that the camera occupies. Its only
	 purpose is to be used so that, for example, the camera is blocked by walls and solid items. */
	virtual const aabboxf& getBoundingVolume() const;

protected:
	/** The near clip plane, and the far clip plane. Anything with a distance not within
	 these bounds from the camera will be clipped. */
	f32 mZNear, mZFar;
	/** The Field-of-View: controls how 'wide' the view is. A good number is PI/2 (90 degrees). */
	f32 mFOV;
	/** The aspect ratio: the ratio of the width of the screen over the height of the screen. */
	f32 mAspectRatio;

	matrix4f mViewMatrix;
	matrix4f mProjectionMatrix;

	// The two basic vectors for the camera - the third one can be derived
	vector3f mForwardVector;
	vector3f mUpVector;

	bool     mAutoTrack;
	bool     mProjectionNeedsRecalculated;
	bool     mViewNeedsRecalculated;

	/** Recalculates the projection matrix used by the camera. */
	void recalculateProjectionMatrix();

	/** Recalculates the view matrix used by the camera. */
	void recalculateViewMatrix();

	/** Updates the transforms for the camera. */
	virtual void updateTransforms();

	/** Recalculates all six planes of the viewing frustum. */
	void recalculateFrustumPlanes();

};

inline void Camera::setNewTarget(const vector3f& newTarget)
{
	mForwardVector = newTarget-mRelativePosition;
	mViewNeedsRecalculated = true;
}

inline void Camera::translateTarget(const vector3f& by)
{
	mForwardVector = mForwardVector+by;
	mViewNeedsRecalculated = true;
}

inline void Camera::setUp(const vector3f& newUp)
{
	mUpVector = newUp;
	mViewNeedsRecalculated = true;
}

inline void Camera::translateUp(const vector3f& by)
{
	mUpVector = mUpVector+by;
	mViewNeedsRecalculated = true;
}

inline void Camera::setProjectionMatrix(const matrix4f& projMat)
{
	mProjectionMatrix = projMat;
}

inline const matrix4f& Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}

inline void Camera::setCutoffValues(f32 zNear, f32 zFar)
{
	mZNear = zNear;
	mZFar  = zFar;
	mProjectionNeedsRecalculated = true;
}

inline void Camera::setNearValue(f32 zNear)
{
	mZNear = zNear;
	mProjectionNeedsRecalculated = true;
}

inline void Camera::setFarValue(f32 zFar)
{
	mZFar = zFar;
	mProjectionNeedsRecalculated = true;
}

inline void Camera::setFOV(f32 fovRadians)
{
	mFOV = fovRadians;
	mProjectionNeedsRecalculated = true;
}

inline f32 Camera::getFOV() const
{
	return mFOV;
}

inline void Camera::setAspectRatio(f32 aspectRatio)
{
	mAspectRatio = aspectRatio;
	mProjectionNeedsRecalculated = true;
}

inline void Camera::setAspectRatio(s32 width, s32 height)
{
	mAspectRatio = static_cast<f32>(width)/height;
	mProjectionNeedsRecalculated = true;
}

inline void Camera::setAutoTrack(bool at)
{
	mAutoTrack = at;
}

inline bool Camera::isAutoTrackEnabled() const
{
	return mAutoTrack;
}

}

#endif // CAMERA_H_INCLUDED
