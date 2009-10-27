/**
 * FILE:    LightSpaceNode.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: LightSpaceNode.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A light in the 3D world, in the space node hierarchy.
**/

#include "LightSpaceNode.h"
#include "IRenderer.h"
#include "Vertex3.h"

namespace fire_engine
{

LightSpaceNode::LightSpaceNode(Light * light)
	: m_light(light)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::LightSpaceNode");
#endif
	if (m_light)
		m_light->grab();
}

LightSpaceNode::~LightSpaceNode()
{
	if (m_light)
		m_light->drop();
}

s32 LightSpaceNode::render(IRenderer * rd)
{
	rd->setTransform(EMM_MODEL, mWorldTransform);
	if (mShowDebugInformation)
	{
		rd->draw3Dline(Color32(0.5f, 0.5f, 0.5f, 1.0f), m_light->getPosition(),
			m_light->getDirection()-m_light->getPosition());
		Vertex3 vertex;
		vertex.setPosition(m_light->getPosition());
		vertex.setColor(Color8(255, 0, 0, 255));
		u32 index = 0;
		rd->drawIndexedPrimitiveList(EPT_POINTS, 1, &vertex, &index);
	}
	rd->addDynamicLight(m_light);
	ISpaceNode::render(rd);
	return 0;
}

void LightSpaceNode::attachLight(Light * light)
{
	if (m_light)
		m_light->drop();
	m_light = light;
	if (m_light)
		m_light->grab();
}

void LightSpaceNode::detachLight()
{
	if (m_light)
	{
		m_light->drop();
		m_light = 0;
	}
}

const Light * LightSpaceNode::getLight() const
{
	return m_light;
}

}
