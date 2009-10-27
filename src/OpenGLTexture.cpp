/**
 * FILE:    OpenGLTexture.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: OpenGLTexture.cpp 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: Implementation of the OpenGLTexture class
**/

#include "OpenGLTexture.h"
#include <stdio.h>
namespace fire_engine
{

OpenGLTexture::OpenGLTexture(Image * image, u32 creation_flags)
	: mGLTextureName(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::OpenGLTexture");
#endif
	mImage         = image;
	mCreationFlags = creation_flags;

	if (mImage != nullptr)
	{
		mImage->grab();
		mDimension = image->dim();
		setDataFormat();

		glGenTextures(1, &mGLTextureName);
		glBindTexture(GL_TEXTURE_2D, mGLTextureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mDimension.getWidth(), mDimension.getHeight(),
			0, mGLFormat, mGLType, mImage->data());
	}
}

OpenGLTexture::~OpenGLTexture(void)
{
	glDeleteTextures(1, &mGLTextureName);
	if (mImage != nullptr)
	{
		mImage->drop();
	}
}

void OpenGLTexture::update(const dimension2i &dim)
{
}

void OpenGLTexture::setDataFormat(void)
{
	switch (mImage->type())
	{
	case Image::EIDT_NONE:
		return;
	case Image::EIDT_A1R5G5B5:
		mGLFormat = GL_BGRA;
		mGLType   = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;
	case Image::EIDT_R5G6B5:
		mGLFormat = GL_BGR;
		mGLType   = GL_UNSIGNED_SHORT_5_6_5_REV;
		break;
	case Image::EIDT_R8G8B8:
		mGLFormat = GL_RGB;
		mGLType   = GL_UNSIGNED_BYTE;
		break;
	case Image::EIDT_A8R8G8B8:
		mGLFormat = GL_BGRA;
		mGLType   = GL_UNSIGNED_INT_8_8_8_8_REV;
	}
}

}
