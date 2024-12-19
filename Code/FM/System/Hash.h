#pragma once

#include <cstring>
#include "GlobalInclude.h"

namespace fm
{
	const uint64 INVALID_HASH = 0u;
	typedef uint64 hashid;

	struct ConstCharWrapper
	{
		inline ConstCharWrapper(const char* str) : String(str) {}
		const char* String;
	};

	// TODO: constexpr
	inline hashid HashString(const char* str, const size_t len)
	{
		uint32 hash = 0;
		for (size_t i = 0; i < len; ++i)
		{
			hash = 65599 * hash + str[i];
		}

		return hash ^ (hash >> 16);
	}

	FORCEINLINE hashid HashString(const ConstCharWrapper& wrapper)
	{
		const char * str = wrapper.String;
		return HashString(str, strlen(str));
	}

	// ------------------------------------------------
	// Unrolled overloads to make compile time hashes up to 20 characters
	inline hashid HashString(const char(&str)[2])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[3])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[4])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[5])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[6])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[7])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[8])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[9])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[10])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[11])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[12])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[13])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[14])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[15])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[16])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[17])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];
		hash = hash * 65599 + str[15];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[18])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];
		hash = hash * 65599 + str[15];
		hash = hash * 65599 + str[16];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[19])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];
		hash = hash * 65599 + str[15];
		hash = hash * 65599 + str[16];
		hash = hash * 65599 + str[17];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[20])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];
		hash = hash * 65599 + str[15];
		hash = hash * 65599 + str[16];
		hash = hash * 65599 + str[17];
		hash = hash * 65599 + str[18];

		return hash ^ (hash >> 16);
	}

	inline hashid HashString(const char(&str)[21])
	{
		uint32 hash = 0;
		hash = hash * 65599 + str[0];
		hash = hash * 65599 + str[1];
		hash = hash * 65599 + str[2];
		hash = hash * 65599 + str[3];
		hash = hash * 65599 + str[4];
		hash = hash * 65599 + str[5];
		hash = hash * 65599 + str[6];
		hash = hash * 65599 + str[7];
		hash = hash * 65599 + str[8];
		hash = hash * 65599 + str[9];
		hash = hash * 65599 + str[10];
		hash = hash * 65599 + str[11];
		hash = hash * 65599 + str[12];
		hash = hash * 65599 + str[13];
		hash = hash * 65599 + str[14];
		hash = hash * 65599 + str[15];
		hash = hash * 65599 + str[16];
		hash = hash * 65599 + str[17];
		hash = hash * 65599 + str[18];
		hash = hash * 65599 + str[19];

		return hash ^ (hash >> 16);
	}
}

