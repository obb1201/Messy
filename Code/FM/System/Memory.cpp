#include <corecrt_malloc.h>

#include "System/Memory.h"
#include "System/GlobalInclude.h"
#include "System/Memory/MemoryTracker.h"

namespace fm
{
	void TrackedDelete(void* ptr)
	{
		MemoryTracker::TrackDeallocate(ptr);
	}
}

void* operator new(const size_t size, const fm::eMemPool pooltype, const char* description)
{
	void* ptr = malloc(size);
	fm::MemoryTracker::TrackAllocate(ptr, (const fm::uint64)size, pooltype, description);

	return ptr;
}

void operator delete(void* ptr, const fm::eMemPool pooltype, const char* description)
{
	Assert(false, "it is not supposed to be used");
}

