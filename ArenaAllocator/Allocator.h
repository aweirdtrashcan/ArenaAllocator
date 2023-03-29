#pragma once

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cassert>

template<class T>
class Allocator
{
public:
	/* 
		deleting what i believe are the copy/move constructors
		don't remember quite well :p 
	 */

	Allocator& operator=(Allocator&) = delete;
	Allocator& operator=(Allocator&&) = delete;
	
	/*
		constructor with buffer_size as args so i can construct 
		the array of pointers with malloc
	*/
	explicit Allocator(size_t buffer_size)
		:
		_mem_size(buffer_size),
		_curr_size(0),
		_mem_block((T**)malloc(buffer_size))
	{
#ifdef _DEBUG
		std::cout << "allocated " << _mem_size << " bytes\n";
#endif
	}

	/* 
		calling the destructor (and begging you actually got a destructor)
		and then setting the pointer to nullptr just to make sure it's gone
		(((i have no ideia if this is a memory leak or not)))
	*/
	~Allocator()
	{
		for (size_t i = 0; i < _mem_size / sizeof(T); i++)
		{
			free(_mem_block[i]);
		}
	}

	/*
		insert the templated object in the array
		if the array is already full, get back to index 0
		destroy what's in there and place the new object at location
	*/
	void Insert(T* _obj_insert)
	{
		assert(!(_curr_size > (_mem_size / sizeof(T))) && "Trying to add more elements but the buffer array is already full\n");

		_mem_block[_curr_size] = _obj_insert;
		_curr_size = _curr_size + 1;
#ifdef _DEBUG
		std::cout << "curr size is now " << _curr_size << std::endl;
#endif
	}

	void InsertAtLocation(T* _obj_insert, size_t index)
	{
		if (_mem_block[index] != nullptr)
		{
			_mem_block[index]->~T();
			_mem_block[index] = nullptr;
			_mem_block[index] = _obj_insert;
		}
	}

	void DeleteAllObjects()
	{
		for (size_t i = 0; i < _mem_size / sizeof(T); i++)
		{
			free(_mem_block[i]);
		}
	}

	/*
		created an object just for testing...
		have no idea how should one pass custom args
		==cries in dumb c++==
	*/
	void CreateObject()
	{
		T* _new_object = new T(30.f, 12.f, 14.f, 12.f);
		Insert(_new_object);
	}

	/*
		This function takes care to delete
		whatever was in the first slot and add
		a new object
	*/
	void force_insert_at_beginning(T* _obj)
	{
		_mem_block[0]->~T();
		_mem_block[0] = nullptr;
		_mem_block[0] = _obj;
	}

public:
	/*
		Just get the pointer at some index in case you
		want to...
	*/
	inline T* GetPointerAtIndex(size_t index) const
	{
		return _mem_block[index];
	}

	/*
		Rawrrr(sorry) pointer so you can have full access
		to your data. Potentially easy to break the whole 
		code with this.
	*/
	inline T** GetRawPointer() const
	{
		return _mem_block;
	}

	/*
		the name already tell
	*/
	inline T* GetLastObjectAdded() const
	{
		return _mem_block[_curr_size - 1];
	}

	/*
		Might be userful huh
	*/
	inline const size_t GetBufferSize() const
	{
		return _mem_size;
	}

private:

	/* === ABANDONED IDEA === */

//	/*
//		I've created this function just in case
//		the user overflows the array, the class can still
//		be used.
//
//		It will start counting again the num of elements in the
//		array and will eventually delete old objects to give
//		space to newer ones
//	*/
//	void INTERNAL_force_insert_at_location(T* _obj, size_t index, bool _has_overflow)
//	{
//		if (_has_overflow)
//		{
//#ifdef _DEBUG
//			std::cout << "Overflow algorithm triggered\n";
//#endif
//			size_t _max_objects_capacity = _mem_size / sizeof(T);
//			/* 
//				capacity discrepancy between max capacity
//				and the current amount of objects in the array.
//				substract 1 because once the overflow is triggered,
//				the difference will result in "1". i want index 0.
//			*/
//			size_t _overflow_difference = (_curr_size - _max_objects_capacity) - 1;
//			_mem_block[index]->~T();
//			_mem_block[index] = nullptr;
//			_mem_block[index] = _obj;
//			return;
//		}
//
//		_mem_block[index]->~T();
//		_mem_block[index] = nullptr;
//		_mem_block[index] = _obj;
//	}

private:
	size_t _mem_size;
	size_t _curr_size;
	T** _mem_block;
};

