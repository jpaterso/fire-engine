/**
 * FILE:    AnimatedModel.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedModel.h 115 2007-10-05 10:52:32Z jpaterso $
 * PURPOSE: An extension to an entity: an entity that has multiple frames,
 *          and that can be animated.
**/

#ifndef IANIMATEDMODEL_H_INCLUDED
#define IANIMATEDMODEL_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "IModel.h"
#include "Array.h"
#include "Material.h"

namespace fire_engine
{

class IAnimatedMesh;
class IRenderer;
class IMesh;
class SceneManager;
class INode;

/** A model based on an IAnimatedMesh, that can be animated in 3D Space. */
class _FIRE_ENGINE_API_ AnimatedModel : public IModel
{
public:
	/** Construct an AnimatedModel, with some IAnimatedMesh. */
	AnimatedModel(INode * parent, IAnimatedMesh * mesh = 0);

	/** Destructor. */
	virtual ~AnimatedModel();

	/** Set the frame loop for this AnimatedModel. */
	void setFrameLoop(s32 start, s32 end, bool loop = true);

	/** Get the current frame that is being rendered. */
	s32 getCurrentFrame() const;

	/** Set a new IAnimatedMesh for this AnimatedModel. */
	void setAnimatedMesh(IAnimatedMesh * mesh);

	/** Get the IAnimatedMesh for this AnimatedModel. */
	IAnimatedMesh * getAnimatedMesh();

	//! Inherited from IRenderable
	virtual void preRender(f64 time);
	virtual s32 render(IRenderer * rd);

	/** Returns the Material based on the 0-indexed nr.
	 \param nr The index of the Material. */
	virtual Material& getMaterial(s32 nr);

	/** Set a certain material property for all the IMeshBuffers contained within
	 the IAnimatedMesh. */
	virtual void setMaterialProperty(EMATERIAL_PROPERTY prop, bool isset);

protected:
	IAnimatedMesh *  mMesh;
	Array<Material>  mMaterials;

	/** Contains information about the current rendering state of the model. */
	typedef struct
	{
		s16  mFrameStart;
		s16  mFrameEnd;
		s16  mFrameCur;
		s16  mFrameNext;
		s16  mFrameFPS;
		f64  mTimeStart;
		f64  mTimeCur;
		f32  mIpolTime;
		bool mLooping;
	} model_animation_t;

	model_animation_t mAnimInfo;

	/** UPdates the internal animation information, current frame, interpolation etc... */
	void updateAnimationInfo(f64 time);

	/** Recalculates the models bounding box for the current frame. */
	void recalculateBoundingBox();

};

}


#endif // IANIMATEDMODEL_H_INCLUDED
