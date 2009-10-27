/**
* FILE:    CameraFPS.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: CameraFPS.h 112 2007-09-25 23:37:26Z jpaterso $
* PURPOSE: A First Person Shooter (FPS) style camera.
**/

#ifndef CAMERA_FPS_H_INCLUDED
#define CAMERA_FPS_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Camera.h"
#include "HashTable.h"
#include "KeyEvent.h"

namespace fire_engine
{

class ICursor;

class _FIRE_ENGINE_API_ CameraFPS : public Camera
{
public:
	CameraFPS(IRenderer * renderer,
		INode * parent         = 0,
		ICursor * cursor       = 0,
		const vector3f& pos    = vector3f(0.0f, 0.0f, 0.0f),
		const vector3f& lookat = vector3f(0.0f, 0.0f, -1.0f),
		const vector3f& up     = vector3f(0.0f, 1.0f, 0.0f));

	~CameraFPS();

	virtual void onKeyEvent(KeyEvent& kevent);

	virtual void onMouseEvent(MouseEvent& mevent);

	inline void setMoveKeys(HashTable<KeyEvent::EKEY_CODE, EMOVEMENT_TYPE> * keyToMouvement);

private:
	f32         mMoveSpeed;
	f32         mRotationSpeed;
	vector3f    mOriginalFwdVector;
	vector3f    mCumulativeRotations;
	f32         mPitchRotationMin, mPitchRotationMax;

	/** Control of the cursor, which we use to keep it centered, but move the world around
	 instead. */
	ICursor * mCursor;

	/** Dealing with movement: we keep track of keys that can represent movement,
	 we have a hash table mapping EKEY_CODEs to EMOVEMENT_TYPEs, and finally
	 an array of function pointers representing the various movement functions. */
	bool      mMovementKeys[EMT_MOVEMENT_COUNT];
	HashTable<KeyEvent::EKEY_CODE, EMOVEMENT_TYPE> * mKeyToMouvement;
	void (CameraFPS::*mMoveFunction[EMT_MOVEMENT_COUNT])(void);

	virtual void updateTransforms();

	void allKeysUp();

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void strafeLeft();
	void strafeRight();
	void lookUp();
	void lookDown();
	void leanLeft();
	void leanRight();
	void jump();
};

inline void CameraFPS::setMoveKeys(HashTable<KeyEvent::EKEY_CODE, EMOVEMENT_TYPE> * keyToMouvement)
{
	if (mKeyToMouvement != 0)
		delete mKeyToMouvement;
	mKeyToMouvement = keyToMouvement;
}

}

#endif // CAMERA_FPS_H_INCLUDED
