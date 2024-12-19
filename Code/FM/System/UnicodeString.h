#pragma once

#include <cstdio>
#include <cstdarg>
#include <algorithm>

#include "System\Container\Array.h"
#include "System\Memory.h"

namespace fm
{
	struct UnicodeStringWrapper final
	{
		const char* Data;		// bytes
		uint32_t ByteLength;
		uint32_t Length;
	};

	template < uint32 CAPACITY >
	class UnicodeString final
	{
	public:
		UnicodeString() = default;
		UnicodeString(const char* const value, const uint32 byteLength);
		UnicodeString(const char* const value);
		UnicodeString(const UnicodeStringWrapper& str);

		FORCEINLINE uint32 GetLength() const;			// number of characaters
		FORCEINLINE uint32 GetByteLength() const;		// number of bytes
		FORCEINLINE uint32 GetCapacity() const;

		void Append(const UnicodeStringWrapper& str);

		operator UnicodeStringWrapper() const;

	private:
		uint32 mNextIndex = 0;			// index of null char for easy to append
		uint32 mNumCharacters = 0;
		Array<char, CAPACITY> mData;

		void recalculateLength();
	};

	template < uint32 SIZE >
	UnicodeString< SIZE >::UnicodeString(const char* const value, const uint32 byteLength)
	{
		mNextIndex = byteLength;
		Assert(mNextIndex + 1 < SIZE, "assigned small size string");

		MemCpy(mData.Elements, SIZE, value, std::min(byteLength, SIZE));
		mData.Elements[mNextIndex] = 0;
		recalculateLength();
	}

	template < uint32 SIZE >
	UnicodeString< SIZE >::UnicodeString(const char* const value)
		: UnicodeString(value, (uint32)strlen(value))
	{
	}

	template < uint32 SIZE >
	UnicodeString< SIZE >::UnicodeString(const UnicodeStringWrapper& str)
	{
		mNextIndex = str.ByteLength;
		Assert(mNextIndex + 1 < SIZE, "assigned small size string");

		MemCpy(mData.Elements, SIZE, value, std::min(mNextIndex, SIZE));
		mData.Elements[mNextIndex] = 0;

		mNumCharacters = str.Length;
	}

	template < uint32 SIZE >
	uint32 UnicodeString< SIZE >::GetLength() const
	{
		return mNumCharacters;
	}

	template < uint32 SIZE >
	uint32 UnicodeString< SIZE >::GetByteLength() const
	{
		return mNextIndex;
	}

	template < uint32 SIZE >
	uint32 UnicodeString< SIZE >::GetCapacity() const
	{
		return SIZE;
	}

	template < uint32 SIZE >
	void UnicodeString< SIZE >::recalculateLength()
	{
		mNumCharacters = 0;
		const char* p = mData.Elements;
		const char* end = &mData.Elements[mNextIndex];

		while (p != end)
		{
			// https://en.wikipedia.org/wiki/UTF-8#Description
			if ((*p++ & 0xC0) != 0x80)
			{
				++mNumCharacters;
			}
		}
	}

	template < uint32 SIZE >
	UnicodeString< SIZE >::operator UnicodeStringWrapper() const
	{
		UnicodeStringWrapper str;
		str.Data = mData.Elements;
		str.ByteLength = mNextIndex;
		str.Length = mNumCharacters;

		return str;
	}

	template < uint32 SIZE >
	void UnicodeString< SIZE >::Append(const UnicodeStringWrapper& str)
	{
		Assert(str.ByteLength + mNextIndex < SIZE - 1, "exceeding UnicodeString buffer size");

		// TODO: handle overflow
		MemCpy(mData.Elements + mNextIndex, SIZE - mNextIndex - 1, str.Data, str.ByteLength);
		mNextIndex += str.ByteLength;
		mNumCharacters += str.Length;
		mData.Elements[mNextIndex] = 0;
	}
}
