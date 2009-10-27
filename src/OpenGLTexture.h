/**
 * FILE:    OpenGLTexture.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: OpenGLTexture.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: An implementation of a texture for OpenGL
**/

#ifndef OPENGLTEXTURE_H_INCLUDED
#define OPENGLTEXTURE_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "ITexture.h"
#include "dimension2.h"
#include "Object.h"

#ifdef _FIRE_ENGINE_WIN32_
#	include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

namespace fire_engine
{

class Image;

/** Implementation of a texture used in an OpenGL context. */
class _FIRE_ENGINE_API_ OpenGLTexture : public ITexture, public virtual Object
{
public:
	OpenGLTexture(Image * image = 0, u32 creation_flags = 0x00);

	virtual ~OpenGLTexture(void);

	virtual void update(const dimension2i& dim);

	//! Get the internal texture name, as assigned by OpenGL glGenTextures
	inline GLuint getTextureName(void) const;

private:
	GLuint mGLTextureName;
	GLenum mGLFormat;
	GLenum mGLType;

	//! Set the internal format and type of the pixel data, as used by OpenGL
	void setDataFormat(void);
};

inline GLuint OpenGLTexture::getTextureName(void) const
{
	return mGLTextureName;
}

}

#endif // OPENGLTEXTURE_H_INCLUDED
