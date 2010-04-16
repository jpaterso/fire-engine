#ifndef OCTREE_H_INCLUDED
#define OCTREE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include "aabbox.h"
#include "IMesh.h"
#include "CMeshBuffer.h"
#include "Vertex3.h"
#include "Array.h"

namespace fire_engine
{

template <class T>
class _FIRE_ENGINE_API_ Octree : public virtual Object
{
public:
	/** Construct an Octree from some Mesh. The mesh should not be animated.
	 \param mesh The mesh to construct the Octree from. 
	 \param maxPolyCount The maximum number of polygons per child Octant. */
	Octree(IMesh * mesh, int maxPolygonCount = 256) 
		: Mesh(mesh), MaxPolyCount(maxPolygonCount)
	{
		if (Mesh != nullptr)
		{
			Mesh->grab();
		}
	}

	virtual ~Octree()
	{
		if (Mesh != nullptr)
		{
			Mesh->drop();
		}
		if (TreeRoot != nullptr)
		{
			delete TreeRoot;
		}
	}

	/** Sets the Mesh to use when constructing the Octree.
	 \param mesh The mesh to use when constructing the Octree. */
	void setMesh(IMesh * mesh)
	{
		if (Mesh != nullptr)
		{
			Mesh->drop();
		}
		Mesh = mesh;
		if (Mesh != nullptr)
		{
			Mesh->grab();
		}
	}

private:
	void buildTree()
	{
		TreeRoot = new OctreeNode(Mesh, 0, MaxPolyCount);
	}

	/** A piece of a Mesh Buffer stored in a leaf node of the Octree. */
	class _FIRE_ENGINE_API_ OctreeMeshBufferChunk : public CMeshBuffer
	{
	public:
		OctreeMeshBufferChunk(Vertex3 * vertices, s32 vertexCount, 
			Array<u32> * indices, EPOLYGON_TYPE polygonType, Material Mat)
			: CMeshBuffer(vertices, vertexCount, indices, polygonType, mat)
		{
		}

		virtual ~OctreeMeshBufferChunk()
		{
		}
	};

	/** A node in the Octree. */
    class _FIRE_ENGINE_API_ OctreeNode
    {
    public:
        OctreeNode(IMesh * mesh, u32 depth, u32 maxPolyCount, bool shareVertices = true)
			: Depth(depth), ShareVertices(shareVertices)
        {
			if (ShareVertices && Depth = 0)
			{
				Vertices = new Vertex3*[mesh->getMeshBufferCount()];
				for (u32 i = 0; i < mesh->getMeshBufferCount(); i++)
				{
					Vertices[i] = mesh->getMeshBuffer(i)->getVertices();
				}
			}

			u32 polyCount = 0;
			for (u32 i = 0; i < mesh->getMeshBufferCount; i++)
			{
				IMeshBuffer * mb = mesh->getMeshBuffer(i);
				const Vertex3 * verts = mb->getVertices();
				for (u32 j = 0; j < mb->getVertexCount(); j++)
				{
					Box.addInternalPoint(verts[j].getPosition());
				}

				polyCount += mb->getPolygonCount();
			}

			if (polyCount > maxPolyCount)
			{
				IsLeaf = false;
				aabbox<T> childBox[8];
				calculateChildBoundingBoxes(childBox);

				for (u32 i = 0; i < mesh->getMeshBufferCount(); i++)
				{
					IMeshBuffer * mb = mesh->getMeshBuffer(i);
					const Vertex3 * meshVertices = mb->getVertices();
					const Array<u32> * meshIndices =  mb->getIndices();
					for (u32 j = 0; j < 8; j++)
					{
						IMeshBuffer * chunk = nullptr;
						Array<s32> indices;
						switch (mb->getPolygonType())
						{
						case EPT_POINTS:
							break;
						case EPT_LINES:
							break;
						case EPT_TRIANGLES:
							for (u32 k = 0; k < meshIndices->size(); k+=3)
							{
								if (childBox[j].contains(meshVertices[k].getPosition()) ||
									childBox[j].contains(meshVertices[k+1].getPosition()) ||
									childBox[j].contains(meshVertices[k+2].getPosition()))
								{
									indices.push_back(k);
									indices.push_back(k+1);
									indices.push_back(k+2);
								}
							}
							break;
						case EPT_QUADS:
							for (u32 k = 0; k < meshIndices->size(); k+=4)
							{
								if (childBox[j].contains(meshVertices[k].getPosition()) ||
									childBox[j].contains(meshVertices[k+1].getPosition()) ||
									childBox[j].contains(meshVertices[k+2].getPosition()) ||
									childBox[j].contains(meshVertices[k+3].getPosition()))
								{
									indices.push_back(k);
									indices.push_back(k+1);
									indices.push_back(k+2);
									indices.push_back(k+3);
								}
							}
							break;
						case EPT_TRIANGLE_STRIP:
							break;
						case EPT_TRIANGLE_FAN:
							break;
						}

						if (indices.size() > 0)
						{
							Children[i] = new OctreeNode(null, depth+1, maxPolyCount);
						}
						else
						{
							Children[i] = nullptr;
						}
					}
				}
			}
			else
			{
				IsLeaf = true;
			}
        }

