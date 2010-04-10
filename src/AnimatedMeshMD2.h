/**
 * FILE:    AnimatedMeshMD2.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD2.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An animated mesh loaded from an .MD2 file
**/

#ifndef ANIMATEDMESHMD2_H_INCLUDED
#define ANIMATEDMESHMD2_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "IAnimatedMesh.h"
#include "IMeshBuffer.h"
#include "vector3.h"
#include "vector2.h"
#include "array.h"
#include "Object.h"
#include "Vertex3.h"
#include "AnimatedModel.h"
#include "Material.h"
#include "aabbox.h"

namespace fire_engine
{

/** The standard types of animation that Quake II MD2 models have. These
 symbolic constants should be used when setting the frame loop, instead
 of using a start and end frame index */
enum EMD2_ANIMATION_TYPE
{
	EMAT_STAND = 0x00,
	EMAT_RUN,
	EMAT_ATTACK,
    EMAT_PAIN_A,
    EMAT_PAIN_B,
    EMAT_PAIN_C,
    EMAT_JUMP,
    EMAT_FLIP,
    EMAT_SALUTE,
    EMAT_FALLBACK,
    EMAT_WAVE,
    EMAT_POINT,
    EMAT_CROUCH_STAND,
    EMAT_CROUCH_WALK,
    EMAT_CROUCH_ATTACK,
    EMAT_CROUCH_PAIN,
    EMAT_CROUCH_DEATH,
    EMAT_DEATH_FALLBACK,
    EMAT_DEATH_FALLFORWARD,
    EMAT_DEATH_FALLBACKSLOW,
    EMAT_BOOM,
	EMAT_MAX_ANIMATIONS // Do not use this one
};

class ITexture;

/** An AnimatedMesh loaded from a Quake II MD2 file. */
class _FIRE_ENGINE_API_ AnimatedMeshMD2 : public virtual IAnimatedMesh, public virtual IMeshBuffer
{
public:
	//! Default ctor
	AnimatedMeshMD2();

	/** Construct an AnimatedMeshMD2
	 \param name       A name to associate with it.
	 \param num_frames The total number of frames.
	 \param vertices_per_frame The number of vertices in each frame
	 \param vertices   The vertices.
	 \param indices    The indices to the vertices.
	 \param texture    The texture to use for the model */
	AnimatedMeshMD2(const string& name, s32 num_frames, s32 vertices_per_frame,
		Vertex3 * vertices, array<u32> * indices, ITexture * texture);

	//! Dtor
	virtual ~AnimatedMeshMD2();

	/** Virtual functions inherited from IMesh */
	virtual inline IMeshBuffer * getMeshBuffer(s32 some_argument);
	virtual inline s32 getMeshBufferCount() const;

	/** Renders the interpolation buffer. */
	virtual IMesh * getMesh(s32 frame, s32 start = -1, s32 end = -1);
	virtual IMesh * getMesh(s32 first, s32 second, f32 ipol);

	/** Virtual functions inherited from IAnimatedMesh */
	virtual inline s32 getFrameCount() const;
	virtual inline s32 getStartFrame() const;
	virtual inline s32 getEndFrame() const;
	virtual inline EANIMATED_MESH_TYPE getAnimatedMeshType() const;

	/** Virtual functions inherited from IMeshBuffer */
	virtual EPOLYGON_TYPE getPolygonType() const;
	virtual inline Vertex3 * _getOriginalVertices();
	virtual inline s32 _getOriginalVertexCount();
	virtual inline const Vertex3 * getVertices() const;
	virtual inline s32 getVertexCount() const;
	virtual inline const array<u32> * getIndices() const;
	virtual inline const ITexture * getTexture() const;

	/** Set the frame loop for an MD2 model. Use this instead of the other
	 setFrameLoop() function, as the frame start and ends are pre-defined
	 for these models. */
	void setFrameLoop(EMD2_ANIMATION_TYPE animation_type, bool loop = true); // done

	/** Set the texture to use. */
	virtual void setTexture(ITexture * texture);

	/** Get the frames per second that should be rendered for a given frame
	 interval. In this case, since the fps changes for the different types
	 of Quake II MD2 animations, if the frame intervals go over multiple
	 different QII animations, an average fps number is calculated. */
	virtual s32 getFPS(s32 frameStart, s32 frameEnd) const;

	/** Returns the material used by this AnimatedMeshMD2. */
	virtual inline Material getMaterial() const;

	virtual const aabboxf& getBoundingBox() const;

	virtual aabboxf getBoundingBox(s32 first, s32 second, f32 ipol) const;

private:
	string                 m_mesh_name;
	s32                    mNumFrames;
	s32                    mNumVerticesPerFrame;
	Vertex3 *              mVertices;
	Vertex3 *              mInterpolationBuffer;
	array<u32> *           mIndices;
	ITexture *             mTexture;
	Material               mMaterial;
	aabboxf *       mBoundingBoxes;
	aabboxf         mCurrentBoundingBox;

	/** Update the interpolation buffer, based on the interpolation between two
	 frames. */
	void animate(s32 first, s32 second, f32 ipol);
};

inline IMeshBuffer * AnimatedMeshMD2::getMeshBuffer(s32 some_argument)
{
	return dynamic_cast<IMeshBuffer *>(this);
}

inline s32 AnimatedMeshMD2::getMeshBufferCount() const
{
	return 1;
}

inline s32 AnimatedMeshMD2::getFrameCount() const
{
	return mNumFrames;
}

inline s32 AnimatedMeshMD2::getStartFrame() const
{
	return 0;
}

inline s32 AnimatedMeshMD2::getEndFrame() const
{
	return mNumFrames-1;
}

inline EANIMATED_MESH_TYPE AnimatedMeshMD2::getAnimatedMeshType() const
{
	return EAMT_MD2;
}

inline Vertex3 * AnimatedMeshMD2::_getOriginalVertices()
{
	return mVertices;
}

inline s32 AnimatedMeshMD2::_getOriginalVertexCount()
{
	return mNumFrames*mNumVerticesPerFrame;
}

inline const Vertex3 * AnimatedMeshMD2::getVertices() const
{
	return mInterpolationBuffer;
}

inline s32 AnimatedMeshMD2::getVertexCount() const
{
	return mNumVerticesPerFrame;
}

inline EPOLYGON_TYPE AnimatedMeshMD2::getPolygonType() const
{
	return EPT_TRIANGLES;
}

inline const array<u32> * AnimatedMeshMD2::getIndices() const
{
	return mIndices;
}

inline const ITexture * AnimatedMeshMD2::getTexture() const
{
	return mTexture;
}

inline Material AnimatedMeshMD2::getMaterial() const
{
	return mMaterial;
}

}

#endif // ANIMATEDMESHMD2_H_INCLUDED
