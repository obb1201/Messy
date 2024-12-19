#pragma once
#include "System\Assert.h"
#include "System\GlobalInclude.h"
#include "System\Container\Vector.h"

#include "WorkerThread.h"

// WorkerThreadManager manage worker threads to control game components in parallel  
// It should be able to switch single thread mode or multi thread mode.
// It has to add jobs to parallelize components into some queues.
// It can not add components into GOM on multi thread. 
namespace fm
{
	class WorkerThreadManager final
	{
		friend class WorkerThread;
		FRIEND_UNIQUEPTR(WorkerThreadManager);
	public:
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();

		static WorkerThreadManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(WorkerThreadManager) = delete;
		ASSIGN_OPERATOR(WorkerThreadManager) = delete;

		void Initialize(const uint8 numThreads);

		void AddJob(const Job& job) { Assert(mNumThreads != 0, "need to be more than 1 of thread"); mJobs.Add(job); }
		void RunAndWait();

	private:
		static UniquePtr<WorkerThreadManager> mInstance;
		
		static const uint8 MAXIMUM_WORKERS = 16;
		static const uint32 MAXIMUM_JOBS_WORKER = 512;
		static const uint32 MAXIMUM_JOBS = MAXIMUM_WORKERS * MAXIMUM_JOBS_WORKER;

		struct WorkerJobInfo
		{
			uint32 StartIndex;
			uint32 Count;
		};

		uint32 mNumRunningWorkers;
		std::mutex mSharedWorkersMutex;
		std::condition_variable mWorkerThreadWaitEvent;
		std::condition_variable mMainThreadWaitEvent;

		Vector<WorkerThread*, MAXIMUM_WORKERS> mWorkers;
		Array<WorkerJobInfo, MAXIMUM_WORKERS> mJobInfos;
		Vector<Job, MAXIMUM_JOBS> mJobs;

		uint8 mNumThreads = 0;

		WorkerThreadManager() = default;
		~WorkerThreadManager();
	};
}
