/**
 * FILE:    IMeshBuffer.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IMeshBuffer.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for a MeshBuffer, a mesh with a single polygon type
 *          (for example, triangles), and one texture.
**/

#ifndef IMESHBUFFER_H_INCLUDED
#define IMESHBUFFER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Array.h"
#include "Object.h"
#include "IRenderable.h"
#include "AABoundingBox.h"

namespace fire_engine
{

class IRenderer;
class Material;

/** The different types of polygons that a IMeshBuffer can hold */
enum EPOLYGON_TYPE
{
	/** Simple list of points in space. Each vertex is new point. */
	EPT_POINTS,
	/** 3-dimensional lines in space. Each new vertex is a new point on the
	 line */
	EPT_LINES,
	/** The polygons contained are triangles. Each set of 3 indices
	 forms a new triangle */
	EPT_TRIANGLES,
	/** The polygons contain indices forming quads. Each set of 4 indices
	 forms a new quad */
	EPT_QUADS,
	/** The polygons contain indices forming triangle strips. After the
	 first three indices, each new index forms a new triangle, containing
	 it's vertex along with the last two */
	EPT_TRIANGLE_STRIP,
	/** The polygon contain indices forming triangle fans. After the first
	 index, each new set of two indices forms a triangle containing their
	 respective two vertices, along with the first one */
	EPT_TRIANGLE_FAN
};

// Forward declarations
class ITexture;
class Vertex3;

/** A class containing information about some mesh, with a single texture.
 The vertex data contained can represent various polygonal types */
class _FIRE_ENGINE_API_ IMeshBuffer : public virtual Object, public virtual IRenderable
{
public:
	/** Virtual destructor */
	virtual ~IMeshBuffer() {};

	/** Get the type of Polygon that this MeshBuffer contains. The Polygon Type
	 can be used when rendering the vertices */
	virtual EPOLYGON_TYPE getPolygonType() const = 0;

	/** Returns a pointer to the original vertices. This should be used only if
	 the vertices are to be modified, for example like a MeshModifier would do. */
	virtual Vertex3 * _getOriginalVertices() = 0;

	/** Returns the total number of vertices that would be returned by a call to
	 _getOriginalVertices(). */
	virtual s32 _getOriginalVertexCount() = 0;

	/** Get the vertices contained in the Mesh Buffer. To render them properly,
	 use the indices obtained via getIndices() */
	virtual const Vertex3 * getVertices() const = 0;

	/** Get the number of vertices contained within the IMeshBuffer. This will
	 be a number per frame! If the Model contains more than one frame, then the
	 total number will be the number of vertices times the number of frames.*/
	virtual s32 getVertexCount() const = 0;

	/** Get the indices, which can be used to access the vertices, in order
	 to display proper polygons, as described by the getPolygonType() method */
	virtual const Array<u32> * getIndices() const = 0;

	/** Get the single Texture used by the IMeshBuffer. The proper coordinates
	 to the texture can be accessed via the getTextureCoordinates() method */
	//TODO: Could extend this to create a Material class, that contains a single texture
	virtual const ITexture * getTexture() const = 0;

	/** Set the ITexture to be used by this IMeshBuffer */
	virtual void setTexture(ITexture * texture) = 0;

	/** Returns the Material that the IMeshBuffer was created with. */
	virtual Material getMaterial() const = 0;

	/** Inherited from IRenderable. Render the IMeshBuffer, worrying just about
	 the vertex data, no Materials or anything. */
	virtual s32 render(IRenderer * rd) = 0;

	/** Returns a bounding box, that the IMeshBuffer is contained in. */
	virtual const AABoundingBoxf& getBoundingBox() const = 0;
};

}

#endif // IMESHBUFFER_H_INCLUDED
