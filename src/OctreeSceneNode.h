/**
 * FILE:    OctreeSceneNode.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: A Node in space that is based on a static Mesh and partitioned
 *          into an Octree.
**/

#ifndef OCTREESCENENODE_H_INCLUDED
#define OCTREESCENENODE_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "IModel.h"
#include "Octree.h"

namespace fire_engine
{

class IMesh;

/** A Node in space that is based on a static Mesh and partitioned into an 
 Octree. *.
class OctreeSceneNode : public IModel
{
public:
	/** Constuct an OctreeSceneNode with a parent and a Mesh. This will create
	 the Octree to partition the Mesh. */
	OctreeSceneNode(INode * parentNode, IMesh * mesh);

	/** Destructor. */
	virtual ~OctreeSceneNode();

	//! Inherited from IRenderable
	virtual void preRender(f64 time);
	virtual s32 render(IRenderer * rd);

protected:
	IMesh *      mOriginalMesh;
	Octree<f32>* mTree;
};

}

#endif // OCTREESCENENODE_H_INCLUDED

