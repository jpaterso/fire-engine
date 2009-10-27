/**
* FILE:    AnimatedModelMD3.h
* AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
* RCS ID:  $Id: AnimatedModelMD3.h 115 2007-10-05 10:52:32Z jpaterso $
* PURPOSE: An extension to the AnimatedModel class, to add support for bone modelling in MD3
*          models.
**/

#ifndef ANIMATEDMODELMD3_H_INCLUDED
#define ANIMATEDMODELMD3_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "AnimatedModel.h"
#include "AnimatedMeshMD3.h"

namespace fire_engine
{

// Forward declarations
class AnimatedMeshMD3;
class INode;

/** A class used model an item in space, that was loaded from a Quake III .MD3 model.
 We extend the AnimatedModel class, as MD3 models can be hierarchical, and can have
 bones, ie. different parts to a same model. This way for example, a body can have
 3 separate parts: a set of legs, a torso and a head, all of which can move
 independently of the others. The AnimatedModelMD3 class can handle this type of
 animation. The only catch is that instead of calling INode's setParent() method,
 when setting a parent for a model, you must call AnimatedModelMD3's attach() method,
 with a tagname. */
class _FIRE_ENGINE_API_ AnimatedModelMD3 : public AnimatedModel
{
public:
	/** Constructor - same as AnimatedModel because no MD3 specific stuff is done here. */
	AnimatedModelMD3(INode * parent, IAnimatedMesh * mesh = 0);

	/** Destructor. */
	virtual ~AnimatedModelMD3();

	/** Attach a parent to this model. In the case of a hierarchical MD3 model, this function
	 should be used instead of INode::setParent().
	 \param parent  A pointer to the parents MD3 model.
	 \param tagname The tag to attach itself to.
	 \return        true if the mode was correctly attached to its parent (ie. if tagname was a
	                valid tag), false otherwise. */
	bool attach(AnimatedModelMD3 * parent, const string& tagname);

	/** Returns a pointer to the MD3 mesh. */
	const AnimatedMeshMD3 * getMD3Mesh() const;

	/** Override of ISpaceNode::preRender(): update the interpolated quaternion tags. */
	virtual void preRender(f64 time);

	virtual void setMaterialProperty(EMATERIAL_PROPERTY prop, bool isset);

private:
	AnimatedModelMD3 * mMD3Parent;
	s32                mTagIndex;
	MD3QuaternionTag * mInterpolatedTags;

	/** We must do a little more than just apply the local transforms: must check if the model
	 has an MD3 parent, and apply the correct transforms if so. */
	virtual void updateTransforms();
};

}

#endif // ANIMATEDMODELMD3_H_INCLUDED
