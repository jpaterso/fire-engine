/**
 * FILE:    MemoryManager.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MemoryManager.h 117 2007-10-06 00:31:15Z jpaterso $
 * PURPOSE: A simple memory manager, that can track and log memory leaks and
 *          so on.
**/

#ifndef __MEMORYMANAGER_H_INCLUDED__
#define __MEMORYMANAGER_H_INCLUDED__

#include "Types.h"
#include "CompileConfig.h"
#include <String.h>
#include <stdlib.h>

namespace fire_engine
{


/** A simple class to keep track of allocated memory.
 If used correctly it can track memory leaks. The memory manager is entirely
 self-contained, using no classes/methods from FireEngine, as that causes
 dependency issues. Unfortunately the MemoryManager induces some overhead
 in memory allocation, and should only really be used for debug purposes.*/
class _FIRE_ENGINE_API_ MemoryManager
{
public:
	enum EMEMORYMANAGERVERBOSITY
	{
		EMMV_SILENT,
		EMMV_MIN_REPORT,
		EMMV_FULL_REPORT
	};

private:
	struct block_t
	{
		size_t Size;
		bool   IsArray;
		c8 *   FileName;
		s32    LineNumber;
	};

	struct pair_t
	{
		void*   Key;
		block_t Value;
		bool    Deleted;
	};

	struct hash_table_t
	{
		pair_t ** Table;
		u32       Size;
		u32       Elements;
	};

public:
	/** Creates the memory manager if it doesn't already exist. */
	static MemoryManager * Create(EMEMORYMANAGERVERBOSITY verbosity = EMMV_MIN_REPORT);

	/** Returns the singleton instance of the memory manager. */
	static MemoryManager * Get();

	/** Destructor. */
	~MemoryManager();

	/** Allocates and logs a new block of memory
	 \param size     The size of the block to be allocated.
	 \param isArray  Whether an array was allocated or not.
	 \param file     The name of the file where the block was allocated.
	 \param line     The line that the block was allocated at. */
	void * allocate(size_t size, bool isArray, const c8 * , s32 line);

	/** Frees a block, and logs it. */
	void deallocate(void * pointer, bool isArray);

	void nextDelete(const c8 * file, s32 line);

private:
	static MemoryManager *   mInstance;
	hash_table_t *           mHashTable;
	EMEMORYMANAGERVERBOSITY  mVerbosity;

	/** Private constructor to ensure singleton existence. */
	MemoryManager(EMEMORYMANAGERVERBOSITY verbosity);

	/** Reports the blocks that have not been freed -- should be called when the application
	 exits to check for memory leeks. */
	void reportMemoryLeaks();

	const c8 * basename(const c8 * name) const;

	u32 ht_hash(void * const& key) const;

	bool ht_insert(pair_t * pair);

	void ht_rehash();

	pair_t * ht_find(void * const& key);

	bool ht_remove(void * const& key);
};

}

#include <stdio.h>

inline void * operator new(size_t size, const char * file, int line)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return malloc(size);
	return fire_engine::MemoryManager::Get()->allocate(size, false, file, line);
}

inline void * operator new[](size_t size, const char * file, int line)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return malloc(size);
	return fire_engine::MemoryManager::Get()->allocate(size, true, file, line);
}

inline void operator delete(void * pointer, const char * file, int line)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return free(pointer);
	fire_engine::MemoryManager::Get()->nextDelete(file, line);
	return fire_engine::MemoryManager::Get()->deallocate(pointer, false);
}

inline void operator delete[](void * pointer, const char * file, int line)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return free(pointer);
	fire_engine::MemoryManager::Get()->nextDelete(file, line);
	return fire_engine::MemoryManager::Get()->deallocate(pointer, true);
}

inline void operator delete(void * pointer)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return free(pointer);
	fire_engine::MemoryManager::Get()->deallocate(pointer, false);
}

inline void operator delete[](void * pointer)
{
	if (fire_engine::MemoryManager::Get() == 0)
		return free(pointer);
	fire_engine::MemoryManager::Get()->deallocate(pointer, true);
}

// We re-define the new, new[], delete and delete[] operators using some hacky maccros
#define new new(__FILE__, __LINE__)
#define delete fire_engine::MemoryManager::Get()->nextDelete(__FILE__, __LINE__), delete
#endif // __MEMORYMANAGER_H_INCLUDED__
