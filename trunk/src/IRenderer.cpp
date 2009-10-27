/**
 * FILE:    IRenderer.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IRenderer.cpp 114 2007-10-04 04:57:03Z jpaterso $
 * PURPOSE: Implementing the IRenderer class
**/

#include "IRenderer.h"

namespace fire_engine
{

IRenderer * IRenderer::mInstance = 0;

IRenderer::~IRenderer()
{
}

IRenderer * IRenderer::Get()
{
	return mInstance;
}

}
