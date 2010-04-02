/**
 *  $Id: IRenderer.h 114 2007-10-04 04:57:03Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Interface for renderers. All renderers should extend this class and
 *  implement the virtual functions defined here.
**/

#ifndef	IRENDERER_H_INCLUDED
#define	IRENDERER_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "Color.h"
#include "dimension2.h"
#include "matrix4.h"
#include "vector3.h"
#include "vector2.h"
#include "Array.h"
#include "Object.h"
#include "IMeshBuffer.h"
#include "AABoundingBox.h"

namespace fire_engine
{

// Forward declarations
class Image;
class Material;
class IndexedTriangle;
class ITexture;
class Light;
class Device;

namespace io
{
class IFileProvider;
}

enum EMATRIX_MODE
{
	/** The projection matrix, used to define the volume in physical space that is
	 to be rendered, as well as how that that volume should be seen. matrix4 has
	 methods for easily creating projection matrices. */
	EMM_PROJECTION = 0x00,
	/** The view matrix should not exist per say, but it is an easy transformation
	 to use, to simulate the existence of a Camera in space. */
	EMM_VIEW,
	/** The modelview matrix is applied to all physical objects to be renderd. */
	EMM_MODEL,
	/** The texture matrix is applied to texture coordinates before accessing the
	 texture. */
	EMM_TEXTURE,
	/** Do not use this, it is to count the number of different matrix modes
	 supported. */
	EMM_MATRIX_MODE_COUNT
};

class _FIRE_ENGINE_API_ IRenderer : public virtual Object
{
public:
	/** Destructor. */
	virtual ~IRenderer();

	static IRenderer * Get();

	/** Method used to initialise the renderer. */
	virtual void init() = 0;

	/** Callback function when the window size is modified. */
	virtual void onResize(const dimension2i& newSize) = 0;

	const dimension2i& getViewport() const
	{
		return mViewport;
	}

	/** Start a new scene, clear buffers, and so on. */
	virtual void startScene(const Color32& c) = 0;

	/** End the scene, flush buffers and swap them. */
	virtual void endScene() = 0;

	//! Set the current texture being used
	/** Set the current texture being used.
	 \param unit    The stage in the texture rendering
	 \param texture The texture to use */
	virtual void setTexture(s32 unit, const ITexture * texture) = 0;

	/**
	 *	Show an image on the screen, at a user-defined location
	 *	@param	image	The Image object to show.
	 *	@param	pos		The position (x, y) on the screen of the upper
	 *					left-hand corner of the image
	 *	@param	zoom	The zoom to use on the image.
	 *					use dimension2f (1.0f, 1.0f) for no zoom.
	**/
	virtual void showImage(const Image& image, const dimension2i& pos,
		const dimension2f& zoom) = 0;

	/** Draw a 2D rectangle, with a given ITexture. */
	virtual void draw2DRectangle(const vector2f bl, const vector2f tr,
		ITexture * texture) = 0;

	/** Draw a line in 3D space. */
	virtual void draw3Dline(Color32 color, const vector3f& start, const vector3f& finish) = 0;

	/** Draw an Axis-Aligned Bounding Box in the scene. Can be useful for
	 debugging. */
	virtual void drawAABoundingBox(const AABoundingBoxf& box, const Color32& color) = 0;

	/** Draw a list of indexed primitives, of type primitiveType. */
	virtual void drawIndexedPrimitiveList(EPOLYGON_TYPE primitiveType,
		s32 num_primitives, const Vertex3 * vertices, const u32 * indices) = 0;

	/** Returns a screenshot, in R8G8B8 color format. */
	virtual Image * screenshot(void) const = 0;

	/** Add a new light to the scene. */
	virtual void addDynamicLight(Light * light) = 0;

	/** Remove all the dynamic lights from the scene. */
	virtual void removeAllDynamicLights() = 0;

	/** Set an ambient light to the entire scene. */
	virtual void setAmbientLight(Color32 ambient) = 0;

	/** Use this to set the current transformation.
	 \param mmode The type of matrix to set.
	 \param mat   The matrix to use. */
	virtual void setTransform(EMATRIX_MODE mmode, const matrix4f& mat) = 0;

	/** Returns the transform currently being used by the renderer.
	 \param mmode The desired transform. */
	virtual matrix4f getTransform(EMATRIX_MODE mmode) const = 0;

	/** Sets the material that should be used when rendering. */
	virtual void setMaterial(const Material& mat) = 0;

	/** Asks the IRenderer to create a texture from a given file. */
	virtual ITexture * createTexture(const string& filename, io::IFileProvider * fileProvider) const = 0;

protected:
	static IRenderer * mInstance; //! Singleton instance of the IRenderer
	dimension2i        mViewport; //! The current viewport
};

} // namespace fire_engine

#endif // IRENDERER_H_INCLUDED
