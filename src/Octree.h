#ifndef OCTREE_H_INCLUDED
#define OCTREE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include "AABoundingBox.h"
#include "IMesh.h"
#include "IMeshBuffer.h"

namespace fire_engine
{

template <class T>
class _FIRE_ENGINE_API_ Octree : public virtual Object
{
public:
	Octree(IMesh * mesh) : mMesh(mesh)
	{
		if (mMesh != nullptr)
		{
			mMesh->grab();
		}
	}

	void setMesh(IMesh * mesh)
	{
		if (mMesh != nullptr)
		{
			mMesh->drop();
		}
		mMesh = mesh;
		if (mMesh != nullptr)
		{
			mMesh->grab();
		}
	}

private:

    OctreeNode * mTreeRoot = nullptr;
	IMesh * mMesh = nullptr;


	void buildTree()
	{
		for (int i = 0; i < mMesh->getMeshBufferCount; i++)
		{
			IMeshBuffer * mb = mMesh->getMeshBuffer(i);
			const Vertex3 * verts = mb->getVertices();


		}
	}


    class _FIRE_ENGINE_API_ OctreeNode
    {
    public:
        OctreeNode(Vertex3<T> * vertices, int numVertices,
                   int * indices, EPOLYGON_TYPE primitiveType,
                   int numPrimitives, int maxPrimitives)
        {
            for (int i = 0; i < numVertices; i++)
            {
                mBox.addInternalPoint(vertices[i].getPosition);
            }

            if (numPrimitives > maxPrimitives)
            {
                mChildren = new OctreeNode<T>[8];
                vector3<T> * boxCorners = new vector3<T>[8];
                mBox.getCorners(boxCorners);
            }
        }

        virtual ~OctreeNode()
        {
            if (mVertices != nullptr)
            {
                delete [] mVertices;
            }

            if (mChildren != nullptr)
            {
                delete [] mChildren;
            }
        }

        inline bool isLeaf() const
        {
            return mChildren == nullptr;
        }

    private:
        OctreeNode* mChildren = nullptr;
        Vertex<T> * mVertices = nullptr;
        AABoundingBox<T> mBox;

    }
}

}

#endif // OCTREE_H_INCLUDED
