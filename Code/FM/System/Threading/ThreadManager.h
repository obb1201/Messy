#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "System/Container\Vector.h"
#include "System/Assert.h"
#include "System/UniquePtr.h"

namespace fm
{
	// the range value of window's thread priority is -2 to 2 between THREAD_PRIORITY_LOWEST and THREAD_PRIORITY_HIGHEST.
	enum class eThreadPriority
	{
		Lowest	= -2,
		Low		= -1,
		Normal	= 0,
		High	= 1,
		Highest = 2,
	};

	enum class eThreadID
	{
		Main,
		FileIO,
		Network,
		Debug,

		Worker0,
		Worker1,
		Worker2,
		Worker3,
		Worker4,
		Worker5,
		Worker6,
		Worker7,
		Worker8,
		Worker9,
		Worker10,
		Worker11,
		Worker12,
		Worker13,
		Worker14,
		Worker15,

		Count
	};

	class WorkerThread;

	class ThreadManager final
	{
		FRIEND_UNIQUEPTR(ThreadManager);
	public:
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();

		static ThreadManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(ThreadManager) = delete;
		ASSIGN_OPERATOR(ThreadManager) = delete;

		void RegisterThread(const std::thread::id& id, const eThreadID threadID);

		const eThreadPriority GetPriority(void* nativeHandle);
		void SetPriority(void* nativeHandle, const eThreadPriority priority);

		bool IsMainThread() const { return mThreads[int(eThreadID::Main)] == std::this_thread::get_id(); }
		bool IsFileIOThread() const { return mThreads[int(eThreadID::FileIO)] == std::this_thread::get_id(); }
		bool IsWorkerThread() const;

	private:
		static UniquePtr<ThreadManager> mInstance;

		Array<std::thread::id, int(eThreadID::Count)> mThreads = {};

		ThreadManager() = default;
		~ThreadManager() = default;

		const char* toString(const eThreadID threadID) const;
	};
}
