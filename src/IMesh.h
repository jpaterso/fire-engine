/**
 * FILE:    IMesh.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IMesh.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for a mesh: something that can be rendered,
 *          that can have multiple textures, etc...
**/

#ifndef IMESH_H_INCLUDED
#define IMESH_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ISpaceNode.h"
#include "Object.h"
#include "aabbox.h"

namespace fire_engine
{

class IMeshBuffer;
class String;

/** An interface for a mesh: a collection of mesh buffers, each with their own 
 set of vertices, polygons as well as a single material. */
class _FIRE_ENGINE_API_ IMesh : public virtual Object
{
public:
	/** Destructor */
	virtual ~IMesh() {};

	/** Returns a certain IMeshBuffer
	 \param some_argument This will be used later */
	virtual IMeshBuffer * getMeshBuffer(s32 some_argument) = 0;

	/** Get the number of IMeshBuffers that this Mesh contains */
	virtual s32 getMeshBufferCount() const = 0;

	/** Get the name associated with the given IMesh */
	inline const String& getName() const;

	/** Sets the name of the IMesh. */
	inline void setName(const String& name);

	/** Returns a bounding box, encompassing the entire IMesh. */
	virtual const aabboxf& getBoundingBox() const = 0;

protected:
	String mMeshName;
};

inline const String& IMesh::getName() const
{
	return mMeshName;
}

inline void IMesh::setName(const String& name)
{
	mMeshName = name;
}

}

#endif // IMESH_H_INCLUDED
