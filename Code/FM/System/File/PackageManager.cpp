#include "PackageManager.h"

#include "Resource/Data/TextureData.h"
#include "Resource/Data/ModelData.h"
#include "Resource/Data/AnimationData.h"
#include "Resource/TextureManager.h"
#include "Resource/ModelManager.h"
#include "Resource/AnimationManager.h"
#include "System/Memory.h"
#include "System/File/Package.h"
#include "System/Threading/ThreadManager.h"

namespace fm
{
	UniquePtr<PackageManager> PackageManager::mInstance;

	void PackageManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<PackageManager>(fmNew(eMemPool::Resource, "Package Manager instance") PackageManager());
	}

	void PackageManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated PackageManager");
		mInstance.reset();
	}

	PackageManager::PackageManager()
	{
		MemSet(mPackageDescs.Elements, 0, sizeof(mPackageDescs.Elements));

		std::lock_guard<std::mutex> lock(mQueueLock);

		mThread = MakeUnique<std::thread>(eMemPool::Resource, "resource manager thread", &PackageManager::doWork, this);
		ThreadManager::GetInstance().RegisterThread(mThread->get_id(), eThreadID::FileIO);
	}

	PackageManager::~PackageManager()
	{
		bShouldExit = true;
		{
			std::unique_lock<std::mutex> lock(mQueueLock);
			mFileIOThreadWaitEvent.notify_one();
		}

		// TODO: we should know this is joinable or not when we create it. no?
		if (mThread->joinable())
		{
			mThread->join();
		}
	}

	PackageHandle PackageManager::RequestLoad(const char* const relativePath)
	{
		LoadRequest request;
		request.Handle = INVALID_HANDLE;
		request.RelativePath = relativePath;

		for (uint32 i = 0; i < mPackageDescs.GetLength(); ++i)
		{
			PackageDesc& package = mPackageDescs[i];
			if (package.Status == ePackageStatus::Unused)
			{
				request.Handle = i;
				package.Status = ePackageStatus::Queued;
				break;
			}
		}

		Assert(request.Handle != INVALID_HANDLE, "not enough splace in mPackages");
		{
			std::lock_guard<std::mutex> Guard(mQueueLock);
			mLoadQueue.Enqueue(request);
			mFileIOThreadWaitEvent.notify_one();
		}

		return request.Handle;
	}

	void PackageManager::doWork()
	{
		{
			std::lock_guard<std::mutex> guard(mQueueLock);
			Assert(ThreadManager::GetInstance().IsFileIOThread());
		}

		while (!bShouldExit)
		{
			{
				// wait until package is queued
				std::unique_lock<std::mutex> lock(mQueueLock);
				if (mLoadQueue.IsEmpty())
				{
					mFileIOThreadWaitEvent.wait(lock);
				}
			}

			if (!mLoadQueue.IsEmpty())
			{
				LoadRequest request;
				{
					std::lock_guard<std::mutex> guard(mQueueLock);
					request = mLoadQueue.Dequeue();
				}

				Package package(request.RelativePath.GetCString(), true, false);
				const PackageDataHeader& header = package.GetHeader();
				PackageDesc& packageDesc = mPackageDescs[request.Handle];
				Assert(packageDesc.Resources.GetSize() == 0);

				const auto& fileTable = package.mPackageData->FileTable;

				ResourceDesc resource;
				for (uint32 i = 0; i < header.NumFiles; ++i)
				{
					const PackageData::FileInfo& info = fileTable.Infos[i];

					resource.Name = fileTable.Names[i];
					resource.Type = info.Type;
					const void* file = package.GetFileByIndex(i);

					switch (info.Type)
					{
					case eFileType::Texture:
					{
						const TextureData& texture = *reinterpret_cast<const TextureData*>(file);
						resource.Handle = TextureManager::GetInstance().Create(texture);
						break;
					}
					case eFileType::Model:
					{
						const ModelData& model = *reinterpret_cast<const ModelData*>(file);
						resource.Handle = ModelManager::GetInstance().Create(model);
						break;
					}
					case eFileType::Animation:
					{
						const AnimationData& animation = *reinterpret_cast<const AnimationData*>(file);
						resource.Handle = AnimationManager::GetInstance().Create(animation);
						break;
					}
					default:
						Assert(false, "unknown data type");
						break;
					}

					packageDesc.Resources.Add(resource);
				}

				packageDesc.Status = ePackageStatus::Loaded;
			}
		}
	}

	void PackageManager::Unload(const PackageHandle handle)
	{
		Assert(handle != INVALID_HANDLE);

		PackageDesc& packageDesc = mPackageDescs[handle];
		auto& resources = packageDesc.Resources;
		for (uint32 i = 0; i < resources.GetSize(); ++i)
		{
			const auto& resource = resources[i];
			switch (resource.Type)
			{
			case eFileType::Texture:
				TextureManager::GetInstance().Delete(resource.Handle);
				break;
			case eFileType::Model:
				ModelManager::GetInstance().Delete(resource.Handle);
				break;
			default:
				Assert(false, "unknown data type");
				break;
			}
		}
		resources.RemoveAll();

		mPackageDescs[handle].Status = ePackageStatus::Unused;
	}
}
