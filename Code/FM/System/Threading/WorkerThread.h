#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "System\GlobalInclude.h"
#include "System\Container\Queue.h"

#include "Job.h"
#include "ThreadManager.h"

namespace fm
{
	class WorkerThread final
	{
	public:
		WorkerThread(const eThreadID threadID);
		~WorkerThread();

		COPY_CONSTRUCTOR(WorkerThread) = delete;
		ASSIGN_OPERATOR(WorkerThread) = delete;

		void Kill();
		void RequestExit();

		eThreadID GetID() const { return mThreadID; }
		std::thread& GetThread() { return mThread; }

	private:
		bool mbExit;
		eThreadID mThreadID;
		std::thread mThread;

		// --- methods
		void Run();
	};
}
