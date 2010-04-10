
#include "Types.h"
#include "CompileConfig.h"
#include "IMeshBuffer.h"
#include "aabbox.h"

namespace fire_engine
{

/** Default implementation of an IMeshBuffer. */
class _FIRE_ENGINE_API_ CMeshBuffer : public virtual IMeshBuffer
{
public:
	CMeshBuffer(Vertex3 * vertices, s32 vertexCount, array<u32> * indices, EPOLYGON_TYPE polygonType, Material mat)
		: Vertices(vertices), Indices(indices), PolygonType(polygonType), VertexCount(vertexCount), Material(mat)
	{
	}

	virtual ~CMeshBuffer()
	{
		if (Indices != nullptr)
		{
			delete Indices;
		}
	}

	virtual EPOLYGON_TYPE getPolygonType()
	{
		return PolygonType;
	}

	virtual Vertex3 * _getOriginalVertices()
	{
		return Vertices;
	}

	virtual s32 _getOriginalVertexCount()
	{
		return VertexCount;
	}

	virtual const Vertex3 * getVertices() const
	{
		return Vertices;
	}

	virtual s32 getVertexCount() const
	{
		return VertexCount;
	}

	virtual const array<u32> * getIndices() const
	{
		return Indices;
	}

	virtual const ITexture * getTexture() const
	{
		return Texture;
	}

	virtual void setTexture(ITexture * texture)
	{
		if (Texture != nullptr)
		{
			Texture->drop();
		}
		Texture = texture;
		if (Texture != nullptr)
		{
			Texture->grab();
		}
	}

	virtual Material getMaterial() const
	{
		return Mat;
	}

	virtual const aabboxf& getBoundingBox() const
	{
		return BoundingBox;
	}
private:
	Vertex3 *      Vertices;
	s32            VertexCount;
	array<u32> *   Indices;
	EPOLYGON_TYPE  PolygonType;
	ITexture *     Texture;
	Material       Mat;
	aabboxf BoundingBox;
};



}