#pragma once

#include <type_traits>
#include "system/assert.h"

namespace fm
{
	template < typename T, uint32 SIZE >
	class Array final
	{
		template < typename T, uint32 S > friend class Stack;
		template < typename T, uint32 S > friend class Vector;
		template < typename T, uint32 S > friend class Queue;
	public:
		T Elements[SIZE];

		//-------------------------------------
		Array() = default;
		COPY_CONSTRUCTOR(Array) = delete;
		ASSIGN_OPERATOR(Array) = delete;

		FORCEINLINE uint32 GetLength() const;
		FORCEINLINE bool Contains(const T& value) const;
		int32 FindIndex(const T& value) const;

		T& operator[] (const int32 index);
		const T& operator[] (const int32 index) const;

		// ------------------------------------------------
		static_assert(SIZE > 0, "cannot make 0 length array");
	};

	//---
	// functions

	template < typename T, uint32 SIZE >
	uint32 Array< T, SIZE >::GetLength(void) const
	{
		return SIZE;
	}

	template < typename T, uint32 SIZE >
	bool Array< T, SIZE >::Contains(const T& value) const
	{
		return (FindIndex(value) != INVALID_INDEX);
	}

	template < typename T, uint32 SIZE >
	int32 Array< T, SIZE >::FindIndex(const T& value) const
	{
		for (int32 i = ToSigned(SIZE) - 1; i >= 0; --i)
		{
			if (Elements[i] == value)
			{
				return i;
			}
		}

		return INVALID_INDEX;
	}

	////--
	//// operators
	template < typename T, uint32 SIZE >
	T& Array< T, SIZE >::operator[] (const int32 index)
	{
		Assert(index < ToSigned(SIZE), "accessing out-of-index in array");

		return Elements[index];
	}

	template < typename T, uint32 SIZE >
	const T& Array< T, SIZE >::operator[] (const int32 index) const
	{
		Assert(index < ToSigned(SIZE), "accessing out-of-index in array");

		return Elements[index];
	}
}