
#include "Types.h"
#include "CompileConfig.h"
#include "IMeshBuffer.h"
#include "aabbox.h"
#include "Material.h"

namespace fire_engine
{

/** Default implementation of an IMeshBuffer. */
class _FIRE_ENGINE_API_ CMeshBuffer : public virtual IMeshBuffer
{
public:
	CMeshBuffer(Vertex3 * vertices, s32 vertexCount, u32 * indices, s32 indexCount, 
		EPOLYGON_TYPE polygonType, Material mat, bool sharedVertexData)
		: Vertices(vertices, vertexCount), 
		  Indices(indices, indexCount), 
		  PolygonType(polygonType),
		  Mat(mat), 
		  SharedVertexData(sharedVertexData)
	{
		if (SharedVertexData)
		{
			Vertices.setFreeWhenDestroyed(false);
		}
	}

	virtual ~CMeshBuffer()
	{
	}

	virtual EPOLYGON_TYPE getPolygonType()
	{
		return PolygonType;
	}

	virtual Vertex3 * _getOriginalVertices()
	{
		return Vertices.pointer();
	}

	virtual s32 _getOriginalVertexCount()
	{
		return Vertices.getCount();
	}

	virtual const Vertex3 * getVertices() const
	{
		return Vertices.const_pointer();
	}

	virtual s32 getVertexCount() const
	{
		return Vertices.getCount();
	}

	virtual const Array<u32> * getIndices() const
	{
		return &Indices;
	}

	virtual Material getMaterial() const
	{
		return Mat;
	}

	virtual const aabboxf& getBoundingBox() const
	{
		return BoundingBox;
	}

protected:
	Array<Vertex3> Vertices;
	Array<u32>     Indices;
	EPOLYGON_TYPE  PolygonType;
	Material       Mat;
	aabboxf        BoundingBox;
	bool           SharedVertexData;
};



}