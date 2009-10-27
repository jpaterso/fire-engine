
#ifndef MESHMODIFIER_H_INCLUDED
#define MESHMODIFIER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "matrix4.h"

namespace fire_engine
{

class IMesh;

/** A class containing some simple methods to modify the entirety of a given IMesh. It should
 probably not be used during rendering, but rather during the loading of IMeshes. */
class _FIRE_ENGINE_API_ MeshModifier
{
public:
	/** Applies a given transform to all the vertices in the IMesh. */
	static void ApplyTransform(IMesh * mesh, const matrix4f& transform);

};

}

#endif // MESHMODIFIER_H_INCLUDED
