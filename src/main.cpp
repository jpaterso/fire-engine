/**
 *  $Id: main.cpp 119 2007-12-03 02:12:08Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Test-control file
**/

#include "FireEngine.h"

using namespace fire_engine;
using namespace io;

AnimatedModel * model;
AnimatedModelMD3 * model_lower;
AnimatedModelMD3 * model_upper;
AnimatedModelMD3 * model_head;
CameraFPS * myCamera;

class MyEventReceiver : public IEventReceiver
{
public:
	virtual void onMouseEvent(MouseEvent& mevent)
	{
	}

	virtual void onKeyEvent(KeyEvent& kevent)
	{
		if (kevent.getKey() == KeyEvent::KEY_ESCAPE)
			Device::Get()->close();
	}
};

int main(int argc, const char * argv[])
{
	MediaManager * mm = MediaManager::CreateDefault();
	Device * d = Device::Create(EDT_OPENGL, dimension2i(1024, 768));
	SceneManager * sc = d->getSceneManager ();
	IWindowManager * iwm = d->getWindowManager();
	MyEventReceiver * receive = new MyEventReceiver();
	d->addEventReceiver(receive);

	Light * light = new Light(ELT_POINT, vector3f(50.0f, 50.0f,50.0f), vector3f(-1.0f, -1.0f, -1.0f),
		Color32(1.0f, 1.0f, 1.0f, 1.0f), Color32(1.0f, 1.0f, 1.0f, 1.0f), Color32(1.0f, 1.0f, 1.0f, 1.0f),
		45.0f);
	LightSpaceNode * lsn = sc->addDynamicLight(light);
	lsn->setShowDebugInformation(true);
	light->drop();

	Light * light2 = new Light(ELT_POINT, vector3f(20.0f, 45.0f, 20.0f), vector3f(-1.0f, -1.0f, -1.0f),
		Color32(0.2f, 0.2f, 0.2f, 1.0f), Color32(0.5f, 0.5f, 0.5f, 1.0f), Color32(1.0f, 1.0f, 1.0f, 1.0f),
		45.0f);
	LightSpaceNode * lsn2 = sc->addDynamicLight(light2);
	light2->drop();

	FileSystem::Get()->addDirectory("../media");
	FileSystem::Get()->addArchive("md3/md3-kt_kubalwagon.pk3");
	io::IFileProvider* fp=FileSystem::Get()->addArchive("md3/quake3-model1.pk3");
	FileSystem::Get()->addArchive("md3/chicken.pk3");
	FileSystem::Get()->addArchive("md3/galvatron.pk3");
	
	AnimatedMeshMD2 * mesh = mm->load<AnimatedMeshMD2>("md2/alien/tris.md2");
	AnimatedModel * model = sc->addAnimatedMesh(mesh);
	model->setFrameLoop(0, 40, true);
	model->setShowDebugInformation(true);
	model->setRelativePosition(vector3f(0.0f, 0.0f, -70.0f));

	//Octree<f32> * octree = new Octree<f32>(nullptr, 265);

	//model_lower = dynamic_cast<AnimatedModelMD3*>(sc->addAnimatedMesh(ammd3));
	//model_lower->setFrameLoop(0, 0, true);
	//model_lower->setRelativePosition(vector3f(0.0f, 0.0f, -70.0f));
	//model_lower->setShowDebugInformation(true);
	//ammd3->drop();

	//AnimatedMeshMD3 * ammd3_upper = mm->load<AnimatedMeshMD3>("models/players/Galvatron/upper.md3");
	//model_upper = dynamic_cast<AnimatedModelMD3*>(sc->addAnimatedMesh(ammd3_upper));
	//model_upper->setFrameLoop(0, 0, true);
	//model_upper->attach(model_lower, "tag_torso");
	//model_upper->setShowDebugInformation(true);
	//ammd3_upper->drop();

	//AnimatedMeshMD3 * ammd3_head = mm->load<AnimatedMeshMD3>("models/players/Galvatron/head.md3");
	//model_head = dynamic_cast<AnimatedModelMD3*>(sc->addAnimatedMesh(ammd3_head));
	//model_head->setFrameLoop(0, 0, true);
	//model_head->attach(model_upper, "tag_head");
	//model_head->setShowDebugInformation(true);
	//ammd3_head->drop();

	////Q3Map * map = mm->load<Q3Map>("maps/20kdm2.bsp");

	//lsn->setParent(model_lower);
	//lsn2->setParent(model_lower);
	iwm->getCursor()->setCursorVisible(false);
	myCamera = (CameraFPS*)sc->addCamera(vector3f(0.0f, 0.0f, 0.0f), vector3f(0.0f, 0.0f, -1.0f));
	HashTable<KeyEvent::EKEY_CODE, EMOVEMENT_TYPE> * moveKeys = new HashTable<KeyEvent::EKEY_CODE, EMOVEMENT_TYPE>();
	moveKeys->insert(KeyEvent::KEY_UP, EMT_FORWARD);
	moveKeys->insert(KeyEvent::KEY_DOWN, EMT_BACKWARD);
	moveKeys->insert(KeyEvent::KEY_LEFT, EMT_STRAFE_LEFT);
	moveKeys->insert(KeyEvent::KEY_RIGHT, EMT_STRAFE_RIGHT);
	moveKeys->insert(KeyEvent::KEY_SPACE, EMT_JUMP);
	myCamera->setMoveKeys(moveKeys);

	/*SkyBox * sk = sc->addSkyBox("../media/sky/marty/sky_FR.bmp", "../media/sky/marty/sky_BK.bmp",
		"../media/sky/marty/sky_UP.bmp", "../media/sky/marty/sky_DN.bmp",
		"../media/sky/marty/sky_LF.bmp", "../media/sky/marty/sky_RT.bmp"); */

	while (d->isRunning())
	{
		d->getRenderer()->startScene(Color32(0.0f, 0.0f, 0.0f, 1.0f));
		iwm->run();
		sc->draw();
		iwm->setTitle(String("Running at ")+d->getFPS()+" frames per second. Drawing " + d->_getFPSCalculator()->getLastPolyCount() + " polygons");
		//d->getRenderer()->showImage(*im, dimension2i(0, 0), dimension2f(1.0, 1.0));
		d->getRenderer()->endScene();
	}

	d->drop();
	delete receive;
	delete mm;

	return 0x00;
}
