#pragma once

#include "Array.h"
#include "System/Memory.h"

namespace fm
{
	template < typename T, uint32 CAPACITY >
	class Vector final
	{
	public:
		template <typename, uint32, uint32> friend class HashMap;

		Vector() = default;
		COPY_CONSTRUCTOR(Vector) = delete;
		ASSIGN_OPERATOR(Vector) = delete;

		FORCEINLINE uint32 GetSize() const;
		FORCEINLINE uint32 GetCapacity() const;

		FORCEINLINE void Add(const T& value);

		// NOTE: unordered versions are faster
		FORCEINLINE void RemoveSwap(const T& value); // unordered remove
		FORCEINLINE void Remove(const T& value); // remove and shift
		FORCEINLINE void RemoveAll();
		FORCEINLINE void RemoveAtSwap(const uint32 index); // unordered remove
		FORCEINLINE void RemovedAt(const uint32 index); // remove and shift

		FORCEINLINE bool Contains(const T& value) const;
		FORCEINLINE int32 FindIndex(const T& value) const;

		const T& operator [] (const int index) const;
		T& operator [] (const int index);

	private:
		uint32 mNextIndex = 0;
		Array< T, CAPACITY > mData;

		//----------------------------------
		static_assert(CAPACITY > 0, "cannot make Vector with 0 size");
	};

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::Add(const T& value)
	{
		Assert(mNextIndex < SIZE, "Vector out of size");

		mData[mNextIndex++] = value;
	}

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::RemoveSwap(const T& value)
	{
		const int32 index = FindIndex(value);
		Assert(index != INVALID_INDEX, "cannot find an item in Array");

		RemoveAtSwap(ToUnsigned(index));
	}

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::RemoveAtSwap(const uint32 index)
	{
		Assert(mNextIndex > 0, "deleting from empty array");
		Assert(index < mNextIndex, "removing from empty slot");
		Assert(index < SIZE, "out of index");

		// move last element to the removed spot

		if (index != --mNextIndex)
		{
			mData[index] = mData[mNextIndex];
		}

		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			mData[mNextIndex] = T();
		}
	}

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::Remove(const T& value)
	{
		const int32 index = FindIndex(value);
		Assert(index != INVALID_INDEX, "cannot find an item in Array");

		RemovedAt(ToUnsigned(index));
	}

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::RemovedAt(const uint32 index)
	{
		Assert(mNextIndex > 0, "deleting from empty array");
		Assert(index < mNextIndex, "removing from empty slot");
		Assert(index < SIZE, "out of index");

		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			mData[index] = T();
		}

		// shift
		if (index != --mNextIndex)
		{
			MemMove(&mData[index], &mData[index + 1], sizeof(T) * (mNextIndex - index));
		}

		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			MemSet(&mData[mNextIndex], sizeof(T), 0);
		}
	}

	template < typename T, uint32 SIZE >
	void Vector< T, SIZE >::RemoveAll()
	{
		if (!std::is_scalar<T>::value && !std::is_pod<T>::value)
		{
			for (uint32 i = 0; i < mNextIndex; ++i)
			{
				mData[i] = T();
			}
		}
		mNextIndex = 0;
	}

	template < typename T, uint32 SIZE >
	uint32 Vector< T, SIZE >::GetSize() const
	{
		return mNextIndex;
	}

	template < typename T, uint32 SIZE >
	uint32 Vector< T, SIZE >::GetCapacity() const
	{
		return SIZE;
	}

	template < typename T, uint32 SIZE >
	bool Vector< T, SIZE >::Contains(const T& value) const
	{
		return (FindIndexRef(value) != INVALID_INDEX);
	}

	template < typename T, uint32 SIZE >
	int32 Vector< T, SIZE >::FindIndex(const T& value) const
	{
		for (int i = ToSigned(mNextIndex) - 1; i >= 0; --i)
		{
			if (mData[i] == value)
			{
				return i;
			}
		}

		return INVALID_INDEX;
	}

	template < typename T, uint32 SIZE >
	T& Vector< T, SIZE >::operator[] (const int index)
	{
		Assert(index < ToSigned(mNextIndex), "accessing out-of-index in array");
		Assert(index < ToSigned(SIZE), "accessing out-of-index in array");

		return mData[index];
	}

	template < typename T, uint32 SIZE >
	const T& Vector< T, SIZE >::operator[] (const int index) const
	{
		Assert(index < ToSigned(mNextIndex), "accessing out-of-index in array");
		Assert(index < ToSigned(SIZE), "accessing out-of-index in array");

		return mData[index];
	}
}
