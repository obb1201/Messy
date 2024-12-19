#include "FrontEndManager.h"
#include "system/Assert.h"
#include "system/memory.h"

namespace fm
{
	UniquePtr<FrontEndManager> FrontEndManager::mInstance;

	void FrontEndManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<FrontEndManager>(fmNew(eMemPool::Rendering, "render instance") FrontEndManager());
	}

	void FrontEndManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated renderer");
		mInstance = nullptr;
	}
}