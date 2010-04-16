/**
 * FILE:    list.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: list.h 115 2007-10-05 10:52:32Z jpaterso $$
 * PURPOSE: A simple template for a linked list
**/

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "counter.h"

namespace fire_engine
{

template <class T>
class _FIRE_ENGINE_API_ List : public Counter
{
private:
	/** Simple linked list entry, that keeps track of what goes before, and what
	 goes after it. */
	typedef struct _linked_list_entry_t
	{
		T object;
		_linked_list_entry_t * next;
		_linked_list_entry_t * prev;
	} linked_list_entry_t;

public:
	/** An iterator to explore all the objects in the list. */
	class iterator
	{
	public:
		virtual ~iterator()
		{
		}

		virtual iterator& operator++()
		{
			m_current = m_current->next;
			return *this;
		}

		virtual iterator operator++(s32)
		{
			iterator tmp = *this;
			m_current = m_current->next;
			return tmp;
		}

		virtual iterator& operator--()
		{
			m_current = m_current->prev;
			return *this;
		}

		virtual iterator operator--(s32)
		{
			iterator tmp = *this;
			m_current = m_current->prev;
			return tmp;
		}

		virtual T& operator*()
		{
			return m_current->object;
		}

		virtual T& operator->()
		{
			return m_current->object;
		}

		virtual T& operator()()
		{
			return m_current->object;
		}

		virtual bool operator!=(const iterator& it)
		{
			return m_current != it.m_current;
		}

		virtual bool operator==(const iterator& it)
		{
			return m_current == it.m_current;
		}

		virtual iterator& operator=(const iterator& other)
		{
			m_current = other.m_current;
			return *this;
		}
	private:
		// list<Object> needs to access the private constructor, so make it a friend!
		friend class List<T>;

		// The current position in the list
		linked_list_entry_t * m_current;

		/** Private constructor so that only list can create an iterator. */
		iterator(linked_list_entry_t * start) : m_current(start) {}
	};

	//! Default constructor
	List()
		: Counter(), m_head(0), m_tail(0)
	{
	}

	//! Destructor
	virtual ~List()
	{
		clear();
	}

	/** Add an element to the front of the list. */
	void push_front(const T& object)
	{
		linked_list_entry_t * new_entry = new linked_list_entry_t;
		new_entry->next = m_head;
		new_entry->prev = 0;
		new_entry->object = object;
		if (m_head)
			m_head->prev = new_entry;
		m_head = new_entry;
		if (m_tail == 0)
			m_tail = new_entry;
	}

	/** Add an element to the back of the list. */
	void push_back(const T& object)
	{
		if (m_head != 0)
		{
			linked_list_entry_t * new_entry = new linked_list_entry_t;
			new_entry->next = 0;
			new_entry->prev = m_tail;
			new_entry->object = object;
			m_tail->next = new_entry;
			m_tail = new_entry;
		}
		else
		{
			m_head = new linked_list_entry_t;
			m_head->next = 0;
			m_head->prev = 0;
			m_head->object = object;
			m_tail = m_head;
		}
		this->incrementCount();
	}

	//! Remove an object from the list, and return true if the removal was successful
	bool removeElement(const T& object)
	{
		linked_list_entry_t * cur = m_head;
		linked_list_entry_t * prev = 0;
		bool found = false;
		while (cur != 0)
		{
			if (cur->object == object)
			{
				found = true;
				break;
			}
			prev = cur;
			cur = cur->next;
		}
		if (!found)
			return false;
		else if (cur == m_head)
		{
			prev = m_head;
			if (m_tail == m_head)
				m_tail = m_head->next;
			m_head = m_head->next;
			if (m_head)
				m_head->prev = 0;
			delete prev;
		}
		else
		{
			prev->next = cur->next;
			if (cur->next)
				cur->next->prev = prev;
			if (cur == m_tail)
				m_tail = prev;
			delete cur;
		}
		this->decrementCount();
		return true;
	}

	//! Returns whether an object exists in the list
	bool contains(const T& object) const
	{
		linked_list_entry_t * it = m_head;
		while (it != 0)
		{
			if (it->object == object)
				return true;
			it = it->next;
		}
		return false;
	}

	//! Delete all the elements in the list
	void clear()
	{
		linked_list_entry_t * cur  = m_head;
		linked_list_entry_t * prev = 0;
		while (cur != 0)
		{
			prev = cur;
			cur = cur->next;
			delete prev;
		}
		m_head = m_tail = 0;
		this->resetCount();
	}

	/** Returns the size (number of elements contained) of the list */
	inline s32 size() const
	{
		return this->getCount();
	}

	inline iterator begin() const
	{
		return iterator(m_head);
	}

	iterator last() const
	{
		return iterator(m_tail);
	}

	inline iterator end() const
	{
		return iterator(0);
	}

private:
	linked_list_entry_t * m_head, * m_tail;
};

}

#endif // LIST_H_INCLUDED
