/**
 * FILE:    AnimatedMeshMD3.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD3.cpp 117 2007-10-06 00:31:15Z jpaterso $
 * PURPOSE: Implementation of the AnimatedMeshMD3 class.
**/

#include "AnimatedMeshMD3.h"
#include "ITexture.h"
#include "IRenderer.h"

namespace fire_engine
{

MeshBufferMD3::MeshBufferMD3(Vertex3 * vertices, array<u32> * indices, ITexture * texture,
	s32 verts_per_frame, s32 num_frames)
	: mVertices(vertices), mIndices(indices), mTexture(texture),
	  mVerticesPerFrame(verts_per_frame), mNumFrames(num_frames)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::MeshBufferMD3");
#endif
	if (mTexture)
		mTexture->grab();
	mBoundingBoxes = new aabboxf[mNumFrames];
	calculateBoundingBoxes();
	mInterpolationBuffer = new Vertex3[verts_per_frame];
}

MeshBufferMD3::~MeshBufferMD3()
{
	if (mVertices)
		delete [] mVertices;
	if (mIndices)
		delete mIndices;
	if (mTexture)
		mTexture->drop();
	delete [] mInterpolationBuffer;
	delete [] mBoundingBoxes;
}

EPOLYGON_TYPE MeshBufferMD3::getPolygonType() const
{
	return EPT_TRIANGLES;
}

const Vertex3 * MeshBufferMD3::getVertices() const
{
	return mInterpolationBuffer;
}

s32 MeshBufferMD3::getVertexCount() const
{
	return mVerticesPerFrame;
}

const array<u32> * MeshBufferMD3::getIndices() const
{
	return mIndices;
}

const ITexture * MeshBufferMD3::getTexture() const
{
	return mTexture;
}

void MeshBufferMD3::setTexture(ITexture * texture)
{
	mTexture = texture;
}

Material MeshBufferMD3::getMaterial() const
{
	return mMaterial;
}

const aabboxf& MeshBufferMD3::getBoundingBox() const
{
	return mInterpolationBoundingBox;
}

void MeshBufferMD3::updateInterpolationBuffer(s32 first, s32 second, f32 time)
{
	const Vertex3 * fVerts = &mVertices[first*mVerticesPerFrame];
	const Vertex3 * sVerts = &mVertices[second*mVerticesPerFrame];
	f32 invtime = 1.0f-time;
	for (s32 i = 0; i < mVerticesPerFrame; i++)
	{
		mInterpolationBuffer[i].setPosition(fVerts[i].getPosition()*invtime +
			sVerts[i].getPosition()*time);
		mInterpolationBuffer[i].setNormal(fVerts[i].getNormal()*invtime +
			sVerts[i].getNormal()*time);
		mInterpolationBuffer[i].setColor(fVerts[i].getColor()*invtime +
			sVerts[i].getColor()*time);
		mInterpolationBuffer[i].setTextureCoordinates(fVerts[i].getTextureCoordinates()*invtime +
			sVerts[i].getTextureCoordinates()*time);
	}
	mInterpolationBoundingBox = mBoundingBoxes[first].getInterpolate(mBoundingBoxes[second], time);
}

void MeshBufferMD3::calculateBoundingBoxes()
{
	for (s32 i = 0; i < mNumFrames; i++)
		for (s32 j = 0; j < mVerticesPerFrame; j++)
			mBoundingBoxes[i].addInternalPoint(mVertices[i*mVerticesPerFrame+j].getPosition());
}

AnimatedMeshMD3::AnimatedMeshMD3(MeshBufferMD3 ** buffers, s32 mbuffer_count, s32 frame_count,
	array<MD3QuaternionTag*> * tags)
	: mBuffers(buffers), mBufferCount(mbuffer_count), mFrameCount(frame_count),
	  mTags(tags)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::AnimatedMeshMD3");
#endif
	for (s32 i = 0; i < mBufferCount; i++)
		mBuffers[i]->grab();
}

AnimatedMeshMD3::~AnimatedMeshMD3()
{
	for (s32 i = 0; i < mBufferCount; i++)
		mBuffers[i]->drop();
	delete [] mBuffers;
	for (s32 i = 0; i < mTags->size(); i++)
		delete [] mTags->at(i);
	delete mTags;
}

IMesh * AnimatedMeshMD3::getMesh(s32 frame, s32 start, s32 end)
{
	mInterpolationBoundingBox.reset();
	for (s32 i = 0; i < mBufferCount; i++)
	{
		mBuffers[i]->updateInterpolationBuffer(frame, frame, 0.0f);
		mInterpolationBoundingBox.addInternalBoundingBox(mBuffers[i]->getBoundingBox());
	}
	return this;
}

IMesh * AnimatedMeshMD3::getMesh(s32 first, s32 second, f32 ipol)
{
	mInterpolationBoundingBox.reset();
	for (s32 i = 0; i < mBufferCount; i++)
	{
		mBuffers[i]->updateInterpolationBuffer(first, second, ipol);
		mInterpolationBoundingBox.addInternalBoundingBox(mBuffers[i]->getBoundingBox());
	}
	return this;
}

s32 AnimatedMeshMD3::getFPS(s32 frameStart, s32 frameEnd) const
{
	//TODO: Fix this constant crap
	return 10;
}

const aabboxf& AnimatedMeshMD3::getBoundingBox() const
{
	return mInterpolationBoundingBox;
}

aabboxf AnimatedMeshMD3::getBoundingBox(s32 first, s32 second, f32 ipol) const
{
	aabboxf box;
	for (s32 i = 0; i < mBufferCount; i++)
		box.addInternalBoundingBox(mBuffers[i]->mBoundingBoxes[first].getInterpolate(mBuffers[i]->mBoundingBoxes[second], ipol));
	return box;
}

s32 AnimatedMeshMD3::getNumTags() const
{
	return mTags->size();
}

s32 AnimatedMeshMD3::getTagIndex(const string& tagname) const
{
	for (s32 i = 0; i < mTags->size(); i++)
		if (mTags->at(i)[0].Name == tagname)
			return i;
	return -1;
}

MD3QuaternionTag AnimatedMeshMD3::getTagTransform(s32 tagIndex, s32 cur, s32 next, f32 time) const
{
	return mTags->at(tagIndex)[cur].interpolate(mTags->at(tagIndex)[next], time);
}


} // namespace fire_engine
