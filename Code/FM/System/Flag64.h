#pragma once

#include "system/assert.h"

namespace fm
{
	class Flag64
	{
	public:
		FORCEINLINE void SetBit(uint8 bit);
		FORCEINLINE void ClearBit(uint8 bit);
		FORCEINLINE bool IsSetBit(uint8 bit) const;

		FORCEINLINE void SetFlags(uint64 flags);
		FORCEINLINE void ClearFlags(uint64 flags);
		FORCEINLINE bool IsSetAnyFlags(uint64 flags) const;
		FORCEINLINE bool IsSetAllFlags(uint64 flags) const;

	private:
		uint64 mFlags = 0;
	};

	void Flag64::SetBit(uint8 bit)
	{
		Assert(bit <= 64, "can't use bit bigger than 64");

		mFlags |= (1ULL << bit);
	}

	void Flag64::ClearBit(uint8 bit)
	{
		Assert(bit <= 64, "can't use bit bigger than 64");
		mFlags &= ~(1ULL << bit);
	}

	bool Flag64::IsSetBit(uint8 bit) const
	{
		Assert(bit <= 64, "can't use bit bigger than 64");

		return ( (mFlags & (1ULL << bit)) != 0);
	}

	void Flag64::SetFlags(uint64 flags)
	{
		mFlags |= flags;
	}

	void Flag64::ClearFlags(uint64 flags)
	{
		mFlags &= ~flags;
	}

	bool Flag64::IsSetAnyFlags(uint64 flags) const
	{
		return ( (mFlags & flags) != 0);
	}
	
	bool Flag64::IsSetAllFlags(uint64 flags) const
	{
		return ( (mFlags & flags) == flags);
	}
}