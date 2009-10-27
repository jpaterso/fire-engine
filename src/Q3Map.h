/**
 * RCS ID:  $Id$
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
*/
#ifndef Q3MAP_H_INCLUDED
#define Q3MAP_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "vector2.h"
#include "Vertex3.h"
#include "IMesh.h"
#include "AABoundingBox.h"
#include "quake3.h"
#include "Array.h"
#include "plane3.h"
#include "IMeshBuffer.h"

namespace fire_engine
{

/** <p><code>Q3Vertex3</code> extends <code>Vertex3</code> to store
 additional lightmap coordinates.</p>*/
class _FIRE_ENGINE_API_ Q3Vertex3 : public Vertex3
{
public:
	Q3Vertex3()
	{
	}

    /** <p>Construct a <code>Q3Vertex3</code> from it's components.</p>
     \param position        The position of the vertex in space.
     \param normal          The normal to the vertex.
     \param color           The color of the vertex.
     \param tex_coords      The coordinates of the texture associated with the vertex.
     \param lightmap_coords The coordinates of the lightmap associated with the vertex. */
    Q3Vertex3(const vector3f& position, const vector3f& normal,
              const Color8& color, const vector2f& tex_coords,
              const vector2f& lightmap_coords)
              : Vertex3(position, normal, color, tex_coords),
                m_lightmap_coordinates(lightmap_coords)
    {
    }

	Q3Vertex3(const f32 * position,const f32 * normal,
	         const u8 * color, const f32 * tex_coords,
	         const f32 * lightmap_coords)
	{
	}

private:
    vector2f m_lightmap_coordinates;
};

class _FIRE_ENGINE_API_ Q3MeshBuffer : public IMeshBuffer
{
public:
	/** The type of data that the mesh buffer contains. */
	enum EMESH_BUFFER_TYPE
	{
		EMBT_POLYGON = 0x01,
		EMBT_PATCH,
		EMBT_MESH,
		EMBT_BILLBOARD
	};

	virtual EPOLYGON_TYPE getPolygonType() const
	{
		return EPT_TRIANGLES;
	}
private:
	Q3Vertex3 * mVertices;
	Array<u32> * mIndices;
	AABoundingBoxf mBoundingBox;
	ITexture * mTexture;
	ITexture * mLightmap;
};

/** <p>A holder class for maps loaded from Quake III .bsp files.</p> */
class _FIRE_ENGINE_API_ Q3Map : public IMesh
{
public:
	Q3Map(const string& name, Array<Q3Vertex3> * vertices, Array<q3::bsp_face_t> * faces,
		Array<q3::bsp_model_t> * models, q3::bsp_mesh_vertex_t * mesh_vertices,
		plane3f * planes, Array<q3::bsp_node_t> * nodes, q3::bsp_leaf_t * leafs);

	virtual ~Q3Map();

	//! methods inherited from IMesh
	virtual IMeshBuffer * getMeshBuffer(s32 offset);
	virtual s32 getMeshBufferCount() const;
	virtual const AABoundingBoxf& getBoundingBox() const;

private:
	Array<Q3Vertex3> * mVertices;
	Array<q3::bsp_face_t> * mFaces;
	Array<q3::bsp_model_t> * mModels;
	q3::bsp_mesh_vertex_t * mMeshVertices;
	plane3f * mPlanes;
	Array<q3::bsp_node_t> * mNodes;
	q3::bsp_leaf_t * mLeafs;
	AABoundingBoxf mBoundingBox;
};

}

#endif // Q3MAP_H_INCLUDED
