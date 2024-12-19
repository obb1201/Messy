#pragma once

#include "Array.h"
#include "System/Memory.h"

namespace fm
{
	template < typename T, uint32 CAPACITY >
	class Queue final
	{
	public:
		Queue() = default;
		COPY_CONSTRUCTOR(Queue) = delete;	// TODO: should we really delete this? what about assignment?
		ASSIGN_OPERATOR(Queue) = delete;

		FORCEINLINE void Enqueue(const T& value);
		FORCEINLINE T Dequeue();
		FORCEINLINE void RemoveAll();

		FORCEINLINE uint32 GetSize() const;
		FORCEINLINE uint32 GetCapacity() const;

		FORCEINLINE const T& Peek() const;
		FORCEINLINE T& Peek();
		FORCEINLINE bool IsEmpty() const;

		FORCEINLINE bool Contains(const T& value) const;

	private:
		uint32 mReadIndex = 0;
		uint32 mSize = 0;
		Array< T, CAPACITY > mData;
	};

	template < typename T, uint32 SIZE >
	void Queue< T, SIZE >::Enqueue(const T& value)
	{
		Assert(mSize <= SIZE);
		mData[(mReadIndex + mSize++) % SIZE] = value;
	}

	template < typename T, uint32 SIZE >
	T Queue< T, SIZE >::Dequeue()
	{
		Verify(mSize--, "buffer empty");
		
		T ret = mData[mReadIndex];
		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			mData[mReadIndex] = T();
		}

		mReadIndex = (mReadIndex + 1) % SIZE;

		return ret;
	}

	template < typename T, uint32 SIZE >
	T& Queue< T, SIZE >::Peek()
	{
		Assert(mSize, "buffer empty");
		return mData[mReadIndex];
	}

	template < typename T, uint32 SIZE >
	const T& Queue< T, SIZE >::Peek() const
	{
		Assert(mSize, "buffer empty");
		return mData[mReadIndex];
	}

	template < typename T, uint32 SIZE >
	void Queue< T, SIZE >::RemoveAll()
	{
		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			while (!IsEmpty())
			{
				Dequeue();
			}
		}
		mReadIndex = 0;
		mSize = 0;
	}

	template < typename T, uint32 SIZE >
	uint32 Queue< T, SIZE >::GetSize() const
	{
		return mSize;
	}

	template < typename T, uint32 SIZE >
	uint32 Queue< T, SIZE >::GetCapacity() const
	{
		return SIZE;
	}

	template < typename T, uint32 SIZE >
	bool Queue< T, SIZE >::IsEmpty() const
	{
		return mSize == 0;
	}

	template < typename T, uint32 SIZE >
	bool Queue< T, SIZE >::Contains(const T& value) const
	{
		for (auto i = mReadIndex; i < mReadIndex + mSize; ++i)
		{
			if (mData[i%SIZE] == value)
			{
				return true;
			}
		}

		return false;
	}
}
