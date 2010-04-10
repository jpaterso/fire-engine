/**
 * RCS ID:  $Id$
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
*/

#include "Q3Map.h"
#include "IMeshBuffer.h"

namespace fire_engine
{

Q3Map::Q3Map(const string& name, array<Q3Vertex3> * vertices, array<q3::bsp_face_t> * faces,
             array<q3::bsp_model_t> * models, q3::bsp_mesh_vertex_t * mesh_vertices,
			 plane3f * planes, array<q3::bsp_node_t> * nodes, q3::bsp_leaf_t * leafs)
	: mVertices(vertices), mFaces(faces), mModels(models), mMeshVertices(mesh_vertices),
	  mPlanes(planes), mNodes(nodes), mLeafs(leafs)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::Q3Map");
#endif
	mMeshName = name;
	for (s32 i = 0; i < mVertices->size(); i++)
	{
		mBoundingBox.addInternalPoint(mVertices->at(i).getPosition());
	}
}

Q3Map::~Q3Map()
{
	delete mVertices;
	delete mFaces;
	delete mModels;
	delete [] mMeshVertices;
	delete [] mPlanes;
	delete mNodes;
	delete [] mLeafs;
}

IMeshBuffer * Q3Map::getMeshBuffer(s32 offset)
{
	if (offset < 0 || offset >= mModels->size())
	{
		return nullptr;
	}
	return nullptr;
}

s32 Q3Map::getMeshBufferCount() const
{
	return mModels->size();
}

const aabboxf& Q3Map::getBoundingBox() const
{
	return mBoundingBox;
}
}
