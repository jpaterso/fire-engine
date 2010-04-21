#ifndef OCTREE_H_INCLUDED
#define OCTREE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Object.h"
#include "aabbox.h"
#include "IMesh.h"
#include "Vertex3.h"
#include "Array.h"
#include "ViewFrustum.h"
#include "IMeshBuffer.h"

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

	Array<const IMeshBuffer*> getVisibleNodes(const aabbox<T>& box) const
	{
		Array<const IMeshBuffer*> visibleNodes;
		return visibleNodes;
	}

	Array<const IMeshBuffer*> getVisibleNodes(const ViewFrustum& frustum) const
	{
		Array<const IMeshBuffer*> visibleNodes;
		TreeRoot->addVisibleNodes(visibleNodes, frustum);
		return visibleNodes;
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
		TreeRoot = new OctreeNode(this, 0, MaxPolyCount, nullptr, true);
	}

	/** A piece of a Mesh Buffer stored in a leaf node of the Octree.
	class _FIRE_ENGINE_API_ OctreeMeshBufferChunk : public CMeshBuffer
	{
	public:
		OctreeMeshBufferChunk(Vertex3 * vertices, s32 vertexCount, 
			Array<u32> * indices, EPOLYGON_TYPE polygonType, Material mat)
			: CMeshBuffer(vertices, vertexCount, indices, polygonType, mat)
		{
		}

		virtual ~OctreeMeshBufferChunk()
		{
		}
	}; */

	/** A node in the Octree. */
    class _FIRE_ENGINE_API_ OctreeNode
    {
    public:
        OctreeNode(Octree * tree, u32 depth, u32 maxPolyCount, Array<u32> ** indices, bool shareVertices)
			: Depth(depth)
        {
			/*if (ShareVertices && Depth = 0)
			{
				//Vertices = new Vertex3*[mesh->getMeshBufferCount()];
				for (u32 i = 0; i < tree->Mesh->getMeshBufferCount(); i++)
				{
					//Vertices[i] = mesh->getMeshBuffer(i)->getVertices();
				}
			}*/

			buildNode(tree, maxPolyCount, nullptr);
        }

		void addVisibleNodes(Array<const IMeshBuffer*>& nodeArray, const ViewFrustum& frustum)
		{
			if (!IsLeaf)
			{
				for (u32 i = 0; i < 8; i++)
				{
					if (Children[i] != nullptr)
					{
						Children[i]->addVisibleNodes(nodeArray, frustum);
					}
				}
			}
			else
			{
				if (frustum.calculateIntersection(Box) != EFIT_OUTSIDE)
				{
				}
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
        }

        inline bool isLeaf() const
        {
            return IsLeaf;
        }

    private:
		u32                     Depth;
		bool                    IsLeaf;
		IMesh *                 OriginalMesh;
		Octree *                Tree;
        OctreeNode *            Children[8];
        aabbox<T>               Box;

		void buildNode(Octree * tree, u32 maxPolyCount, Array<u32> ** indices)
		{
			u32 polyCount = 0;
			s32 meshBufferCount = tree->Mesh->getMeshBufferCount(); 

			if (indices == nullptr)
			{
				for (s32 i = 0; i < meshBufferCount; i++)
				{
					IMeshBuffer * mb = tree->Mesh->getMeshBuffer(i);
					const Vertex3 * verts = mb->getVertices();
					for (s32 j = 0; j < mb->getVertexCount(); j++)
					{
						Box.addInternalPoint(verts[j].getPosition());
					}

					polyCount += mb->getPolygonCount();
				}
			}
			else
			{
				for (s32 i = 0; i < meshBufferCount; i++)
				{
					if (indices[i] != nullptr)
					{
						IMeshBuffer * mb = tree->Mesh->getMeshBuffer(i);
						const Vertex3 * verts = mb->getVertices();
						for (s32 j = 0; j < indices[i]->size(); j++)
						{
							Box.addInternalPoint(verts[indices[i]->at(j)].getPosition());
						}
						polyCount += indices[i]->size();
					}
				}
			}

			Array<u32> ** newIndices = nullptr;
			if (polyCount > maxPolyCount)
			{
				IsLeaf = false;
				aabbox<T> childBox[8];
				calculateChildBoundingBoxes(childBox);

				for (u32 i = 0; i < 8; i++)
				{
					newIndices = new Array<u32>*[meshBufferCount];
					bool childContainsVertices = false;
					for (s32 j = 0; j < meshBufferCount; j++)
					{
						IMeshBuffer * mb = tree->Mesh->getMeshBuffer(i);
						const Vertex3 * meshVertices = mb->getVertices();
						const Array<u32> * nodeMBIndices = (indices != nullptr) ? indices[i] : mb->getIndices();
						Array<u32> * childIndices = nullptr;
						if (nodeMBIndices != nullptr)
						{
							Array<s32> indices;
							switch (mb->getPolygonType())
							{
							case EPT_POINTS:
								break;
							case EPT_LINES:
								break;
							case EPT_TRIANGLES:
								for (s32 k = 0; k < nodeMBIndices->size(); k+=3)
								{
									if (childBox[i].contains(meshVertices[nodeMBIndices->at(k)].getPosition()) ||
										childBox[i].contains(meshVertices[nodeMBIndices->at(k+1)].getPosition()) ||
										childBox[i].contains(meshVertices[nodeMBIndices->at(k+2)].getPosition()))
									{
										if (childIndices == nullptr)
										{
											childIndices = new Array<u32>();
											childContainsVertices = true;
										}
										childIndices->push_back(nodeMBIndices->at(k));
										childIndices->push_back(nodeMBIndices->at(k+1));
										childIndices->push_back(nodeMBIndices->at(k+2));
									}
								}
								break;
							case EPT_QUADS:
								for (s32 k = 0; k < nodeMBIndices->size(); k+=4)
								{
									if (childBox[i].contains(meshVertices[nodeMBIndices->at(k)].getPosition()) ||
										childBox[i].contains(meshVertices[nodeMBIndices->at(k+1)].getPosition()) ||
										childBox[i].contains(meshVertices[nodeMBIndices->at(k+2)].getPosition()) ||
										childBox[i].contains(meshVertices[nodeMBIndices->at(k+3)].getPosition()))
									{
										if (childIndices == nullptr)
										{
											childIndices = new Array<u32>();
											childContainsVertices = true;
										}
										childIndices->push_back(nodeMBIndices->at(k));
										childIndices->push_back(nodeMBIndices->at(k+1));
										childIndices->push_back(nodeMBIndices->at(k+2));
										childIndices->push_back(nodeMBIndices->at(k+3));
									}
								}
								break;
							case EPT_TRIANGLE_STRIP:
								break;
							case EPT_TRIANGLE_FAN:
								break;
							} // switch (mb->getPolygonType())

							newIndices[j] = childIndices;

						} // Indices available for mesh buffer?

					} // Loop through mesh buffers

					if (childContainsVertices)
					{
						Children[i] = new OctreeNode(tree, Depth+1, maxPolyCount, newIndices, true);
						if (!Children[i]->isLeaf())
						{
							// If it's not a leaf we can safely delete the indices as they will have been re-partitioned
							for (s32 j = 0; j < meshBufferCount; j++)
							{
								if (newIndices[j] != nullptr)
								{
									delete newIndices[j];
								}
							}
							delete [] newIndices;
						}
					}
					else
					{
						Children[i] = nullptr;
					}

				} // Loop through child boxes
			}
			else
			{
				IsLeaf = true;
			}
		}

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

template class _FIRE_ENGINE_API_ Octree<f32>;

}

#endif // OCTREE_H_INCLUDED
