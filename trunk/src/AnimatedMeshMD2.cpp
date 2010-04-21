/**
 * FILE:    AnimatedMeshMD2.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD2.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of the AnimatedMeshMD2 class
**/

#include "AnimatedMeshMD2.h"
#include "IRenderer.h"
#include "ITexture.h"
#include "Vertex3.h"
#include "Timer.h"

namespace fire_engine
{

typedef struct
{
	u16 start;
	u16 end;
	u16 fps;
} md2_animation_t;

static const md2_animation_t md2_animation_list[EMAT_MAX_ANIMATIONS] =
{
	{  0,  39,  9},   // STAND
    { 40,  45, 10},   // RUN
    { 46,  53, 10},   // ATTACK
    { 54,  57,  7},   // PAIN_A
    { 58,  61,  7},   // PAIN_B
    { 62,  65,  7},   // PAIN_C
    { 66,  71,  7},   // JUMP
    { 72,  83,  7},   // FLIP
    { 84,  94,  7},   // SALUTE
    { 95, 111, 10},   // FALLBACK
    {112, 122,  7},   // WAVE
    {123, 134,  6},   // POINT
    {135, 153, 10},   // CROUCH_STAND
    {154, 159,  7},   // CROUCH_WALK
    {160, 168, 10},   // CROUCH_ATTACK
    {169, 172,  7},   // CROUCH_PAIN
    {173, 177,  5},   // CROUCH_DEATH
    {178, 183,  7},   // DEATH_FALLBACK
    {184, 189,  7},   // DEATH_FALLFORWARD
    {190, 197,  7},   // DEATH_FALLBACKSLOW
    {198, 198,  5},   // BOOM
};

static const String md2_frame_names[EMAT_MAX_ANIMATIONS] =
{
	"stand", "run", "attack", "pain_A", "pain_B", "pain_C",
	"jump", "flip", "salute", "fallback", "wave", "point",
	"crouch_stand", "crouch_walk", "crouch_attack", "crouch_pain",
	"crouch_death", "death_fallback", "death_fallforward",
	"death_fallbackslow", "boom"
};

AnimatedMeshMD2::AnimatedMeshMD2()
	: mNumFrames(0), mNumVerticesPerFrame(0),
	  mVertices(0), mInterpolationBuffer(0), mIndices(0),
	  mBoundingBoxes(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::AnimatedMeshMD2");
#endif
	m_mesh_name = "";
}

AnimatedMeshMD2::AnimatedMeshMD2(const String& name, s32 num_frames, s32 vertices_per_frame,
	Vertex3 * vertices, Array<u32> * indices, ITexture * texture)
	: mNumFrames(num_frames), mNumVerticesPerFrame(vertices_per_frame),
	  mVertices(vertices), mInterpolationBuffer(0), mIndices(indices),
	  mBoundingBoxes(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::AnimatedMeshMD2");
#endif
	m_mesh_name = name;
	mInterpolationBuffer = new Vertex3[mNumVerticesPerFrame];

	// Calculate bounding boxes
	mBoundingBoxes = new aabboxf[num_frames];
	for (s32 i = 0; i < num_frames; i++)
	{
		const Vertex3 * frameVertices = &vertices[i*vertices_per_frame];
		for (s32 j = 0; j < vertices_per_frame; j++)
			mBoundingBoxes[i].addInternalPoint(frameVertices[j].getPosition());
	}
	Mat.setTexture(0, texture);
}

AnimatedMeshMD2::~AnimatedMeshMD2()
{
	if (mVertices)
		delete [] mVertices;
	if (mInterpolationBuffer)
		delete [] mInterpolationBuffer;
	if (mIndices)
		delete mIndices;
}

void AnimatedMeshMD2::animate(s32 first, s32 second, f32 ipol)
{
	f32 om_ipol = 1.0f-ipol;
	const Vertex3 * cvertices, * nvertices;
	cvertices = &mVertices[first*mNumVerticesPerFrame];
	nvertices = &mVertices[second*mNumVerticesPerFrame];
	for (s32 i = 0; i < mNumVerticesPerFrame; i++)
	{
		mInterpolationBuffer[i].setPosition(cvertices[i].getPosition()*om_ipol + nvertices[i].getPosition()*ipol);
		mInterpolationBuffer[i].setNormal(cvertices[i].getNormal()*om_ipol + nvertices[i].getNormal()*ipol);
		mInterpolationBuffer[i].setTextureCoordinates(cvertices[i].getTextureCoordinates());
	}
	mCurrentBoundingBox = mBoundingBoxes[first].getInterpolate(mBoundingBoxes[second], ipol);
}

IMesh * AnimatedMeshMD2::getMesh(s32 frame, s32 start, s32 end)
{
	animate(frame, frame, 0.0f);
	return this;
}

IMesh * AnimatedMeshMD2::getMesh(s32 first, s32 second, f32 ipol)
{
	animate(first, second, ipol);
	return this;
}

s32 AnimatedMeshMD2::getFPS(s32 frameStart, s32 frameEnd) const
{
	s16 interval = 1;
	s16 cumulative_fps;
	for (s32 i = 0; i < EMAT_MAX_ANIMATIONS; i++)
	{
		if (frameStart >= md2_animation_list[i].start)
		{
			cumulative_fps = md2_animation_list[i].fps;
			for (s32 j = i; j < EMAT_MAX_ANIMATIONS; j++)
			{
				if (frameEnd <= md2_animation_list[j].end)
					break;
				cumulative_fps += md2_animation_list[j].fps;
				interval++;
			}
			return cumulative_fps/interval;
		}
	}
	//TODO: write this
	return 1;
}

const aabboxf& AnimatedMeshMD2::getBoundingBox() const
{
	return mCurrentBoundingBox;
}

aabboxf AnimatedMeshMD2::getBoundingBox(s32 first, s32 second, f32 ipol) const
{
	return mBoundingBoxes[first].getInterpolate(mBoundingBoxes[second], ipol);
}

}
