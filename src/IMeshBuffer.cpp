/**
 * FILE:    IMeshBuffer.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id$
 * PURPOSE: Implementation of non-virtual methods in the IMeshBuffer class.
**/

#include "IMeshBuffer.h"
#include "Logger.h"

namespace fire_engine
{

s32 IMeshBuffer::getPolygonCount() const
{
	int polygonCount = 0;
	int numVertices = getVertexCount();
	int numIndices = getIndices()->size();
	switch (getPolygonType())
	{
	case EPT_POINTS:
		polygonCount = numIndices;
		break;
	case EPT_LINES:
		polygonCount = 1;
		break;
	case EPT_TRIANGLES:
		polygonCount = numIndices / 3;
		if ((numIndices % 3) != 0)
		{
			Logger::Get()->log(ES_DEBUG, "IMeshBuffer::getPolygonCount", "Polygon count error - number is not an integer.");
		}
		break;
	case EPT_QUADS:
		polygonCount = numIndices / 4;
		if ((numIndices % 4) != 0)
		{
			Logger::Get()->log(ES_DEBUG, "IMeshBuffer::getPolygonCount", "Polygon count error - number is not an integer.");
		}
		break;
	case EPT_TRIANGLE_STRIP:
		polygonCount = numIndices-3;
		break;
	case EPT_TRIANGLE_FAN:
		polygonCount = (numIndices-1)/2;
		if (((numIndices-1)% 2) != 0)
		{
			Logger::Get()->log(ES_DEBUG, "IMeshBuffer::getPolygonCount", "Polygon count error - number is not an integer.");
		}
		break;
	}
	return polygonCount;
}

}
