
#include "Types.h"
#include "CompileConfig.h"
#include "IMeshBuffer.h"
#include "AABoundingBox.h"

namespace fire_engine
{

/** Default implemention of an IMeshBuffer. */
class _FIRE_ENGINE_API_ MeshBuffer : public virtual IMeshBuffer
{
public:

	MeshBuffer()
	{

	}

	virtual ~MeshBuffer()
	{

	}

	virtual inline EPOLYGON_TYPE getPolygonType() const;

	virtual inline Vertex3 * _getOriginalVertices();

	virtual inline s32 _getOriginalVertexCount();

	virtual inline const Vertex3 * getVertices() const;

	virtual inline s32 getVertexCount() const;

	virtual inline const Array<u32> * getIndices() const;

	virtual inline const ITexture * getTexture() const ;

	virtual void setTexture(ITexture * texture);

	virtual inline Material getMaterial() const;

	virtual inline const AABoundingBoxf& getBoundingBox() const;

private:
	Vertex3 *      Vertices;
	Array<u32> *   Indices;
	EPOLYGON_TYPE  PolygonType;
	ITexture *     Texture;
	s32            VertexCount;
	AABoundingBoxf BoundingBox;
};

}