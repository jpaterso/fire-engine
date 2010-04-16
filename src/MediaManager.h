/**
 * FILE:    MediaManager.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MediaManager.h 116 2007-10-05 13:22:45Z jpaterso $$
 * PURPOSE: A manager for file loader-writers.
**/

#ifndef MEDIAMANAGER_H_INCLUDED
#define MEDIAMANAGER_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "HashTable.h"
#include "ILoader.h"
#include "IWriter.h"
#include "Logger.h"
#include "Array.h"
#include "FileUtils.h"

namespace fire_engine
{

class Image;
class AnimatedMeshMD2;
class AnimatedMeshMD3;
class Q3Map;

namespace io
{
class IFileProvider;
}

template <class T>
struct _FIRE_ENGINE_API_ MediaHolder
{
	HashTable<String, ILoader<T> *, StringHashIgnoreCase, StringHashIgnoreCase> m_loaders;
	HashTable<String, IWriter<T> *, StringHashIgnoreCase, StringHashIgnoreCase> m_writers;

	virtual ~MediaHolder()
	{
		Array<ILoader<T>*> * loaders = m_loaders.values();
		Array<IWriter<T>*> * writers = m_writers.values();
		for (s32 i = 0; i < loaders->size(); i++)
			delete loaders->at(i);
//		for (s32 i = 0; i < writers->size(); i++)
//			delete writers->at(i);
		delete loaders;
		delete writers;
	}
};

/** The MediaManager provides an easy-to-use interface for loading Media from disk. It should
 be used whenever some type of media (image, 3d model) is to be loaded, instead of manually
 creating a loader and using it. A default set of loaders is provided. */
class _FIRE_ENGINE_API_ MediaManager : public MediaHolder<Image>, 
	                                   public MediaHolder<AnimatedMeshMD2>,
	                                   public MediaHolder<AnimatedMeshMD3>, 
									   public MediaHolder<Q3Map>
{
public:
	static MediaManager * Create();
	static MediaManager * CreateDefault();
	static MediaManager * Get();

	virtual ~MediaManager();

	template <class Object>
	void addLoader(const String& extension, ILoader<Object> * loader)
	{
		MediaHolder<Object>::m_loaders.insert(extension, loader);
	}

	template <class Object>
	void addWriter(const String& extension, IWriter<Object> * writer)
	{
		MediaHolder<Object>::m_writers.insert(extension, writer);
	}

	template <class Object>
	Object * load(const String& filename, io::IFileProvider * fileProvider = nullptr) const
	{
		ILoader<Object> ** loader = MediaHolder<Object>::m_loaders.find(io::FileUtils::GetFileExtension(filename));
		if (loader != 0)
			return (*loader)->load(filename, fileProvider);
		else // No loader registered for that type
		{
			Logger::Get()->log(ES_DEBUG, "MediaManager", "Couldn't find a loader for file %s",
				filename.c_str());
			return 0;
		}
	}

	template <class Object>
	bool write(const String& filename, const Object * object) const
	{
		IWriter<Object> ** writer = MediaHolder<Object>::m_writers.find(io::FileUtils::GetFileExtension(filename));
		if (writer != 0)
			return (*writer)->write(filename, object);
		else // No writer registered for that type
			return 0;
	}

	void setDefaults();

private:
	static MediaManager * mInstance;

	MediaManager();
};

}

#endif
