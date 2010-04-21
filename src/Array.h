/**
 *  $Id: array.h 115 2007-10-05 10:52:32Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the Array class, for storing elements of different class.
 *  It offers methods like resizing, fast access...
**/

#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "counter.h"
#include "String.h"
#include <stdlib.h>
#include <string.h>

namespace fire_engine
{

/** A useful template for storing some type of Object. */
template<class T>
class _FIRE_ENGINE_API_ Array : public Counter
{
public:
	/** Construct an array.
	 \param size The initial size of the array.
	 \param grow The number to grow by, when the array needs resizing. */
	Array(s32 size = 128, s32 grow = 64)
		: Counter(0), mSize(size), mGrowBy(grow), mFreeWhenDestroyed(true)
	{
		mArray = new T[size];
	}

	/** Construct an array with an initial array of objects. */
	Array(T * objects, s32 size, s32 grow = 64)
		: Counter(size), mArray(objects), mSize(size), mGrowBy(grow), mFreeWhenDestroyed(true)
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
	inline void push_back(const T& elem)
	{
		insert(elem, this->getCount());
	}

	/** Insert an object at the start of the Array. */
	inline void push_front(const T& elem)
	{
		insert(elem, 0);
	}

	/** Returns whether the array contains a given object. */
	bool contains(const T& object) const
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
	inline T at(s32 index) const
	{
		return mArray[index];
	}

	/** Returns the element at position index in the array. */
	inline T& at(s32 index)
	{
		return mArray[index];
	}

	/** Removes a given element from the array.
	 \return true if the element was correctly removed, false otherwise. */
	bool removeElement(const T& elem)
	{
		s32 i, j;
		for (i = 0; i < this->getCount(); i++)
			if (elem == mArray[i])
				break;
		if (i == this->getCount())
			return false;
		for (j = i; j < this->getCount()-1; j++)
			memcpy((void*)&mArray[j], (const void*)&mArray[j+1], sizeof(T));
		this->decrementCount();
		return true;
	}

	/** Remove the elemtent at position index in the array. */
	bool remove(s32 index)
	{
		if (index >= this->getCount())
			return false;
		for (s32 i = index; i < mSize - 1; i++)
			memcpy((void*)&mArray[i], (const void*)&mArray[i+1], sizeof(T));
		this->decrementCount();
		return true;
	}

	/** Sort the Array, using some comparator function. */
	void sort(s32 (*compfunc)(const void * obj1, const void * obj2))
	{
		return qsort(mArray, this->getCount(), sizeof(T), compfunc);
	}

	/** Reverse the order of the elements in the Array. */
	void reverse()
	{
		T temp;
		for (s32 i = 0; i < this->getCount()/2; i++)
		{
			memcpy((void*)&temp, (const void*)&mArray[i], sizeof(T));
			memcpy((void*)&mArray[i], (const void*)&mArray[this->getCount()-1-i], sizeof(T));
			memcpy((void*)&mArray[this->getCount()-i-1], (const void*)&temp, sizeof(T));
		}
	}

	/** Resize the array. The new size must be larger than the current size.
	 \return true if the array was correctly resized, false otherwise. */
	bool resize(s32 newsize)
	{
		if (newsize < this->getCount())
			return false;
		T * tmp = new T[newsize];
		memcpy((void*)tmp, (const void*)mArray, this->getCount()*sizeof(T));
		if (mArray != 0)
			delete [] mArray;
		mSize = newsize;
		mArray = tmp;
		return true;
	}

	/** Sets the size of the Array. 
	 \param size The new size of the Array. */
	void setSize(s32 size)
	{
		if (mArray != nullptr)
		{
			delete [] mArray;
		}
		mArray = new T[size];
		mSize = size;
		mCount = 0;
	}

	/** Returns the number of elements currently stored in the array. */
	inline s32 size() const
	{
		return this->getCount();
	}

	/** Returns a const pointer to the elements in the array. */
	inline const T * const_pointer() const
	{
		return mArray;
	}

	/** Returns a pointer to the elements in the array. */
	inline T * pointer()
	{
		return mArray;
	}

	/** Set whether the elements should be freed when the Array is destroyed. Default is true.
	 \param fwd Whether the elements should be freed when Array is destroyed. */
	inline bool setFreeWhenDestroyed(bool fwd)
	{
		mFreeWhenDestroyed = fwd;
	}

	/** Access the element at position index in the array. */
	inline T& operator[](s32 index)
	{
		return mArray[index];
	}

	/** Access the element at position index in the array. */
	inline T operator[](s32 index) const
	{
		return mArray[index];
	}

private:
	T *   mArray;
	s32   mSize;
	s32   mGrowBy;
	bool  mFreeWhenDestroyed;

	/** Inserts an element at a specified position in the array. */
	void insert(const T& elem, s32 index)
	{
		if (this->getCount() == mSize)
			resize(mSize + mGrowBy);
		for (s32 i = this->getCount(); i > index; i--)
			memcpy((void*)&mArray[i], (const void*)&mArray[i-1], sizeof(T));
		mArray[index] = elem;
		this->incrementCount();
	}
};

} // namespace fire_engine

#endif // ARRAY_H_INCLUDED
