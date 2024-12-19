#pragma once

#include "Array.h"
#include "System/Memory.h"

namespace fm
{
	template < typename T, uint32 CAPACITY >
	class Stack final
	{
	public:
		Stack() = default;
		COPY_CONSTRUCTOR(Stack) = delete;	// TODO: should we really delete this? what about assignment?
		ASSIGN_OPERATOR(Stack) = delete;

		FORCEINLINE void Push(const T& value);
		FORCEINLINE T Pop();

		FORCEINLINE void RemoveAll();

		FORCEINLINE uint32 GetSize() const;
		FORCEINLINE uint32 GetCapacity() const;

		FORCEINLINE bool IsEmpty() const;


		FORCEINLINE bool Contains(const T& value) const;
		FORCEINLINE const T& Peek(const uint32 offset = 0) const;
		FORCEINLINE T& Peek(const uint32 offset = 0);

	private:
		uint32 mNextIndex = 0;
		Array< T, CAPACITY > mData;

		static_assert(CAPACITY > 0, "cannot make Vector with 0 size");
	};

	template < typename T, uint32 SIZE >
	void Stack< T, SIZE >::Push(const T& value)
	{
		Assert(mNextIndex < SIZE, "Vector out of size");

		mData[mNextIndex++] = value;
	}

	template < typename T, uint32 SIZE >
	T Stack< T, SIZE >::Pop()
	{
		Assert(mNextIndex > 0, "can't pop from empty stack");

		T ret = mData[--mNextIndex];
		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			mData[mNextIndex] = T();
		}
		
		return ret;
		
	}

	template < typename T, uint32 SIZE >
	T& Stack< T, SIZE >::Peek(const uint32 offset)
	{
		Assert((ToSigned(mNextIndex) - 1) >= ToSigned(offset), "offset exceeds stack size");
		return mData[mNextIndex - 1- offset];
	}

	template < typename T, uint32 SIZE >
	const T& Stack< T, SIZE >::Peek(const uint32 offset) const
	{
		Assert(mNextIndex > 0, "can't peek from empty stack");
		Assert((mNextIndex - 1) >= offset, "offset exceeds stack size");
		
		return mData[mNextIndex - 1 - offset];
	}

	template < typename T, uint32 SIZE >
	void Stack< T, SIZE >::RemoveAll()
	{
		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			while (!IsEmpty())
			{
				Pop();
			}
		}
		mNextIndex = 0;
	}

	template < typename T, uint32 SIZE >
	uint32 Stack< T, SIZE >::GetSize() const
	{
		return mNextIndex;
	}

	template < typename T, uint32 SIZE >
	uint32 Stack< T, SIZE >::GetCapacity() const
	{
		return SIZE;
	}

	template < typename T, uint32 SIZE >
	bool Stack< T, SIZE >::IsEmpty() const
	{
		return mNextIndex == 0;
	}

	template < typename T, uint32 SIZE >
	bool Stack< T, SIZE >::Contains(const T& value) const
	{
		for (int i = toSigned(mNextIndex) - 1; i >= 0; --i)
		{
			if (mData[i] == value)
			{
				return true;
			}
		}

		return false;
	}
}