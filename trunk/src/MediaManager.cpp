/**
 * FILE:    MediaManager.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MediaManager.cpp 99 2007-08-19 23:51:06Z jpaterso $$
 * PURPOSE: Implementation of non-templated methods of the MediaManager class
**/

#include "MediaManager.h"
#include "ImageLoaderBMP.h"
#include "ImageLoaderTGA.h"
#include "ImageLoaderPCX.h"
#include "AnimatedMeshMD2Loader.h"
#include "AnimatedMeshMD3Loader.h"
#include "Q3MapLoader.h"

namespace fire_engine
{

MediaManager * MediaManager::mInstance = 0;

MediaManager::MediaManager()
{
}

MediaManager * MediaManager::Create()
{
	if (mInstance == nullptr)
	{
		mInstance = new MediaManager();
	}
	return mInstance;
}

MediaManager * MediaManager::CreateDefault()
{
	if (mInstance == nullptr)
	{
		mInstance = new MediaManager();
	}
	mInstance->setDefaults();
	return mInstance;
}

MediaManager * MediaManager::Get()
{
	return mInstance;
}

MediaManager::~MediaManager()
{
	mInstance = nullptr;
}

void MediaManager::setDefaults()
{
	ImageLoaderBMP * ilbmp = new ImageLoaderBMP();
	ImageLoaderTGA * iltga = new ImageLoaderTGA();
	ImageLoaderPCX * ilpcx = new ImageLoaderPCX();
	addLoader("bmp", ilbmp);
	addLoader("tga", iltga);
	addLoader("pcx", ilpcx);
	addLoader("md2", new AnimatedMeshMD2Loader());
	addLoader("md3", new AnimatedMeshMD3Loader());
	addLoader("bsp", new Q3MapLoader());
	addWriter("bmp", ilbmp);
	addWriter("tga", iltga);
	addWriter("pcx", ilpcx);
}

}
