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
		just frees all of the memory in the array pointer
	*/
	~Allocator()
	{
		DeleteAllObjectsAndBuffer();
	}

	/*
		insert the templated object in the array
	*/
	__forceinline void Insert(T* _obj_insert)
	{
		assert(!(_curr_size > (_mem_size / sizeof(T))) && "Trying to add more elements but the buffer array is already full\n");
		assert(_mem_block != nullptr && "Trying to add an object in a NULL buffer.\n");

		_mem_block[_curr_size] = _obj_insert;
		_curr_size = _curr_size + 1;
#ifdef _DEBUG
		std::cout << "curr size is now " << _curr_size << std::endl;
#endif
	}

	/*
		Just deletes everything =)
	*/

	void DeleteAllObjects()
	{
		for (size_t i = 0; i < _mem_size / sizeof(T); i++)
		{
			free(_mem_block[i]);
		}
		_curr_size = 0;
	}

	/*
		Create a new buffer and returns true if 
		creation was successful
	*/

	bool CreateBufferWithSize(size_t buffer_size)
	{
		//assert(_mem_block == nullptr && "Trying to create a buffer, but a buffer already exists.");
	
		if (_mem_block == nullptr)
		{
			_mem_block = (T**)malloc(buffer_size);
			_curr_size = 0;
			_mem_size = buffer_size;
		}

		return _mem_block != nullptr;
	}

	/*
		WARNING: This will delete all the objects and the buffer!!!
		if you want to continue to use this class, make sure you call
		CreateBufferWithSize()
	*/

	void DeleteAllObjectsAndBuffer()
	{
		for (size_t i = 0; i < _mem_size / sizeof(T); i++)
		{
			free(_mem_block[i]);
		}
		free(_mem_block);
		_mem_block = nullptr;
		_curr_size = 0;
		_mem_size = 0;
	}

	/*
		This function takes care to delete
		whatever was in the first slot and add
		a new object
	*/
	void force_insert_at_beginning(T* _obj)
	{
		free(_mem_block[0]);
		_mem_block[0] = nullptr;
		_mem_block[0] = _obj;
	}

public:

	/* Overloads for CreateObject() function */
	
	template<class A1>
	__forceinline void CreateObject(T*(_func_arg1)(A1), A1 arg)
	{
		Insert(_func_arg1(arg));
	}

	template<class A1, class A2>
	__forceinline void CreateObject(T*(_func_arg2)(A1, A2), A1 arg1, A2 arg2)
	{
		Insert(_func_arg2(arg1, arg2));
	}

	template<class A1, class A2, class A3>
	__forceinline void CreateObject(T* (_func_arg3)(A1, A2, A3), A1 arg1, A2 arg2, A3 arg3)
	{
		Insert(_func_arg3(arg1, arg2, arg3));
	}

	template<class A1, class A2, class A3, class A4>
	__forceinline void CreateObject(T* (_func_arg4)(A1, A2, A3, A4), A1 arg1, A2 arg2, A3 arg3, A4 arg4)
	{
		Insert(_func_arg4(arg1, arg2, arg3, arg4));
	}

public:
	/*
		Just get the pointer at some index in case you
		want to...
	*/
	__forceinline T* GetPointerAtIndex(size_t index) const
	{
		return _mem_block[index];
	}

	/*
		Rawrrr(sorry) pointer so you can have full access
		to your data. Potentially easy to break the whole 
		code with this.
	*/
	__forceinline T** GetRawPointer() const
	{
		return _mem_block;
	}

	/*
		the name already tell
	*/
	__forceinline T* GetLastObjectAdded() const
	{
		return _mem_block[_curr_size - 1];
	}

	/*
		Might be userful huh
	*/
	__forceinline const size_t GetBufferSize() const
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

