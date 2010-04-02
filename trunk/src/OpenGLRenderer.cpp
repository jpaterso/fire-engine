/**
 *  $Id: OpenGLRenderer.cpp 114 2007-10-04 04:57:03Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Code for the OpenGL renderer
**/

#include "OpenGLRenderer.h"
#include "Logger.h"
#include "Image.h"
#include "Material.h"
#include "OpenGLTexture.h"
#include "Vertex3.h"
#include "Light.h"
#include "Device.h"
#include "IWindowManager.h"
#include "MediaManager.h"

#define OPENGL_MAX_LIGHTS 0x08

namespace fire_engine
{

OpenGLRenderer::OpenGLRenderer()
	: glWindowPos2iARB(0), glActiveTextureARB(0)
{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	setDebugName("fire_engine::OpenGLRenderer");
#endif
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::isExtensionSupported(const c8 * name)
{
	static const u8 * ext = glGetString(GL_EXTENSIONS);

	if (ext == 0)
	{
		Logger::Get()->log(ES_CRITICAL, "OpenGLRenderer", "could not load GL extension list");
		return false;
	}

	if (strstr((const c8*)ext, name) != 0)
		return true;
	else
		return false;
}

void OpenGLRenderer::init()
{
#if defined(_FIRE_ENGINE_DEBUG_OPENGL_)
	Logger::Get()->log(ES_DEBUG, "OpenGLRenderer", "Running OpenGL version %s", glGetString(GL_VERSION));
#endif
	loadExtensions();
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glFrontFace(GL_CCW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	mLightInfo.next_light = 0;
}

void OpenGLRenderer::loadExtensions()
{
	if (isExtensionSupported("GL_ARB_window_pos"))
	{
		glWindowPos2iARB = (PFNGLWINDOWPOS2IARBPROC) wglGetProcAddress("glWindowPos2iARB");
#ifdef	_FIRE_ENGINE_DEBUG_OPENGL_
		if (glWindowPos2iARB != 0)
			Logger::Get()->log(ES_DEBUG, "OpenGLRenderer",
				"glWindowPos2iARB extension loaded");
		else
			Logger::Get()->log(ES_MEDIUM, "OpenGLRenderer",
				"Could not load glWindowPos2iARB extension");
#endif
	}
	if (isExtensionSupported("GL_ARB_multitexture"))
	{
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
#ifdef _FIRE_ENGINE_DEBUG_OPENGL_
		if (glActiveTextureARB != 0)
			Logger::Get()->log(ES_DEBUG, "OpenGLRenderer",
				"glActivateTextureARB extension loaded");
		else
			Logger::Get()->log(ES_MEDIUM, "OpenGLRenderer",
				"Could not load glActivateTextureARB extension");
#endif
	}
}

void OpenGLRenderer::onResize(const dimension2i& newSize)
{
	mViewport = newSize;
	glViewport(0, 0, mViewport.getWidth(), mViewport.getHeight());
	//mMatrices[EMM_PROJECTION].loadProjectionMatrixFOV(Math32::PI_ON_3,
	//	(f32)mViewport.getWidth()/(f32)mViewport.getHeight(), 1.0f, 500.0f);
	//setTransform(EMM_PROJECTION, mMatrices[EMM_PROJECTION]);
}

void OpenGLRenderer::startScene(const Color32& c)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(c.red(), c.green(), c.blue(), c.alpha());
}

void OpenGLRenderer::endScene()
{
	Device::Get()->getWindowManager()->swapBuffers();
	removeAllDynamicLights();
}

void OpenGLRenderer::setTexture(s32 unit, const ITexture * texture)
{
	if (texture != nullptr)
	{
		const OpenGLTexture * ogl_texture = dynamic_cast<const OpenGLTexture *>(texture);
		glActiveTextureARB(GL_TEXTURE0_ARB + unit);
		glBindTexture(GL_TEXTURE_2D, ogl_texture->getTextureName());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void OpenGLRenderer::showImage(const Image& image, const dimension2i& pos, const dimension2f& zoom)
{
	GLenum format = 0;
	GLenum type = 0;
	GLint glBlendSRC, glBlendDST;
	bool blending;

	// Look at the current blending and lighting options
	// We want to disable lighting, because it causes problems with
	// static pixel rendering.
	// We only want blending to be on if the image actually supports
	// blending, ie. if it has a valid alpha channel. This is determined
	// by the image loader, and is an attribute of the Image class.
	//TODO: Is there a better way to do this?
	blending = (glIsEnabled(GL_BLEND) == GL_TRUE) ? true : false;

	glPushAttrib(GL_LIGHTING_BIT);

	if (blending)
	{
		glGetIntegerv(GL_BLEND_SRC, &glBlendSRC);
		glGetIntegerv(GL_BLEND_DST, &glBlendDST);
		if (image.useAlphaChanel())
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else
			glDisable(GL_BLEND);
	}
	else
	{
		if (image.useAlphaChanel())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	if (glWindowPos2iARB != NULL)
		glWindowPos2iARB(pos.getWidth(), pos.getHeight());
	else
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0.0, (f32) mViewport.getWidth(), 0.0, (f32) mViewport.getHeight());
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				glRasterPos2i(pos.getWidth(), pos.getHeight());
			glPopMatrix();
		glPopMatrix();
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelZoom(zoom.getWidth(), zoom.getHeight());

	switch (image.type())
	{
	case Image::EIDT_NONE:
		return;
	case Image::EIDT_A1R5G5B5:
		type   = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		format = GL_BGRA_EXT;
		break;
	case Image::EIDT_R5G6B5:
		type   = GL_UNSIGNED_SHORT_5_6_5_REV;
		format = GL_RGB;
		break;
	case Image::EIDT_R8G8B8:
		type   = GL_UNSIGNED_BYTE;
		format = GL_RGB;
		break;
	case Image::EIDT_A8R8G8B8:
		//TODO: Why won't GL_UNSIGNED_INT_8_8_8_8_REV work?!
		//type   = GL_UNSIGNED_INT_8_8_8_8_REV;
		type   = GL_UNSIGNED_BYTE;
		format = GL_BGRA_EXT;
		break;
	}

	glDrawPixels(image.width(), image.height(), format, type, (const void *) image.data());

	// Reset the blending options back to what they were
	if (blending)
	{
		if (!image.useAlphaChanel())
			glEnable(GL_BLEND);
		glBlendFunc(glBlendSRC, glBlendDST);
	}
    else
	{
		if (image.useAlphaChanel())
			glDisable(GL_BLEND);
	}

	// And restore lighting if it was on
	glPopAttrib();
}

void OpenGLRenderer::draw2DRectangle(const vector2f bl, const vector2f tr, ITexture * texture)
{
	setTexture(0, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(bl.getX(), bl.getY());
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(tr.getX(), bl.getY());
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(tr.getX(), tr.getY());
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(bl.getX(), tr.getY());
	glEnd();
	setTexture(0, 0);
}

void OpenGLRenderer::draw3Dline(Color32 color, const vector3f& start, const vector3f& finish)
{
	glColor3fv(color.v());
	glBegin(GL_LINES);
		glVertex3fv(start.v());
		glVertex3fv(finish.v());
	glEnd();
}

void OpenGLRenderer::drawAABoundingBox(const AABoundingBoxf& box, const Color32& color)
{
	vector3f min = box.getMinPoint();
	vector3f max = box.getMaxPoint();
	// turn off lighting
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	draw3Dline(color, min, vector3f(max.getX(), min.getY(), min.getZ()));
	draw3Dline(color, min, vector3f(min.getX(), min.getY(), max.getZ()));
	draw3Dline(color, min, vector3f(min.getX(), max.getY(), min.getZ()));
	draw3Dline(color, vector3f(min.getX(), max.getY(), max.getZ()), max);
	draw3Dline(color, vector3f(max.getX(), min.getY(), max.getZ()), max);
	draw3Dline(color, vector3f(max.getX(), max.getY(), min.getZ()), max);
	draw3Dline(color, vector3f(max.getX(), min.getY(), min.getZ()), vector3f(max.getX(), min.getY(), max.getZ()));
	draw3Dline(color, vector3f(min.getX(), min.getY(), max.getZ()), vector3f(max.getX(), min.getY(), max.getZ()));
	draw3Dline(color, vector3f(max.getX(), min.getY(), min.getZ()), vector3f(max.getX(), max.getY(), min.getZ()));
	draw3Dline(color, vector3f(min.getX(), max.getY(), min.getZ()), vector3f(max.getX(), max.getY(), min.getZ()));
	draw3Dline(color, vector3f(min.getX(), max.getY(), min.getZ()), vector3f(min.getX(), max.getY(), max.getZ()));
	draw3Dline(color, vector3f(min.getX(), min.getY(), max.getZ()), vector3f(min.getX(), max.getY(), max.getZ()));
	glPopAttrib();
}

IRenderer * OpenGLRenderer::Create()
{
	if (mInstance == nullptr)
	{
		mInstance = new OpenGLRenderer();
	}
	return mInstance;
}

void OpenGLRenderer::drawIndexedPrimitiveList(EPOLYGON_TYPE primitiveType,
	s32 num_primitives, const Vertex3 * vertices, const u32 * indices)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	if (primitiveType != EPT_POINTS)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3), (const void*)vertices->getColor().v());
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3), (const void*)vertices->getPosition().v());
	if (primitiveType != EPT_POINTS)
	{
		glNormalPointer(GL_FLOAT, sizeof(Vertex3), (const void*)vertices->getNormal().v());
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3),
			(const void*)vertices->getTextureCoordinates().v());
	}

	switch (primitiveType)
	{
	case EPT_LINES:
		glDrawElements(GL_LINES, num_primitives, GL_UNSIGNED_INT, (const void*)indices);
		break;
	case EPT_POINTS:
		glDrawArrays(GL_POINTS, 0, num_primitives);
		break;
	case EPT_TRIANGLES:
		glDrawElements(GL_TRIANGLES, num_primitives, GL_UNSIGNED_INT, (const void*)indices);
		break;
	case EPT_QUADS:
		glDrawElements(GL_QUADS, num_primitives, GL_UNSIGNED_INT, (const void*)indices);
		break;
	case EPT_TRIANGLE_STRIP:
		glDrawElements(GL_TRIANGLE_STRIP, num_primitives, GL_UNSIGNED_INT, (const void*)indices);
		break;
	case EPT_TRIANGLE_FAN:
		glDrawElements(GL_TRIANGLE_FAN, num_primitives, GL_UNSIGNED_INT, (const void*)indices);
		break;
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (primitiveType != EPT_POINTS)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

Image * OpenGLRenderer::screenshot(void) const
{
	Image * screen = new Image(Image::EIDT_R8G8B8, mViewport, false);
	glReadPixels(0, 0, mViewport.getWidth(), mViewport.getHeight(),
		GL_RGB, GL_UNSIGNED_BYTE, (u8*)screen->data());
	return screen;
}

void OpenGLRenderer::addDynamicLight(Light * light)
{
	// Can only have a certain number of lights
	if (mLightInfo.next_light >= OPENGL_MAX_LIGHTS)
		return;
	GLenum  id = GL_LIGHT0+mLightInfo.next_light++;
	GLfloat position[4];
	glLightfv(id, GL_AMBIENT, light->getAmbient().v());
	glLightfv(id, GL_DIFFUSE, light->getDiffuse().v());
	glLightfv(id, GL_SPECULAR, light->getSpecular().v());
	switch (light->getType())
	{
	case ELT_POINT:
		memcpy(position, light->getPosition().v(), 3*sizeof(f32));
		position[3] = 1.0f;
		glLightfv(id, GL_POSITION, position);
		glLightfv(id, GL_SPOT_DIRECTION, light->getDirection().v());
		glLightf(id, GL_SPOT_CUTOFF, light->getRadius());
		break;
	case ELT_DIRECTIONAL:
		memcpy(position, light->getDirection().v(), 3*sizeof(f32));
		position[3] = 0.0f;
		glLightfv(id, GL_POSITION, position);
		break;
	}

	glEnable(id);
}

void OpenGLRenderer::removeAllDynamicLights()
{
	for (s32 i = 0; i < mLightInfo.next_light; i++)
		glDisable(GL_LIGHT0+i);
	mLightInfo.next_light = 0;
}

void OpenGLRenderer::setAmbientLight(Color32 ambient)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient.v());
}

