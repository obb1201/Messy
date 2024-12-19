#include "WorkerThread.h"
#include "WorkerThreadManager.h"

namespace fm
{
	WorkerThread::WorkerThread(const eThreadID threadID)
		: mbExit(false)
		, mThreadID(threadID)
		, mThread(&WorkerThread::Run, this)
	{
	}

	WorkerThread::~WorkerThread()
	{
		Kill();
	}

	void WorkerThread::Kill()
	{
		if (mThread.joinable())
		{
			mThread.join();
		}
	}

	void WorkerThread::RequestExit()
	{
		mbExit = true;
	}

	void WorkerThread::Run()
	{
		ThreadManager::GetInstance().RegisterThread(mThread.get_id(), mThreadID);

		Assert(ThreadManager::GetInstance().IsWorkerThread());

		WorkerThreadManager& workerManager = WorkerThreadManager::GetInstance();
		const uint32 workerIndex = uint32(mThreadID) - uint32(eThreadID::Worker0);
		WorkerThreadManager::WorkerJobInfo* jobInfo = &workerManager.mJobInfos[workerIndex];

		bool bHasRun = false;
		while (!mbExit)
		{
			{
				// wait until job is queued
				std::unique_lock<std::mutex> lock(workerManager.mSharedWorkersMutex);

				// wake up the main thread if this was the last worker that finish
				if (bHasRun && --workerManager.mNumRunningWorkers == 0)
				{
					workerManager.mMainThreadWaitEvent.notify_one();
					bHasRun = false;
				}

				workerManager.mWorkerThreadWaitEvent.wait(lock);
			}

			bHasRun = (jobInfo->Count > 0);
			const uint32 endIndex = jobInfo->StartIndex + jobInfo->Count;
			for (uint32 i = jobInfo->StartIndex; i < endIndex; ++i)
			{
				const Job& job = workerManager.mJobs[i];
				job.Update(job);
			}
			jobInfo->Count = 0;
		}
	}
}
