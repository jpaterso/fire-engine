/**
 * FILE:    Object.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Object.h 115 2007-10-05 10:52:32Z jpaterso $
 * PURPOSE: An interface for some object in the fire engine project. It keeps
 *          track of things like reference count, etc...
**/

#ifndef IOBJECT_H_INCLUDED
#define IOBJECT_H_INCLUDED

#include "CompileConfig.h"
#include "Types.h"
#include "string.h"
#include "counter.h"
#include "array.h"
#include <stdio.h>

namespace fire_engine
{

/** The Object class is the base class of most Object's in the fire engine.
 It provides two main methods: grab() and drop(), whose main purpose is to
 prevent memory leaks, and trying to access deleted memory.
 Object keeps a reference counter, that counts the number of pointers or
 references to the Object, as long as grab() and drop() are used properly.
 The grab() method should be called whenever a new pointer/reference to a
 class deriving Object is made.
 drop() is to be used instead of delete, and it will take care of deleting
 the Object, if there are exactly zero references to it. */
class _FIRE_ENGINE_API_ Object : public virtual counter
{
public:
	/** Constructor: initialize the reference count, and set the debug name if
	 appropriate */
	Object()
#if defined(_FIRE_ENGINE_DEBUG_OBJECT)
		, m_debug_name("")
#endif
	{
		this->incrementCount();
	}

	/** Destructor */
	virtual ~Object()
	{
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
		printf("Deleting %s\n", m_debug_name.c_str());
#endif
	}

	/** Increase the reference count to the Object. A call to grab() should be
	 matched by a similar call to drop(), otherwise it could lead to memory leaks */
	inline void grab()
	{
		this->incrementCount();
	}

	/** Drop the object, ie. decrease the reference count. Deletes the object if
	 the reference count reaches 0. Similar to smart pointers!
	 \return true if the reference counter reached 0, and the Object was actually
	         destroyed, false otherwise */
	inline bool drop()
	{
		this->decrementCount();
		if (this->getCount() == 0)
		{
			delete this;
			return true;
		}
		return false;
	}

#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	/** Set the debug name for the Object */
	inline void setDebugName(string debug_name)
	{
		m_debug_name = debug_name;
	}

	/** Returns the name of the Object */
	inline const string& getDebugName() const
	{
		return m_debug_name;
	}
#endif

	bool operator==(Object* const& obj)
	{
		return this == obj;
	}

private:
#if defined(_FIRE_ENGINE_DEBUG_OBJECT_)
	string m_debug_name;
#endif
};

}

#endif // IOBJECT_H_INCLUDED
