/**
 * FILE:    OctreeSceneNode.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: Implementation of the OctreeSceneNode class.
**/

#include "OctreeSceneNode.h"
#include "IRenderer.h"
#include "Camera.h"
#include "SceneManager.h"


namespace fire_engine
{

OctreeSceneNode::OctreeSceneNode(INode * parent, IMesh * mesh)
	: IModel(parent), mOriginalMesh(mesh)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::OctreeSceneNode");
#endif

	if (mOriginalMesh != nullptr)
	{
		mOriginalMesh->grab();
	}

	mTree = new Octree<f32>(mesh, 256);
}

OctreeSceneNode::~OctreeSceneNode()
{
	if (mOriginalMesh != nullptr)
	{
		mOriginalMesh->drop();
	}

	if (mTree != nullptr)
	{
		mTree->drop();
	}
}

void OctreeSceneNode::preRender(f64 time)
{
	ISpaceNode::preRender(time);
}

s32 OctreeSceneNode::render(IRenderer * rd)
{
	const Camera * activeCamera = SceneManager::Get()->getActiveCamera();
	Array<Octree<f32>::MeshBufferChunk> visibleNodes;
	mTree->getVisibleNodes(*activeCamera, visibleNodes);
	for (s32 i = 0; i < visibleNodes.size(); i++)
	{
		/*const IMeshBuffer * mb = visibleNodes.at(i);
		rd->drawMeshBuffer(mb);*/
	}
	return 0x00;
}

}