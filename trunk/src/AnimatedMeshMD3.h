/**
 * FILE:    AnimatedMeshMD3.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD3.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A container for meshes loaded from Quake III .MD3 files.
**/

#ifndef ANIMATEDMESHMD3_H_INCLUDED
#define ANIMATEDMESHMD3_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "IAnimatedMesh.h"
#include "IMeshBuffer.h"
#include "aabbox.h"
#include "Vertex3.h"
#include "array.h"
#include "matrix4.h"
#include "quaternion.h"
#include "Material.h"
#include "list.h"

namespace fire_engine
{

// Forward declarations
class ITexture;
class IRenderer;

struct _FIRE_ENGINE_API_ MD3QuaternionTag
{
	string      Name;
	quaternionf Rotation;
	vector3f    Position;

	MD3QuaternionTag interpolate(const MD3QuaternionTag& tag, f32 time) const
	{
		MD3QuaternionTag newtag;
		newtag.Name = "";
		newtag.Rotation = Rotation.slerp(tag.Rotation, time);
		newtag.Position = Position*(1-time)+tag.Position*time;
		return newtag;
	}
};

/** A class representing a part of an MD3 model. It stores all the key frames
 associated with that certain part, and has a method for interpolating between
 two key frames, to provide smooth animation. */
class _FIRE_ENGINE_API_ MeshBufferMD3 : public IMeshBuffer
{
public:
	/** Constructor. */
	MeshBufferMD3(Vertex3 * vertices, array<u32> * indices, ITexture * texture,
		s32 verts_per_frame, s32 num_frames);

	/** Destructor. */
	virtual ~MeshBufferMD3();

	virtual EPOLYGON_TYPE getPolygonType() const;

	virtual inline Vertex3 * _getOriginalVertices();

	virtual inline s32 _getOriginalVertexCount();

	virtual const Vertex3 * getVertices() const;

	virtual s32 getVertexCount() const;

	virtual const array<u32> * getIndices() const;

	virtual const ITexture * getTexture() const;

	virtual void setTexture(ITexture * texture);

	virtual Material getMaterial() const;

	/** Returns the BoundingBox for the current interpolation. */
	virtual const aabboxf& getBoundingBox() const;

private:
	Vertex3 *         mVertices;
	array<u32> *      mIndices;
	ITexture *        mTexture;
	Material          mMaterial;
	aabboxf *  mBoundingBoxes;
	s32               mVerticesPerFrame;
	s32               mNumFrames;
	Vertex3 *         mInterpolationBuffer;
	aabboxf    mInterpolationBoundingBox;

	/** we don't want other classes than AnimatedMeshMD3 calling
	 updateInterpolationBuffer(), so make it a friend, and the method
	 private. */
	friend class AnimatedMeshMD3;

	/** Updates the interpolation buffer based on two frames, and an interpolation value
	 between 0.0 and 1.0. */
	void updateInterpolationBuffer(s32 first, s32 second, f32 time);

	/** Calculates all the bounding boxes. */
	void calculateBoundingBoxes();
};

/** A class representing an AnimatedMesh, loaded from a Quake III .MD3 file.
 These differ from .MD2 models, as they can contain multiple sub meshes, that
 can be animated independently. For example, some body parts can move
 independently of others. */
class AnimatedMeshMD3 : public IAnimatedMesh
{
public:
	/** Constructor. */
	AnimatedMeshMD3(MeshBufferMD3 ** buffers, s32 mbuffer_count, s32 frame_count,
		array<MD3QuaternionTag*> * tags);

	/** Destructor. */
	virtual ~AnimatedMeshMD3();

	/** Methods inherited from IAnimatedMesh. */
	virtual inline s32 getFrameCount() const;
	virtual inline s32 getStartFrame() const;
	virtual inline s32 getEndFrame() const;
	virtual inline EANIMATED_MESH_TYPE getAnimatedMeshType() const;
	virtual IMesh * getMesh(s32 frame, s32 start = -1, s32 end = -1);
	virtual IMesh * getMesh(s32 first, s32 second, f32 ipol);
	virtual s32 getFPS(s32 frameStart, s32 frameEnd) const;

	/** Methods inherited from IMesh. */
	virtual inline IMeshBuffer * getMeshBuffer(s32 nr);
	virtual inline s32 getMeshBufferCount() const;

	/** Returns the BoundingBox for the current frame interpolation. */
	virtual const aabboxf& getBoundingBox() const;

	virtual aabboxf getBoundingBox(s32 first, s32 second, f32 ipol) const;

	/** Returns the number of tags. */
	s32 getNumTags() const;

	/** Returns the index of the tag in the tag Array, or -1 if it doesn't exist. */
	s32 getTagIndex(const string& tagname) const;

	/** Returns the interpolated quaternion transform for a given tag, between two given frames.
	 \param tagIndex The Index of the tag, as returned by getTagIndex().
	 \param cur      The current frame.
	 \param next     The next frame.
	 \param time     The current time, must be between 0.0 and 1.0. */
	MD3QuaternionTag getTagTransform(s32 tagIndex, s32 cur, s32 next, f32 time) const;

private:
	MeshBufferMD3 **           mBuffers;
	s32                        mBufferCount;
	s32                        mFrameCount;
	array<MD3QuaternionTag*> * mTags;

	aabboxf  mInterpolationBoundingBox;
};

inline Vertex3 * MeshBufferMD3::_getOriginalVertices()
{
	return mVertices;
}

inline s32 MeshBufferMD3::_getOriginalVertexCount()
{
	return mVerticesPerFrame*mNumFrames;
}

inline s32 AnimatedMeshMD3::getFrameCount() const
{
	return mFrameCount;
}

inline s32 AnimatedMeshMD3::getStartFrame() const
{
	return 0;
}

inline s32 AnimatedMeshMD3::getEndFrame() const
{
	return mFrameCount-1;
}

inline EANIMATED_MESH_TYPE AnimatedMeshMD3::getAnimatedMeshType() const
{
	return EAMT_MD3;
}

inline IMeshBuffer * AnimatedMeshMD3::getMeshBuffer(s32 nr)
{
	return mBuffers[nr];
}

inline s32 AnimatedMeshMD3::getMeshBufferCount() const
{
	return mBufferCount;
}

} // namespace fire_engine

#endif // ANIMATEDMESHMD3_H_INCLUDED
