/**
 * FILE:    IMap.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: IMap.h 116 2007-10-05 13:22:45Z jpaterso $
 * PURPOSE: An interface for a Map, ie. something that maps values from a certain domain to
 *          values from another domain. Values are stored in key-value pairs, where a key can
 *          only map to a single value, but a value can have many keys.
**/

#ifndef IMAP_H_INCLUDED
#define IMAP_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Counter.h"
#include "Array.h"

namespace fire_engine
{

/** A map maps values from a certain domain onto values from another domain. */
template <class Key, class Value>
class _FIRE_ENGINE_API_ IMap : public Counter
{
	public:
		//! Destructor
		virtual ~IMap(void) {}

		//! Check whether the map contains a certain value
		virtual bool contains(const Key& key) const = 0;

		//! Insert a certain key-value pair
		virtual bool insert(const Key& key, const Value& val) = 0;

		//! Remove a key-value pair
		virtual bool remove(const Key& key) = 0;

		//! Find a value given it's key
		virtual Value * find(const Key& key) const = 0;

		//! Check whether the map is empty or not
		virtual bool isEmpty(void) const = 0;

		//! Get the set of keys
		virtual const Array<Key> * keys(void) const = 0;
		virtual Array<Key> * keys(void) = 0;

		//! Get all the values contained within the map
		virtual const Array<Value> * values(void) const = 0;
		virtual Array<Value> * values(void) = 0;

		/** Returns the size (ie. number of actual elements) in the IMap */
		virtual s32 size(void) const = 0;
};

} // namespace fire_engine

#endif // IMAP_H_INCLUDED

