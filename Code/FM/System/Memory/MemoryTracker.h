#pragma once

#include <atomic>
#include <mutex>

#include "System/Assert.h"
#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"
#include "System/String.h"
#include "System/Container/HashMap.h"

namespace fm
{
	class MemoryTracker final
	{
	public:
		static void TrackAllocate(const void* const memory, const uint64 size, const eMemPool pooltype, const char* description);
		static void TrackDeallocate(const void* const memory);
		static void CreateCheckPoint();
		static void RevertAndValidateCheckPoint();
		static void OutputRemainingMemory();

	private:
		struct MemoryTrack
		{
			uint64 Size;
			eMemPool PoolType;
			/* TODO: enable this when we are ready to track each allocation
			static const uint32 MAX_TRACKSTRING = 64;
			String<MAX_TRACKSTRING> Description;
			*/
		};

		// a prime number to avoid hash bucket collision
		static const uint32 HASHSIZE = 12289;

		// TODO : Is mMemoryTracks thread-safe?
		static std::mutex mMemoryTracksLock;
		static HashMap<MemoryTrack, HASHSIZE> mMemoryTracks;
		static std::atomic<uint64> mPoolSize[(uint32)eMemPool::Count];
		static uint64 mPoolSizeCheckPoint[(uint32)eMemPool::Count];

	private:
		MemoryTracker() = delete;
		~MemoryTracker() = delete;
	};
}