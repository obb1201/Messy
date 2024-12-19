#include "System\Memory.h"

#include "ThreadManager.h"
#include "WorkerThread.h"
#include "ThreadUtility.h"

namespace fm
{
	UniquePtr<ThreadManager> ThreadManager::mInstance;

	void ThreadManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<ThreadManager>(fmNew(eMemPool::Threading, "Thread Manager instance") ThreadManager());
	}

	void ThreadManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated ThreadManager");
		mInstance.reset();
	}

	void ThreadManager::RegisterThread(const std::thread::id& id, const eThreadID threadID)
	{
		Assert(mThreads[int(threadID)] == std::thread::id());

		SetThreadName(id, toString(threadID));

		mThreads[int(threadID)] = id;
	}

	const eThreadPriority ThreadManager::GetPriority(void* nativeHandle)
	{
		return eThreadPriority(GetThreadPriority(nativeHandle));
	}

	void ThreadManager::SetPriority(void* nativeHandle, const eThreadPriority priority)
	{
		SetThreadPriority(nativeHandle, int32(priority));
	}

	bool ThreadManager::IsWorkerThread() const
	{
		const auto& id = std::this_thread::get_id();
		for (int i = int(eThreadID::Worker0); i < int(eThreadID::Count); ++i)
		{
			if (mThreads[i] == id)
			{
				return true;
			}
		}

		return false;
	}

	const char* ThreadManager::toString(const eThreadID threadID) const
	{
		switch (threadID)
		{
		case eThreadID::Main:
			return "Main";
		case eThreadID::FileIO:
			return "FileIO";
		case eThreadID::Network:
			return "Network";
		case eThreadID::Debug:
			return "Debug";
		case eThreadID::Worker0:
			return "Worker0";
		case eThreadID::Worker1:
			return "Worker1";
		case eThreadID::Worker2:
			return "Worker2";
		case eThreadID::Worker3:
			return "Worker3";
		case eThreadID::Worker4:
			return "Worker4";
		case eThreadID::Worker5:
			return "Worker5";
		case eThreadID::Worker6:
			return "Worker6";
		case eThreadID::Worker7:
			return "Worker7";
		case eThreadID::Worker8:
			return "Worker8";
		case eThreadID::Worker9:
			return "Worker9";
		case eThreadID::Worker10:
			return "Worker10";
		case eThreadID::Worker11:
			return "Worker11";
		case eThreadID::Worker12:
			return "Worker12";
		case eThreadID::Worker13:
			return "Worker13";
		case eThreadID::Worker14:
			return "Worker14";
		case eThreadID::Worker15:
			return "Worker15";
		default:
			Assert(false, "unknown data type");
			break;
		}

		return "None";
	}
}

