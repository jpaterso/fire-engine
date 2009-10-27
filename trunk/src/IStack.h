/**
 *  $Id: IStack.h 112 2007-09-25 23:37:26Z jpaterso $
 *
 *  This file defines an interface to a Stack class, an implementation of a LIFO
 *  queue.
 *  Items can be push()ed onto the stack, and pop()ed off it.
**/
#ifndef ISTACK_H_INCLUDED
#define ISTACK_H_INCLUDED

#include "CompileConfig.h"

namespace fire_engine
{
//! A basic interface for a Stack class, must implement push() and pop()
template<class Obj>
class _FIRE_ENGINE_API_ IStack
{
protected:
	struct _item
	{
		Obj item;
		struct _item * next;
	};

public:
	virtual ~IStack(void)
	{
		struct _item * ptr;
		struct _item * ptr2;
		if (m_root != 0)
		{
			ptr = m_root;
			while (ptr != 0)
			{
				ptr2 = ptr->next;
				delete ptr;
				ptr = ptr2;
			}
			m_root = 0;
		}
	}

	virtual void push(Obj item)  = 0;
	virtual Obj pop(void)        = 0;

protected:
	struct _item * m_root;
};
}


#endif // ISTACK_H_INCLUDED