        virtual ~OctreeNode()
        {
            if (IsLeaf)
            {
				for (u32 i = 0; i < 8; i++)
				{
					if (Children[i] != nullptr)
					{
						delete Children[i];
					}
				}
            }

			if (Vertices != nullptr)
			{
				if (ShareVertices && Depth == 0)
				{
					//delete [] Vertices;
				}
				else if (!ShareVertices)
				{
					//delete [] Vertices;
				}
			}
        }

        inline bool isLeaf() const
        {
            return IsLeaf;
        }

    private:
		u32                     Depth;
		bool                    ShareVertices;
		bool                    IsLeaf;
		OctreeMeshBufferChunk * Chunks;
        OctreeNode *            Children[8];
        const Vertex3 **        Vertices;
        aabbox<T>               Box;

		/** Populate the eight child axis-aligned bounding boxes.\
		 \param childBox A place to store the boxes. */
		void calculateChildBoundingBoxes(aabbox<T>* childBox) const
		{
			vector3<T> minp = Box.getMinPoint();
			vector3<T> maxp = Box.getMaxPoint();
			vector3<T> center = Box.getCenter();

			T minx = Box.getMinPoint().getX();
			T miny = Box.getMinPoint().getY();
			T minz = Box.getMinPoint().getZ();

			T centerx = Box.getCenter().getX();
			T centery = Box.getCenter().getY();
			T centerz = Box.getCenter().getZ();

			T maxx = Box.getMaxPoint().getX();
			T maxy = Box.getMaxPoint().getY();
			T maxz = Box.getMaxPoint().getZ();

			childBox[0] = aabbox<T>(minp, center);
			childBox[1] = aabbox<T>(vector3<T>(centerx, miny, minz), vector3<T>(maxx, centery, centerz));
			childBox[2] = aabbox<T>(vector3<T>(minx, centery, minz), vector3<T>(centerx, maxy, centerz));
			childBox[3] = aabbox<T>(vector3<T>(centerx, centery, minz), vector3<T>(maxx, maxy, centerz));
			childBox[4] = aabbox<T>(vector3<T>(minx, miny, centerz), vector3<T>(centerx, centery, maxz));
			childBox[5] = aabbox<T>(vector3<T>(centerx, miny, centerz), vector3<T>(maxx, centery, maxz));
			childBox[6] = aabbox<T>(vector3<T>(minx, centery, centerz), vector3<T>(centerx, maxy, maxz));
			childBox[7] = aabbox<T>(center, maxp);
		}

    };

	OctreeNode * TreeRoot;
	IMesh * Mesh;
	int MaxPolyCount;
};

template class _FIRE_ENGINE_API_ vector3<f32>;
template class _FIRE_ENGINE_API_ vector3<f64>;

}

#endif // OCTREE_H_INCLUDED
