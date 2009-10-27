/**
 * FILE:    MemoryManager.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: MemoryManager.cpp 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: A tool to report memory leaks.
**/

#include "MemoryManager.h"
#include "Logger.h"
#include <stdio.h>

// We can't call MemoryManager's new, when creating it, because it doesn't exist yet!
#ifdef new
#	undef new
#endif

namespace fire_engine
{

MemoryManager * MemoryManager::mInstance = 0;

MemoryManager::MemoryManager(EMEMORYMANAGERVERBOSITY verbosity)
	: mVerbosity(verbosity)
{
	mHashTable = (hash_table_t*)malloc(sizeof(hash_table_t));
	mHashTable->Size     = 1024;
	mHashTable->Table    = (pair_t **) malloc(mHashTable->Size * sizeof(pair_t *));
	memset(mHashTable->Table, 0, mHashTable->Size * sizeof(pair_t *));
	if (!mHashTable->Table)
		Logger::Get()->log(ES_CRITICAL, "MemoryManager", "Could not load memory table");
	mHashTable->Elements = 0;
}

MemoryManager * MemoryManager::Create(EMEMORYMANAGERVERBOSITY verbosity)
{
	if (mInstance == 0)
		mInstance = new MemoryManager(verbosity);
	return mInstance;
}

MemoryManager * MemoryManager::Get()
{
	return mInstance;
}

MemoryManager::~MemoryManager()
{
	if (mHashTable->Elements != 0)
		reportMemoryLeaks();
	free(mHashTable);
	mInstance = 0;
}

void * MemoryManager::allocate(size_t size, bool isArray, const c8 * file, s32 line)
{
	void * pointer = malloc(size);

	if (!pointer)
		Logger::Get()->log(ES_CRITICAL, "MemoryManager", "Out of memory!");

	// Create block
	pair_t * pair = (pair_t*) malloc(sizeof(pair_t));
	pair->Key = pointer;
	pair->Value.IsArray = isArray;
	pair->Value.LineNumber = line;
	pair->Value.Size = size;
	pair->Value.FileName = (c8*) malloc((strlen(file)+1)*sizeof(c8));
	strncpy(pair->Value.FileName, file, strlen(file)+1);
	pair->Deleted = false;

	ht_insert(pair);

	return pointer;
}

void MemoryManager::deallocate(void * pointer, bool isArray)
{
	/* this == 0 can actually happen, if the compiler calls the destructor before freeing
	 memory. A simple check solves the problem. */
	pair_t * pair;
	if (this != 0)
		pair = ht_find(pointer);
	if (pair)
	{
		if (isArray != pair->Value.IsArray)
			printf("\n\n\nWrong deallocated: array vs non array\n\n\n\n");
	}
	ht_remove(pointer);
	free(pointer);
}

void MemoryManager::nextDelete(const c8 * file, s32 line)
{
}

void MemoryManager::reportMemoryLeaks()
{
	if (mVerbosity == EMMV_SILENT)
		return;
	s32 totalSize = 0;
	for (u32 i = 0; i < mHashTable->Size; i++)
	{
		if (mHashTable->Table[i] != 0)
		{
			if (!mHashTable->Table[i]->Deleted)
			{
				if (mVerbosity == EMMV_FULL_REPORT)
				{
					Logger::Get()->log(ES_MEDIUM, "MemoryManager", "Memory leak in %s line %d: %d bytes (%d)",
						mHashTable->Table[i]->Value.FileName, mHashTable->Table[i]->Value.LineNumber,
						mHashTable->Table[i]->Value.Size, mHashTable->Table[i]->Key);
				}
				totalSize += mHashTable->Table[i]->Value.Size;
			}
		}
	}
	printf("%d total -> %f mb\n", mHashTable->Elements, ((f32)totalSize/1048576));
}

const c8 * MemoryManager::basename(const c8 * name) const
{
	return name;
}

u32 MemoryManager::ht_hash(void * const& key) const
{
	u32 h;
	memcpy(&h, &key, sizeof(u32));
	h = h + (h << 10);
	h = h ^ (h >> 6);
	h = h + (h << 3);
	h = h ^ (h >> 11);
	h = h + (h << 15);
	return h;
}

bool MemoryManager::ht_insert(pair_t * pair)
{
	mHashTable->Elements++;
	if (((f32)mHashTable->Elements / mHashTable->Size) > 0.75f)
		ht_rehash();
	u32 h = ht_hash(pair->Key) % mHashTable->Size;
	while (mHashTable->Table[h] != 0)
	{
		if (mHashTable->Table[h]->Deleted)
		{
			free(mHashTable->Table[h]->Value.FileName);
			free(mHashTable->Table[h]);
			break;
		}
		if (mHashTable->Table[h]->Key == pair->Key)
			return false;
		h = (h+1) % mHashTable->Size;
	}
	mHashTable->Table[h] = pair;
	return true;
}

void MemoryManager::ht_rehash()
{
	u32 newsize = 2*mHashTable->Size;
	pair_t ** newtable = (pair_t**)malloc(newsize*sizeof(pair_t*));
	memset(newtable, 0, newsize * sizeof(pair_t *));
	u32 i = 0;
	u32 rehashed = 0;
	while (i < mHashTable->Size && rehashed < mHashTable->Elements)
	{
		if (mHashTable->Table[i] != 0)
		{
			if (!mHashTable->Table[i]->Deleted)
			{
				u32 h = ht_hash(mHashTable->Table[i]->Key) % newsize;
				while (newtable[h] != 0)
					h = (h+1) % newsize;
				newtable[h] = mHashTable->Table[i];
				rehashed++;
			}
			else
			{
				free(mHashTable->Table[i]->Value.FileName);
				free(mHashTable->Table[i]);
			}
		}
		i++;
	}
	free(mHashTable->Table);
	mHashTable->Table = newtable;
	mHashTable->Size = newsize;
}

MemoryManager::pair_t * MemoryManager::ht_find(void * const& key)
{
	u32 h = ht_hash(key) % mHashTable->Size;
	while (mHashTable->Table[h])
	{
		if (mHashTable->Table[h]->Key == key)
			break;
		h = (h+1) % mHashTable->Size;
	}
	if (mHashTable->Table[h] == 0)
		return 0;
	if (mHashTable->Table[h]->Deleted)
		return 0;
	return mHashTable->Table[h];
}

bool MemoryManager::ht_remove(void * const& key)
{
	u32 h = ht_hash(key) % mHashTable->Size;
	while (mHashTable->Table[h] != 0)
	{
		if (mHashTable->Table[h]->Key == key)
			break;
		h = (h+1) % mHashTable->Size;
	}
	if (mHashTable->Table[h] == 0)
		return false;
	else if (mHashTable->Table[h]->Key == key)
	{
		if (mHashTable->Table[h]->Deleted)
			return false;
		mHashTable->Table[h]->Deleted = true;
		mHashTable->Elements--;
		return true;
	}
	return false;
}

}
