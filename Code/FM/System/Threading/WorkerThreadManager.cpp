#include "WorkerThreadManager.h"

namespace fm
{
	UniquePtr<WorkerThreadManager> WorkerThreadManager::mInstance;

	void WorkerThreadManager::CreateInstance()
	{
		Assert(mInstance.get() == nullptr, "don't create this twice");
		mInstance = UniquePtr<WorkerThreadManager>(fmNew(eMemPool::Threading, "Worker Manager instance") WorkerThreadManager());
	}

	void WorkerThreadManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated WorkerThreadMangager");
		mInstance.reset();
	}

	WorkerThreadManager::~WorkerThreadManager()
	{
		for (uint32 i = 0; i < mWorkers.GetSize(); ++i)
		{
			mWorkers[i]->RequestExit();
		}

		// if worker threads is sleeping, we should wake up them to finish their execution.
		{
			std::unique_lock<std::mutex> lock(mSharedWorkersMutex);
			mWorkerThreadWaitEvent.notify_all();
		}

		for (uint32 i = 0; i < mWorkers.GetSize(); ++i)
		{
			mWorkers[i]->Kill();
		}

		for (uint32 i = 0; i < mWorkers.GetSize(); ++i)
		{
			SafeDelete(mWorkers[i]);
		}

		mWorkers.RemoveAll();
	}

	void WorkerThreadManager::Initialize(const uint8 numThreads)
	{
		mNumThreads = numThreads;
		MemSet(mJobInfos.Elements, 0, sizeof(mJobInfos.Elements));

		ThreadManager& threadManager = ThreadManager::GetInstance();
		for (uint32 i = 0; i < mNumThreads; ++i)
		{
			const eThreadID threadID = (eThreadID)(int(eThreadID::Worker0) + i);

			WorkerThread* worker = fmNew(eMemPool::Threading, "Worker Thread instance") WorkerThread(threadID);

			mWorkers.Add(worker);
		}
	}

	void WorkerThreadManager::RunAndWait()
	{
		Assert(ThreadManager::GetInstance().IsMainThread());
		
		const uint32 numJobs = mJobs.GetSize();
		if (numJobs == 0)
		{
			return;
		}
		
		const uint32 numJobsPerWorker = numJobs / mNumThreads;
		for (uint32 i = 0; i < mNumThreads; ++i)
		{
			mJobInfos[i].StartIndex = i * numJobsPerWorker;
			mJobInfos[i].Count = numJobsPerWorker;
		}
		mJobInfos[mNumThreads - 1].Count = numJobs - (mNumThreads - 1) * numJobsPerWorker;
	
		{
			std::unique_lock<std::mutex> lock(mSharedWorkersMutex);
			mNumRunningWorkers = (numJobsPerWorker > 0) ? mNumThreads : 1;

			// wake up all the workers and barrier lock
			mWorkerThreadWaitEvent.notify_all();
			mMainThreadWaitEvent.wait(lock);
		}

		mJobs.RemoveAll();
	}
}
