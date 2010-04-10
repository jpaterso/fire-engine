/**
 * FILE:    IModel.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IModel.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An entity that has a physical presence on the scene, like
 *          a Monster...
**/

#ifndef IMODEL_H_INCLUDED
#define IMODEL_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ISpaceNode.h"
#include "aabbox.h"

namespace fire_engine
{

class Material;

//! An entity with a physical presence, like a MD2 model.
class _FIRE_ENGINE_API_ IModel : public ISpaceNode
{
public:
	/** Returns an Axis-Aligned Bounding Box that should encompass this whole
	 model. Note that this will return a bounding box aligned to the *model*
	 coordinate axis, that will most likely not be aligned to the world coordinate.
	 For the box aligned to world coordinates, call getTransformedBoundingVolume() instead. */
	virtual const aabboxf& getBoundingVolume() const
	{
		return mBoundingBox;
	}

	/** Returns a bounding box for the model that is aligned to the world coordinates. */
	virtual aabboxf getTransformedBoundingVolume() const
	{
		return mWorldTransform.applyTransformation(mBoundingBox);
	}

protected:
	aabboxf mBoundingBox;

	/** Constructor - made protected so that it can't be accessed directly. */
	IModel(INode * parent) : ISpaceNode(parent)
	{
	}
};

}

#endif
