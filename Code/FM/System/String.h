#pragma once

#include <algorithm>
#include <cstdio>
#include <cstdarg>


#include "System\Container\Array.h"
#include "System\Memory.h"
#include "System\UnicodeString.h"
#include "Math\MathUtility.h"

namespace fm
{
	struct StringWrapper final
	{
		const char* Data;
		uint32_t Length;
	};

	template < uint32 CAPACITY >
	class String final
	{
	public:
		String() = default;
		String(const String& rhs);
		String(const char* const value, const uint32 length);
		String(const char* const value);
		String(const StringWrapper& str);

		FORCEINLINE uint32 GetLength() const;			// number of characaters
		FORCEINLINE uint32 GetCapacity() const;

		// TODO make va_list class to overload it properly
		void AppendVariableArgs(const char* format, const va_list args);
		void Append(const char* format, ...);

		char operator[] (const int index) const;
		char& operator[] (const int index);
		String& operator= (const String&);

		const char* SubString(const int index) const;
		const char* GetCString() const;

		operator StringWrapper() const;
		operator UnicodeStringWrapper() const;

	private:
		uint32 mNextIndex = 0;			// index of null char for easy to append
		uint32 mNumCharacters = 0;
		Array<char, CAPACITY> mData;
	};

	template < uint32 SIZE >
	String< SIZE >::String(const char* const value, const uint32 length)
	{
		mNextIndex = length;
		Assert(mNextIndex + 1 < SIZE, "assigned small size string");
		MemCpy(mData.Elements, SIZE, value, std::min(mNextIndex, SIZE));
		mData.Elements[mNextIndex] = 0;
	}

	template < uint32 SIZE >
	String< SIZE >::String(const String<SIZE>& rhs)
	{
		*this = rhs;
	}

	template < uint32 SIZE >
	String< SIZE >::String(const char* const value)
		: String(value, (uint32)strlen(value))
	{
	}

	template < uint32 SIZE >
	String< SIZE >::String(const StringWrapper& str)
		: String(str.Data, str.Length)
	{
	}

	template < uint32 SIZE >
	uint32 String< SIZE >::GetLength() const
	{
		return mNextIndex;
	}

	template < uint32 SIZE >
	uint32 String< SIZE >::GetCapacity() const
	{
		return SIZE;
	}

	template < uint32 SIZE >
	void String< SIZE >::AppendVariableArgs(const char* format, const va_list args)
	{
		Assert(format != nullptr, "cannot use null pointer for format string");

		int numAddedChar = vsnprintf(&(mData[mNextIndex]), SIZE - mNextIndex, format, args);	// don't need to add null char

		Assert(0 < numAddedChar, "not enough space to append string");	// if numAddedChar < 0, then fail to append

		mNextIndex += numAddedChar;
	}

	template < uint32 SIZE >
	void String< SIZE >::Append(const char* format, ...)
	{
		Assert(format != nullptr, "cannot use null pointer for format string");

		va_list arglist;
		va_start(arglist, format);
		AppendVariableArgs(format, arglist);
		va_end(arglist);
	}

	template < uint32 SIZE >
	char String< SIZE >::operator[] (const int index) const
	{
		Assert(index < ToSigned(mNextIndex), "accessing out-of-index in string");

		return mData[index];
	}

	template < uint32 SIZE >
	char& String< SIZE >::operator[] (const int index)
	{
		Assert(index < ToSigned(mNextIndex), "accessing out-of-index in string");

		return mData[index];
	}

	template < uint32 SIZE >
	String<SIZE>& String< SIZE >::operator= (const String< SIZE >& rhs)
	{
		mNextIndex = rhs.mNextIndex;
		MemCpy(mData.Elements, SIZE, rhs.mData.Elements, SIZE);

		return *this;
	}

	template < uint32 SIZE >
	const char* String< SIZE >::SubString(const int index) const
	{
		Assert(index < ToSigned(mNextIndex), "accessing out-of-index in string");
		Assert(index < ToSigned(SIZE), "accessing out-of-index in string");

		return &mData[index];
	}
	
	template < uint32 SIZE >
	const char* String< SIZE >::GetCString() const
	{
		return mData.Elements;
	}

	template < uint32 SIZE >
	String< SIZE >::operator StringWrapper() const
	{
		StringWrapper str;
		str.Data = mData.Elements;
		str.Length = mNextIndex;
		
		return str;
	}

	template < uint32 SIZE >
	String< SIZE >::operator UnicodeStringWrapper() const
	{
		UnicodeStringWrapper str;
		str.Data = mData.Elements;
		str.Length = mNextIndex;
		str.ByteLength = mNextIndex;

		return str;
	}
}
