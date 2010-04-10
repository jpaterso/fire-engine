/**
 *  $Id: Stack.h 113 2007-10-02 00:45:44Z jpaterso $
 *
 *  This file defines the Stack class, an implementation of a LIFO queue.
 *  Items can be push()ed onto the stack, and pop()ed off it.
**/
#ifndef __STACK_H
#define __STACK_H

#include "IStack.h"
#include "CompileConfig.h"
#include "counter.h"

namespace fire_engine
{
template <class Obj>
class _FIRE_ENGINE_API_ Stack : public IStack<Obj>, public counter
{
public:
	Stack() : counter()
	{
		this->m_root = 0;
	}

	virtual void push(Obj item)
	{
		struct _item * entry = new struct _item;
		entry->item = item;
		this->incrementCount();
		if (this->m_root != 0)
			entry->next = this->m_root;
		else
			entry->next = 0;
		this->m_root = entry;
	}

	virtual Obj pop(void)
	{
		struct _item * ptr = this->m_root;
		Obj item = this->m_root->item;
		this->decrementCount();
		this->m_root = this->m_root->next;
		delete ptr;
		return item;
	}

	inline s32 size() const
	{
		return this->getCount();
	}

	virtual const Obj& peekTop() const
	{
		return this->m_root->item;
	}
};
}


#endif
