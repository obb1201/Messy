#pragma once

#include <mutex>
#include <condition_variable>

#include "System/GlobalInclude.h"

#include "System/Assert.h"
#include "System/Container/Queue.h"
#include "System/File/PackageDesc.h"
#include "System/String.h"
#include "System/UniquePtr.h"

namespace fm
{
	typedef uint32 PackageHandle;

	class PackageManager final
	{
		FRIEND_UNIQUEPTR(PackageManager);
	public:
		// --- members
		bool bShouldExit = false;

		COPY_CONSTRUCTOR(PackageManager) = delete;
		ASSIGN_OPERATOR(PackageManager) = delete;

		FORCEINLINE const PackageDesc& GetPackageDesc(const PackageHandle handle) const;

		PackageHandle RequestLoad(const char* const relativePath);
		void Unload(const PackageHandle handle);

		// --- singleton-related
		static void CreateInstance();
		static void DeleteInstance();

		static PackageManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

	private:
		struct LoadRequest
		{
		private:
			// TODO: make it global define?
			static const int MAX_RELATIVE_FILEPATH = 64;

		public:
			PackageHandle Handle;
			String<MAX_RELATIVE_FILEPATH> RelativePath;
		};
		static const int MAXIMUM_PACKAGES = 256;

		// --- members
		static UniquePtr<PackageManager> mInstance;

		std::mutex mQueueLock;
		Array<PackageDesc, MAXIMUM_PACKAGES> mPackageDescs;
		Queue<LoadRequest, MAXIMUM_PACKAGES> mLoadQueue;

		UniquePtr<std::thread> mThread;
		std::condition_variable mFileIOThreadWaitEvent;

		// --- methods
		PackageManager();
		~PackageManager();

		void doWork();	// multi-threaded resource loader
	};

	const PackageDesc& PackageManager::GetPackageDesc(const PackageHandle handle) const
	{
		Assert(handle != INVALID_HANDLE);

		return mPackageDescs[handle];
	}
}
