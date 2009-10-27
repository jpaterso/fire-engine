#include "MeshModifier.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "Vertex3.h"
#include "vector3.h"


namespace fire_engine
{

void MeshModifier::ApplyTransform(IMesh * mesh, const matrix4f& transform)
{
	IMeshBuffer * mb = 0;
	Vertex3 * vertices = 0;
	for (s32 i = 0; i < mesh->getMeshBufferCount(); i++)
	{
		mb = mesh->getMeshBuffer(i);
		vertices = mb->_getOriginalVertices();
		for (s32 j = 0; j < mb->_getOriginalVertexCount(); j++)
		{
			vertices[j].setPosition(transform.applyTransformation(vertices[j].getPosition()));
			vertices[j].setNormal(transform.applyTransformation(vertices[j].getNormal()));
			vertices[j].getNormal().normalize();
		}
	}
}

}
