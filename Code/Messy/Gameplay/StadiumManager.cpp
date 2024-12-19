#include "StadiumManager.h"

#include "System\File\PackageManager.h"
#include "System\File\FileType.h"

#include "Resource\ModelManager.h"

#include "Gameplay\ComponentSystem\GameObjectManager.h"
#include "Gameplay\ComponentSystem\Components\ICmpRenderable.h"
#include "Gameplay\ComponentSystem\Components\CmpEntity.h"

#include "Graphics\Model.h"

namespace messy
{
	UniquePtr<StadiumManager> StadiumManager::mInstance;

	void StadiumManager::CreateInstance()
	{
		Assert(mInstance.get() == nullptr, "don't create this twice");
		mInstance = UniquePtr<StadiumManager>(fmNew(eMemPool::Gameplay, "Stadium Manager instance") StadiumManager());
	}

	void StadiumManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated Stadium Manager");
		mInstance.reset();
	}

	StadiumManager::~StadiumManager()
	{

	}

	void StadiumManager::Initialize()
	{
		// pak
		mPackageHandle = PackageManager::GetInstance().RequestLoad("debugdata/stadium.pak");
		mStadiumObjectID = HashString("OldTrafford");
	}

	void StadiumManager::CleanUp()
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		gom.DeleteObject(mStadiumObjectID);

		PackageManager::GetInstance().Unload(mPackageHandle);

		mbLoadHasFinished = false;
	}

	void StadiumManager::checkStadiumCreated()
	{
		if (mbLoadHasFinished)
		{
			return;
		}

		GameObjectManager& gom = GameObjectManager::GetInstance();
		ModelManager& modelManager = ModelManager::GetInstance();
		PackageManager& packageManager = PackageManager::GetInstance();

		const PackageDesc& packageDesc = packageManager.GetPackageDesc(mPackageHandle);
		mbLoadHasFinished = (packageDesc.Status == ePackageStatus::Loaded);

		if (mbLoadHasFinished)
		{
			for (uint32 i = 0; i < packageDesc.Resources.GetSize(); ++i)
			{
				if (packageDesc.Resources[i].Type == eFileType::Model)
				{
					Model* model = modelManager.Get(packageDesc.Resources[i].Handle);

					CmpEntity* entity = (CmpEntity*)gom.CreateObject(mStadiumObjectID);
					entity->SetScale(Vector3(5.0f, 0.1f, 5.0f)); // HACK : to look like ground.

					ICmpRenderable* mesh = (ICmpRenderable*)gom.CreateComponent(mStadiumObjectID, eComponentType::StaticMesh);
					mesh->SetModel(model);
				}
			}
		}
	}
}