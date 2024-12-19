#include "MemoryTracker.h"
#include "System/Logger.h"

namespace fm
{
	std::mutex MemoryTracker::mMemoryTracksLock;
	HashMap<MemoryTracker::MemoryTrack, MemoryTracker::HASHSIZE> MemoryTracker::mMemoryTracks;
	std::atomic<uint64> MemoryTracker::mPoolSize[(uint32)eMemPool::Count] = {0,};
	uint64 MemoryTracker::mPoolSizeCheckPoint[(uint32)eMemPool::Count] = { 0, };

	void MemoryTracker::TrackAllocate(const void* const memory, const uint64 size, const eMemPool pooltype, const char* description)
	{
		mPoolSize[(uint32)pooltype] += size;

		const hashid key = reinterpret_cast<const hashid>(memory);

		std::lock_guard<std::mutex> lock(mMemoryTracksLock);
		mMemoryTracks.Put(key, MemoryTrack{ size, pooltype });
	}

	void MemoryTracker::TrackDeallocate(const void* const memory)
	{
		const hashid key = reinterpret_cast<const hashid>(memory);

		mMemoryTracksLock.lock();
		const MemoryTrack track = mMemoryTracks.Remove(key);
		mMemoryTracksLock.unlock();
		mPoolSize[(uint32)track.PoolType] -= track.Size;
	}

#define MEMORYTRACK_TUPLEENTRY(e, str) str
	static char* MemoryTypeStrings[]
	{
		MEMORYTRACK_TUPLE
	};
#undef MEMORYTRACK_TUPLEENTRY

	void MemoryTracker::OutputRemainingMemory()
	{
		LOG(LogLevel::Info, "--- Memory Stats ---");
		for (uint32 i = 0; i < (uint32)eMemPool::Count; ++i)
		{
			LOG(LogLevel::Info, "[%9s] %9d bytes", MemoryTypeStrings[i], mPoolSize[i]);
		}
	}

	void MemoryTracker::CreateCheckPoint()
	{
		LOG(LogLevel::Info, "--- Memory Checkpoint Created ---");
		OutputRemainingMemory();
		for (uint32 i = 0; i < (uint32)eMemPool::Count; ++i)
		{
			mPoolSizeCheckPoint[i] = mPoolSize[i];
		}
	}

	void MemoryTracker::RevertAndValidateCheckPoint()
	{
		LOG(LogLevel::Info, "--- Memory Checkpoint Reverted ---");
		OutputRemainingMemory();
		for (uint32 i = 0; i < (uint32)eMemPool::Count; ++i)
		{
			Assert(mPoolSizeCheckPoint[i] == mPoolSize[i]);
			mPoolSizeCheckPoint[i] = 0;
		}
	}
}