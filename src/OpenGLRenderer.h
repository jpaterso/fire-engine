/**
 *  $Id: OpenGLRenderer.h 114 2007-10-04 04:57:03Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Class declaration for the OpenGL renderer.
**/

#ifndef	RENDEREROPENGL_H_INCLUDED
#define	RENDEREROPENGL_H_INCLUDED

#include <stdio.h>
#include "CompileConfig.h"

#ifdef _FIRE_ENGINE_WIN32_
#	include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "Types.h"
#include "Color.h"
#include "dimension2.h"
#include "IRenderer.h"
#include "Object.h"
#include "matrix4.h"
#include "Material.h"

namespace fire_engine
{

// Forward declarations
class Image;
class Light;
class Device;

/** The OpenGL renderer. When the chosen driver for rendering is OpenGL, an instance of
 this class will be created. All drawing is done here, as well as other things such as
 lighting and shadows (not yet). */
class _FIRE_ENGINE_API_ OpenGLRenderer : public IRenderer, public virtual Object
{
public:
	/**
	 *	Basic destructor, does nothing
	**/
	virtual ~OpenGLRenderer();

	/**
	 *	Initialise the renderer. This call must be made AFTER the
	 *	WindowManager has created an OpenGL context.
	**/
	virtual void init();

	/**
	 *	Create a singleton instance of this class
	 *	@return	A pointer to the singleton instance of this class
	**/
	static IRenderer * Create();

	virtual void onResize(const dimension2i& newSize);

	virtual void startScene(const Color32& c);

	virtual void endScene();

	virtual void setTexture(s32 unit, const ITexture * texture);

	virtual void showImage(const Image& image, const dimension2i& pos,
		const dimension2f& zoom);

	virtual void draw2DRectangle(const vector2f bl, const vector2f tr,
		ITexture * texture);

	virtual void draw3Dline(Color32 color, const vector3f& start, const vector3f& finish);

	virtual void drawAABoundingBox(const AABoundingBoxf& box, const Color32& color);

	virtual void drawIndexedPrimitiveList(EPOLYGON_TYPE primitiveType,
		s32 num_primitives, const Vertex3 * vertices, const u32 * indices);

	virtual Image * screenshot() const;

	virtual void addDynamicLight(Light * light);

	virtual void removeAllDynamicLights();

	virtual void setAmbientLight(Color32 ambient);

	virtual void setTransform(EMATRIX_MODE mmode, const matrix4f& mat);

	virtual matrix4f getTransform(EMATRIX_MODE mmode) const;

	virtual void setMaterial(const Material& mat);

	virtual ITexture * createTexture(const string& filename) const;

private:
	/** A list of extensions that are used. */
	PFNGLWINDOWPOS2IARBPROC   glWindowPos2iARB;
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

	/** The diverse transformations that we need to keep track of:
	 EMM_VIEW       The 'view' matrix (does not exist in OpenGL, but we emulate it.
	 EMM_MODELVIEW  A transformation applied every primitive drawn.
	 EMM_PROJECTION Defines what volume of 3D space is visible. */
	matrix4f mMatrices[EMM_MATRIX_MODE_COUNT];

	Material mMaterial;

	typedef struct
	{
		s32 next_light;
	} light_info_t;

	light_info_t mLightInfo;

	/** Constructor - made private to ensure only a singleton instance is created. */
	OpenGLRenderer();

	/** Load a pre-defined list of OpenGL extensions. */
	void loadExtensions(void);

	/** Returns whether a given OpenGL extension is available. */
	bool isExtensionSupported(const c8 * name);

	void makeGLMatrix(const matrix4f& mat, GLfloat * store);

};

} // namespace fire_engine

#endif // RENDEREROPENGL_H_INCLUDED
