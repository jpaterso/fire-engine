/**
 * FILE:    IAnimatedMesh.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IAnimatedMesh.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for an Animated Mesh, like for example MD2 models
**/

#ifndef IANIMATEDMESH_H_INCLUDED
#define IANIMATEDMESH_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "vector3.h"
#include "Array.h"
#include "IMesh.h"

namespace fire_engine
{

enum EANIMATED_MESH_TYPE
{
	EAMT_MD2,
	EAMT_MD3
};

class string;
class ITexture;

/** A Mesh that has multiple frames, and possibly multiple textures. */
class _FIRE_ENGINE_API_ IAnimatedMesh : public virtual IMesh
{
public:
	/** Destructor. */
	virtual ~IAnimatedMesh(void);

	/** Returns the total number of frames in the IAnimatedMesh. For details
	 about the starting and ending indices of these frames, use getStartFrame()
	 and getEndFrame() */
	virtual s32 getFrameCount() const = 0;

	/** Returns the index of the first valid frame in the IAnimatedMesh */
	virtual s32 getStartFrame() const = 0;

	/** Returns the index of the last frame in the IAnimatedMesh */
	virtual s32 getEndFrame() const = 0;

	/** Get the type of IAnimatedMesh actually contained. This is useful,
	 because some functions, such as setFrameLoop(), are specific to the type
	 of IAnimatedMesh (for example, setFrameLoop(start, end, loop) is not
	 recommended for AnimatedMeshMD2, instead, use setFrameLoop(type, loop). */
	virtual EANIMATED_MESH_TYPE getAnimatedMeshType() const = 0;

	/** Get the IMesh for a specific frame. Specify the start and end frames if
	 interpolation is needed, to make sure that frame is still within bounds. */
	virtual IMesh * getMesh(s32 frame, s32 start = -1, s32 end = -1) = 0;

	/** Get the IMesh for the interpolation between two frames.
	 \param first  The first frame.
	 \param second The second frame.
	 \param ipol   The time between the two frames. Must be between 0.0f and
	               1.0f */
	virtual IMesh * getMesh(s32 first, s32 second, f32 ipol) = 0;

	/** Returns the bounding box for a given frame interpolation. */
	virtual AABoundingBoxf getBoundingBox(s32 first, s32 second, f32 ipol) const = 0;

	/** Get the frames per second that should be rendered for a given frame
	 interval.*/
	virtual s32 getFPS(s32 frameStart, s32 frameEnd) const = 0;
};

}

#endif // IANIMATEDMESH_H_INCLUDED
