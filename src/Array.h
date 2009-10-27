/**
 *  $Id: Array.h 115 2007-10-05 10:52:32Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the Array class, for storing elements of different class.
 *  It offers methods like resizing, fast access...
**/

#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "Counter.h"
#include "string.h"
#include <stdlib.h>
#include <string.h>

namespace fire_engine
{

/** A useful template for storing some type of Object. */
template<class Obj>
class _FIRE_ENGINE_API_ Array : public Counter
{
public:
	/** Construct an array.
	 \param size The initial size of the array.
	 \param grow The number to grow by, when the array needs resizing. */
	Array(s32 size = 128, s32 grow = 64)
		: Counter(), mSize(size), mGrowBy(grow), mFreeWhenDestroyed(true)
	{
		mArray = new Obj[size];
	}

	/** Construct an array with an initial array of objects. */
	Array(Obj * objects, s32 size, s32 grow = 64)
		: Counter(), mArray(objects), mSize(size), mGrowBy(grow), mFreeWhenDestroyed(true)
	{
	}

	/** Destructor. */
	~Array()
	{
		if (mFreeWhenDestroyed)
		{
			delete [] mArray;
		}
	}

	/** Insert an object to the back of the Array. */
	inline void push_back(const Obj& elem)
	{
		insert(elem, this->getCount());
	}

	/** Insert an object at the start of the Array. */
	inline void push_front(const Obj& elem)
	{
		insert(elem, 0);
	}

	/** Returns whether the array contains a given object. */
	bool contains(const Obj& object) const
	{
		for (s32 i = 0; i < this->getCount(); i++)
		{
			if (mArray[i] == object)
			{
				return true;
			}
		}
		return false;
	}

	/** Clear all the objects in the Array. They will not be deleted however
	 until the Array itself is destroyed */
	inline void clear()
	{
		this->resetCount();
	}

	/** Returns the element at position index in the array. */
	inline Obj at(s32 index) const
	{
		return mArray[index];
	}

	/** Returns the element at position index in the array. */
	inline Obj& at(s32 index)
	{
		return mArray[index];
	}

	/** Removes a given element from the array.
	 \return true if the element was correctly removed, false otherwise. */
	bool removeElement(const Obj& elem)
	{
		s32 i, j;
		for (i = 0; i < this->getCount(); i++)
			if (elem == mArray[i])
				break;
		if (i == this->getCount())
			return false;
		for (j = i; j < this->getCount()-1; j++)
			memcpy((void*)&mArray[j], (const void*)&mArray[j+1], sizeof(Obj));
		this->decrementCount();
		return true;
	}

	/** Remove the elemtent at position index in the array. */
	bool remove(s32 index)
	{
		if (index >= this->getCount())
			return false;
		for (s32 i = index; i < mSize - 1; i++)
			memcpy((void*)&mArray[i], (const void*)&mArray[i+1], sizeof(Obj));
		this->decrementCount();
		return true;
	}

	/** Sort the Array, using some comparator function. */
	void sort(s32 (*compfunc)(const void * obj1, const void * obj2))
	{
		return qsort(mArray, this->getCount(), sizeof(Obj), compfunc);
	}

	/** Reverse the order of the elements in the Array. */
	void reverse()
	{
		Obj temp;
		for (s32 i = 0; i < this->getCount()/2; i++)
		{
			memcpy((void*)&temp, (const void*)&mArray[i], sizeof(Obj));
			memcpy((void*)&mArray[i], (const void*)&mArray[this->getCount()-1-i], sizeof(Obj));
			memcpy((void*)&mArray[this->getCount()-i-1], (const void*)&temp, sizeof(Obj));
		}
	}

	/** Resize the array. The new size must be larger than the current size.
	 \return true if the array was correctly resized, false otherwise. */
	bool resize(s32 newsize)
	{
		if (newsize < this->getCount())
			return false;
		Obj * tmp = new Obj[newsize];
		memcpy((void*)tmp, (const void*)mArray, this->getCount()*sizeof(Obj));
		if (mArray != 0)
			delete [] mArray;
		mSize = newsize;
		mArray = tmp;
		return true;
	}

	/** Returns the number of elements currently stored in the array. */
	inline s32 size() const
	{
		return this->getCount();
	}

	/** Returns a const pointer to the elements in the array. */
	inline const Obj * const_pointer() const
	{
		return mArray;
	}

	/** Returns a pointer to the elements in the array. */
	inline Obj * pointer()
	{
		return mArray;
	}

	/** Access the element at position index in the array. */
	inline Obj& operator[](s32 index)
	{
		return mArray[index];
	}

	/** Access the element at position index in the array. */
	inline Obj operator[](s32 index) const
	{
		return mArray[index];
	}

private:
	Obj * mArray;
	s32   mSize;
	s32   mGrowBy;
	bool  mFreeWhenDestroyed;

	/** Inserts an element at a specified position in the array. */
	void insert(const Obj& elem, s32 index)
	{
		if (this->getCount() == mSize)
			resize(mSize + mGrowBy);
		for (s32 i = this->getCount(); i > index; i--)
			memcpy((void*)&mArray[i], (const void*)&mArray[i-1], sizeof(Obj));
		mArray[index] = elem;
		this->incrementCount();
	}
};

} // namespace fire_engine

#endif // ARRAY_H_INCLUDED
