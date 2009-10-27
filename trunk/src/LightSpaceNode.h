/**
 * FILE:    LightSpaceNode.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: LightSpaceNode.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A light in the 3D world, in the space node hierarchy.
**/

#ifndef LIGHTSPACENODE_H_INCLUDED
#define LIGHTSPACENODE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Light.h"
#include "ISpaceNode.h"

namespace fire_engine
{

class IRenderer;

class _FIRE_ENGINE_API_ LightSpaceNode : public ISpaceNode
{
public:
	LightSpaceNode(Light * light = 0);

	virtual ~LightSpaceNode();

	virtual s32 render(IRenderer * rd);

	void attachLight(Light * light);
	void detachLight();

	const Light * getLight() const;
private:
	Light * m_light;
};

}

#endif // LIGHTSPACENODE_H_INCLUDED
