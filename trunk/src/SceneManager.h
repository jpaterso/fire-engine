/**
 *  $Id: SceneManager.h 115 2007-10-05 10:52:32Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The SceneManager class is the main class used by the FireEngine. All
 *  rendering should go through this.
 *  The SceneManager manages everything, from GuiItems to Images.
**/

#ifndef	__SCENE_MANAGER_H
#define	__SCENE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include "Types.h"
#include "CompileConfig.h"
#include "dimension2.h"
#include "Array.h"
#include "Object.h"
#include "Color.h"
#include "vector3.h"
#include "HighResolutionTimer.h"

namespace fire_engine
{

class IRenderer;
class ISpaceNode;
class IAnimatedMesh;
class AnimatedModel;
class Light;
class LightSpaceNode;
class Camera;
class SkyBox;
class IModel;

class _FIRE_ENGINE_API_ SceneManager : public virtual Object
{
	public:
		/** Destructor. */
		virtual ~SceneManager();

		/**
		 *	Draw the scene. This needs to be called at every frame for proper
		 *	processing
		**/
		void draw();

		/** Add an AnimatedMesh to the scene, and return the SpaceNode created. */
		AnimatedModel * addAnimatedMesh(IAnimatedMesh * mesh);

		/** Add a Light to the scene, and return the SpaceNode created. */
		LightSpaceNode * addDynamicLight(Light * light);

		/** Adds a Camera to the scene. */
		Camera * addCamera(const vector3f& position, const vector3f& target);

		/** Adds a SkyBox to the scene. */
		SkyBox * addSkyBox(const string& ft, const string& bk, const string& up,
			const string& dn, const string& lt, const string& rt);

		/** Returns the (currently) active camera. */
		const Camera * getActiveCamera() const;

		/** Creates an instance of the SceneManager if it doesn't exist, and returns it. */
		static SceneManager * Create(IRenderer * rd);

		/** Returns the singleton instance of the scene manager. */
		static SceneManager * Get(void);

		/** Put a request in for a screenshot to be taken. It is important to do
		 it this way, and not just call IRenderer::screenshot(), as we must make
		 sure that the entire scene is drawn before taking it. It will be taken
		 as soon as the entire scene is drawn. */
		void registerScreenShotDemand(const string& filename);

		ISpaceNode * getRoot();

	private:
		static SceneManager *    mInstance;
		IRenderer *              mRenderer;
		ISpaceNode *             mSpaceRoot;
		Array<ISpaceNode*>       mLights;
		Array<IModel*>           mSolidNodes;
		Color32                  mAmbientLight;
		Camera *                 mActiveCamera;
		sys::HighResolutionTimer mTimer;
		SkyBox *                 mSkyBox;

		typedef struct
		{
			bool   mWantScreenshot;
			string mScreenshotFilename;
		} screenshot_demand_t;

		screenshot_demand_t mScreenshotInfo;
		/**
		 *	Construct a SceneManager with a Renderer
		 *	@param	rd	The renderer to use
		**/
		SceneManager(IRenderer * rd);
};

} // namespace fire_engine

#endif
