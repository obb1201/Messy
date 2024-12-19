#include "LoadingScreenState.h"

#include "Controller/ControllerManager.h"

#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/MessyGame.h"
#include "Gameplay/StadiumManager.h"
#include "Gameplay/PlayerManager.h"

#include "System/Logger.h"

namespace messy
{
	void LoadingScreenState::OnStart()
	{
		StadiumManager::GetInstance().Initialize();
		PlayerManager::GetInstance().Initialize();

		LOG(LogLevel::Debug, "===This is Loading Screen ===");
	}

	void LoadingScreenState::OnRemove()
	{
		GameStateManager::GetInstance().RequestPush(mGamePlayState);
	}

	void LoadingScreenState::OnPause()
	{
	}

	void LoadingScreenState::OnResume()
	{
	}

	void LoadingScreenState::PreUpdate()
	{
	}

	void LoadingScreenState::Update(const uint32 frameCount)
	{
		StadiumManager& stadiumManager = StadiumManager::GetInstance();
		PlayerManager& playerManager = PlayerManager::GetInstance();

		stadiumManager.checkStadiumCreated();
		playerManager.checkPlayersCreated();

		if (stadiumManager.isLoadHasFinished() && playerManager.isLoadHasFinished())
		{
			GameStateManager::GetInstance().RequestPop();
		}
	}

	void LoadingScreenState::PostUpdate()
	{
	}

	void LoadingScreenState::PreRender()
	{
	}

	void LoadingScreenState::Render(const uint32 frameCount)
	{
	}

	void LoadingScreenState::PostRender()
	{
	}
}