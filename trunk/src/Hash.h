#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

class string;

/** A simple template used to hash some value. Used by the HashTable class. */
template <class Key>
class _FIRE_ENGINE_API_ Hash
{
public:
	/** Returns the hash of some key. */
	static u32 hash(const Key& key);

	/** Returns whether two keys are equal. */
	static bool equal(const Key& a, const Key& b);
};

/** Custom hash function, so that strings that differ only by case return the
 same hash value */
class _FIRE_ENGINE_API_ StringHashIgnoreCase
{
public:
	/** Hash function for strings, ignoring case so that two strings that differ only
	 on case will have the same hash value. */
	static u32 hash(const string& key);

	/** Returns whether two strings are equal, ignoring case. */
	static bool equal(const string& a, const string& b);
};

}


#endif // HASH_H_INCLUDED
