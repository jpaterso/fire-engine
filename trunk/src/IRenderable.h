/**
 * FILE:    IRenderable.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IRenderable.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An interface for objects that can be rendered
**/

#ifndef IRENDERABLE_H_INCLUDED
#define IRENDERABLE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

class IRenderer;

//! Something that can be rendered
class _FIRE_ENGINE_API_ IRenderable
{
public:
	/** Render given an IRenderer. */
	virtual s32 render(IRenderer * rd) = 0;
};

}

#endif // IRENDERABLE_H_INCLUDED