void OpenGLRenderer::makeGLMatrix(const matrix4f& mat, GLfloat * store)
{
	memcpy(store, mat.getColumnMajor(), 16*sizeof(f32));
}

void OpenGLRenderer::setTransform(EMATRIX_MODE mmode, const matrix4f& mat)
{
	GLfloat glmat[16];
	mMatrices[mmode] = mat;
	switch (mmode)
	{
	case EMM_PROJECTION:
		makeGLMatrix(mat, glmat);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glmat);
		glMatrixMode(GL_MODELVIEW);
		break;

	case EMM_VIEW:
	case EMM_MODEL:
		makeGLMatrix(mMatrices[EMM_VIEW]*mMatrices[EMM_MODEL], glmat);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);
		break;

	case EMM_TEXTURE:
		break;

	default:
		break;
	}
}

matrix4f OpenGLRenderer::getTransform(EMATRIX_MODE mmode) const
{
	return mMatrices[mmode];
}

void OpenGLRenderer::setMaterial(const Material& mat)
{
	mMaterial = mat;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mMaterial.getAmbient().v());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mMaterial.getDiffuse().v());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mMaterial.getSpecular().v());
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mMaterial.getEmissive().v());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mMaterial.getShininess());

	if (mMaterial.getMaterialProperty(EMP_LIGHTING))
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	if (mMaterial.getMaterialProperty(EMP_WIREFRAME))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (mMaterial.getMaterialProperty(EMP_WRITE_TO_Z_BUFFER))
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

ITexture * OpenGLRenderer::createTexture(const string& filename, io::IFileProvider * fileProvider) const
{
	Image * im = MediaManager::Get()->load<Image>(filename, fileProvider);
	if (im != nullptr)
	{
		return new OpenGLTexture(im);
	}
	return nullptr;
}
} // namespace fire_engine
