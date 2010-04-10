/**
 * FILE:    HashTable.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: HashTable.h 116 2007-10-05 13:22:45Z jpaterso $
 * PURPOSE: An implementation of a Hash table, which stores key-pair values using a hash on the
 *          key. A 'load' factor determines the values/storage factor, and when it reaches a
 *          certain threshold (set at 0.75f by default), the table is re-hashed. A HashTable
 *          with a well-set load factor provides very good insertion/searching times, close to
 *          O(1).
**/

#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "IMap.h"
#include "CompileConfig.h"
#include "Hash.h"
#include <string.h>

namespace fire_engine
{

/** An implementation of a Hash table, which stores key-pair values using a hash on the
 key. A 'load' factor determines the values/storage factor, and when it reaches a
 certain threshold (set at 0.75f by default), the table is re-hashed. A HashTable
 with a well-set load factor provides very good insertion/searching times, close to
 O(1). */
template <class Key, class Value, class HashClass = Hash<Key>, class CompareClass = Hash<Key> >
class _FIRE_ENGINE_API_ HashTable : public IMap<Key, Value>
{
private:
	//! An entry in the HashTable
	struct _HTEntry
	{
		Key   key;
		Value value;

		//! Constructor
		_HTEntry(Key k, Value v)
			: key(k), value(v)
		{
		}
	};

public:
	/** Constructor. */
	HashTable(u32 initial_capacity = 128, f32 load = 0.75f)
		: m_table_size(initial_capacity),
		m_max_load(load),
		m_current_load(0.0f)
	{
		m_table = new struct _HTEntry*[m_table_size];
		memset((void *)m_table, 0, m_table_size * sizeof(struct _HTEntry *));
	}

	/** Destructor. */
	virtual ~HashTable()
	{
		for (u32 i = 0; i < m_table_size; i++)
			if (m_table[i] != 0)
				delete m_table[i];
		delete [] m_table;
	}

	/** Returns whether the hash table contains a specified key. */
	virtual bool contains(const Key& key) const
	{
		u32 h = hash(key, m_table_size);
		while (m_table[h] != 0)
		{
			if (CompareClass::equal(m_table[h]->key, key))
				return true;
			h = (h+1)%(m_table_size-1);
		}
		return false;
	}

	/** Inserts a key-value pair in the hash table.
	 \return Whether the insertion was successful. It will fail if the key-value pair
	         already exists. */
	virtual bool insert(const Key& key, const Value& val)
	{
		u32 h = hash(key, m_table_size);
		this->incrementCount();
		m_current_load = (f32)this->getCount() / m_table_size;
		if (m_current_load > m_max_load)
			re_hash();
		while (m_table[h] != 0)
		{
			if (CompareClass::equal(m_table[h]->key, key))
				return false;
			h = (h+1)%(m_table_size-1);
		}
		m_table[h] = new _HTEntry(key, val);
		return true;
	}

	/** Removes a specified key from the hash table, and returns whether the removal was
	 successful or not. */
	virtual bool remove(const Key& key)
	{
		u32 h = hash(key, m_table_size);
		while (m_table[h] != 0)
		{
			if (CompareClass::equal(m_table[h]->key, key))
			{
				delete m_table[h];
				m_table[h] = 0;
				this->decrementCount();
				m_current_load = (f32)this->getCount() / m_table_size;
				return true;
			}
			h = (h == m_table_size - 1) ? 0 : (h + 1);
		}
		return false;
	}

	/** Attempts to find the value associated with a given key.
	 \return A pointer to the key, or 0 the key was not in the table. */
	virtual Value * find(const Key& key) const
	{
		u32 h = hash(key, m_table_size);
		while (m_table[h] != 0)
		{
			if (CompareClass::equal(m_table[h]->key, key))
				return &(m_table[h]->value);
			h = (h+1) % (m_table_size-1);
		}
		return 0;
	}

	/** Returns whether the hash table is empty. */
	virtual bool isEmpty() const
	{
		return this->getCount() == 0;
	}

	/** Returns an array containing all the keys of the hash table. */
	virtual const array<Key> * keys() const
	{
		array<Key> * k = new array<Key>(this->getCount());
		for (u32 i = 0; i < m_table_size; i++)
			if (m_table[i] != 0)
				k->push_back(m_table[i]->key);
		return k;
	}

	virtual array<Key> * keys()
	{
		array<Key> * k = new array<Key>(this->getCount());
		for (u32 i = 0; i < m_table_size; i++)
			if (m_table[i] != 0)
				k->push_back(m_table[i]->key);
		return k;
	}

	/** Returns an array containing all the values of the hash table. */
	virtual const array<Value> * values() const
	{
		array<Value> * v = new array<Value>(this->getCount());
		for (u32 i = 0; i < m_table_size; i++)
			if (m_table[i] != 0)
				v->push_back(m_table[i]->value);
		return v;
	}

	virtual array<Value> * values()
	{
		array<Value> * v = new array<Value>(this->getCount());
		for (u32 i = 0; i < m_table_size; i++)
			if (m_table[i] != 0)
				v->push_back(m_table[i]->value);
		return v;
	}

	/** Returns the number of key-value pairs in the hash-table. */
	virtual inline s32 size() const
	{
		return this->getCount();
	}

private:
	struct _HTEntry ** m_table;
	u32                m_table_size;
	f32                m_max_load;
	f32                m_current_load;

	//! Hash function - implements the Jenkin-one-at-a-time JOAAT algorithm
	static u32 hash(const Key& key, s32 tbl_size)
	{
		return HashClass::hash(key) % tbl_size;
	}

	//! Re-hash the table
	void re_hash()
	{
		struct _HTEntry ** new_table = new struct _HTEntry*[2 * m_table_size];
		memset((void *)new_table, 0, 2*m_table_size*sizeof(struct _HTEntry)); // set them to 0 just in case
		s32 last = 2*m_table_size-1;
		u32 h;
		for (u32 i = 0; i < m_table_size; i++)
		{
			if (m_table[i] != 0)
			{
				h = hash(m_table[i]->key, m_table_size);
				while (new_table[h] != 0)
					h = (h+1)%(last);
				new_table[i] = m_table[i];
			}
		}
		delete [] m_table;
		m_table        = new_table;
		m_table_size   = last+1;
		m_current_load = (f32)this->getCount() / m_table_size;
	}
};

}

#endif // HASHTABLE_H_INCLUDED
