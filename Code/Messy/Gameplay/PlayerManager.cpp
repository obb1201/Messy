#include "PlayerManager.h"

#include "System/File/PackageManager.h"
#include "System/File/FileType.h"

#include "Resource/ModelManager.h"
#include "Resource/AnimationManager.h"

#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "Gameplay/ComponentSystem/Components/ICmpRenderable.h"
#include "Gameplay/ComponentSystem/Components/CmpEntity.h"

#include "Graphics/Model.h"
#include "GameObject/Player.h"

namespace messy
{
	UniquePtr<PlayerManager> PlayerManager::mInstance;

	void PlayerManager::CreateInstance()
	{
		Assert(mInstance.get() == nullptr, "don't create this twice");
		mInstance = UniquePtr<PlayerManager>(fmNew(eMemPool::Gameplay, "player Manager instance") PlayerManager());
	}

	void PlayerManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated Player Manager");
		mInstance.reset();
	}

	PlayerManager::~PlayerManager()
	{
	}

	void PlayerManager::Initialize()
	{
		// HACK : players should have more than 22.
		Handle player1handle = PackageManager::GetInstance().RequestLoad("debugdata/player1.pak");
		Handle player2handle = PackageManager::GetInstance().RequestLoad("debugdata/player2.pak");

		mPlayerLoadInfos.Add(PlayerLoadInfo{ player1handle, HashString("player1"), false });
		mPlayerLoadInfos.Add(PlayerLoadInfo{ player2handle, HashString("player2"), false });
	}

	void PlayerManager::CleanUp()
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		PackageManager& packageManager = PackageManager::GetInstance();

		for (uint32 i = 0; i < mPlayers.GetSize(); ++i)
		{
			SafeDelete(mPlayers[i]);
		}
		mPlayers.RemoveAll();

		for (uint32 handleIndex = 0; handleIndex < mPlayerLoadInfos.GetSize(); ++handleIndex)
		{
			packageManager.Unload(mPlayerLoadInfos[handleIndex].PackageHandle);
			mPlayerLoadInfos[handleIndex].bLoadHasFinished = false;
		}
		mPlayerLoadInfos.RemoveAll();

		mbAllLoadHasFinished = false;
	}

	void PlayerManager::Update()
	{
		for (uint32 i = 0; i < mPlayers.GetSize(); ++i)
		{
			mPlayers[i]->Update();
		}
	}

	void PlayerManager::checkPlayersCreated()
	{
		if (mbAllLoadHasFinished)
		{
			return;
		}

		GameObjectManager& gom = GameObjectManager::GetInstance();
		ModelManager& modelManager = ModelManager::GetInstance();
		PackageManager& packageManager = PackageManager::GetInstance();
		AnimationManager& animationManager = AnimationManager::GetInstance();

		for (uint32 infoIndex = 0; infoIndex < mPlayerLoadInfos.GetSize(); ++infoIndex)
		{
			if (mPlayerLoadInfos[infoIndex].bLoadHasFinished)
			{
				continue;
			}

			const Handle packageHandle = mPlayerLoadInfos[infoIndex].PackageHandle;
			const PackageDesc& packageDesc = packageManager.GetPackageDesc(packageHandle);

			mPlayerLoadInfos[infoIndex].bLoadHasFinished = (packageDesc.Status == ePackageStatus::Loaded);

			if (mPlayerLoadInfos[infoIndex].bLoadHasFinished)
			{
				for (uint32 i = 0; i < packageDesc.Resources.GetSize(); ++i)
				{
					if (packageDesc.Resources[i].Type == eFileType::Model)
					{
						Model* model = modelManager.Get(packageDesc.Resources[i].Handle);

						Player* player = fmNew(eMemPool::Gameplay, "player instance") Player(mPlayerLoadInfos[infoIndex].ObjectID, model);
						mPlayers.Add(player);
					}
					else if (packageDesc.Resources[i].Type == eFileType::Animation)
					{
						Animation* animation = animationManager.Get(packageDesc.Resources[i].Handle);

						// TODO : add animation.
					}
				}
			}
		}

		// 
		mbAllLoadHasFinished = (mPlayers.GetSize() == mPlayerLoadInfos.GetSize());
	}
}