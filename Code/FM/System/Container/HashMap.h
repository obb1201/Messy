#pragma once

#include "Vector.h"
#include "system/hash.h"

namespace fm
{
	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE = 4 >
	class HashMap final
	{
	public:
		HashMap() = default;
		COPY_CONSTRUCTOR(HashMap) = delete;

		FORCEINLINE bool TryGet(const hashid key, T& value);
		FORCEINLINE T Remove(const hashid key);
		FORCEINLINE void RemoveAll();

		FORCEINLINE bool Contains(const hashid key) const;
		FORCEINLINE uint32 GetSize() const;
		FORCEINLINE void Put(const hashid key, const T& val);

		FORCEINLINE const T& Get(const hashid key) const;
		FORCEINLINE T& Get(const hashid key);

		FORCEINLINE void GetValues(Vector<T, CAPACITY>& list);

	private:
		// TODO: maybe we should implement it as ring buffer?
		Vector< hashid, BUCKET_SIZE > mKeys[CAPACITY];
		Vector< T, BUCKET_SIZE > mValues[CAPACITY];
		uint32 mSize = 0;

		bool FndIndices(const hashid key, uint32* const index, int32* const bucketIndex) const;

		static_assert(CAPACITY > 0, "cannot make HahsMap with 0 size");
		static_assert(BUCKET_SIZE > 0, "cannot make HahsMap with 0 bucket size");
	};

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	bool HashMap< T, CAPACITY, BUCKET_SIZE>::FndIndices(const hashid key, uint32* const index, int32* const bucketIndex) const
	{
		const uint32 tmpIndex = key % CAPACITY;
		const uint32 tmpBucketIndex = mKeys[tmpIndex].FindIndex(key);

		*index = tmpIndex;
		*bucketIndex = tmpBucketIndex;

		return (tmpBucketIndex != INVALID_INDEX);
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	bool HashMap< T, CAPACITY, BUCKET_SIZE >::Contains(const hashid key) const
	{
		uint32 index;
		int32 bucketIndex;
		return FndIndices(key, &index, &bucketIndex);
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	const T& HashMap< T, CAPACITY, BUCKET_SIZE >::Get(const hashid key) const
	{
		uint32 index;
		int32 bucketIndex;
		Verify(FndIndices(key, &index, &bucketIndex), "cannot find item in hashmap");

		return mValues[index][bucketIndex];
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	T& HashMap< T, CAPACITY, BUCKET_SIZE >::Get(const hashid key)
	{
		uint32 index;
		int32 bucketIndex;
		Verify(FndIndices(key, &index, &bucketIndex), "cannot find item in hashmap");

		return mValues[index][bucketIndex];
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	bool HashMap< T, CAPACITY, BUCKET_SIZE >::TryGet(const hashid key, T& val)
	{
		uint32 index;
		int32 bucketIndex;
		const bool result = FndIndices(key, &index, &bucketIndex);
		if (result)
		{
			val = mValues[index][bucketIndex];
		}

		return result;
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	void HashMap< T, CAPACITY, BUCKET_SIZE >::Put(const hashid key, const T& val)
	{
		uint32 index;
		int32 bucketIndex;
		if (FndIndices(key, &index, &bucketIndex))
		{
			// TODO : should allow to change value when a key is duplication?
			mValues[index][bucketIndex] = val;
		}
		else
		{
			Assert(mSize < CAPACITY, "out of capacity");

			mKeys[index].Add(key);
			mValues[index].Add(val);

			++mSize;
		}
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	T HashMap< T, CAPACITY, BUCKET_SIZE >::Remove(const hashid key)
	{
		uint32 index;
		int32 bucketIndex;
		const bool result = FndIndices(key, &index, &bucketIndex);
		Assert(result, "cannot find item in hashmap");

		T ret = mValues[index][bucketIndex];

		mKeys[index].RemoveAtSwap(bucketIndex);
		mValues[index].RemoveAtSwap(bucketIndex);
		--mSize;

		return ret;
	}

	template < typename T, uint32 SIZE, uint32 BUCKET_SIZE >
	void HashMap< T, SIZE, BUCKET_SIZE >::RemoveAll()
	{
		for (auto& k : mKeys)
		{
			k.RemoveAll();
		}

		for (auto& v : mValues)
		{
			v.RemoveAll();
		}

		mSize = 0;
	}

	template < typename T, uint32 CAPACITY, uint32 BUCKET_SIZE >
	FORCEINLINE void HashMap< T, CAPACITY, BUCKET_SIZE >::GetValues(Vector<T, CAPACITY>& list)
	{
		uint32 numAdded = 0u;
		for (const auto& values : mValues)
		{
			const auto count = values.GetSize();

			for (auto i = 0u; i < count; ++i)
			{
				list.Add(values[i]);
			}
		}
	}

	template < typename T, uint32 SIZE, uint32 BUCKET_SIZE >
	uint32 HashMap< T, SIZE, BUCKET_SIZE >::GetSize() const
	{
		return mSize;
	}
}
