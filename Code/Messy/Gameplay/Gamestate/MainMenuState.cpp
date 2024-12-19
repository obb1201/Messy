#include "MainMenuState.h"

#include "Controller/ControllerManager.h"
#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/MessyGame.h"
#include "System/Logger.h"

namespace messy
{
	void MainMenuState::OnStart()
	{
		LOG(LogLevel::Debug, "===This is Main Menu, Press SHIFT to START===");
	}

	void MainMenuState::OnRemove()
	{
	}

	void MainMenuState::OnPause()
	{
	}

	void MainMenuState::OnResume()
	{
		LOG(LogLevel::Debug, "===This is Main Menu, Press SHIFT to START===");
	}

	void MainMenuState::PreUpdate()
	{
	}

	void MainMenuState::Update(const uint32 frameCount)
	{
		if (ControllerManager::GetInstance().GetAction(0, Action::OK))
		{
			GameStateManager::GetInstance().RequestPush(mLoadingScreenState);
		}
	}

	void MainMenuState::PostUpdate()
	{
	}

	void MainMenuState::PreRender()
	{
	}

	void MainMenuState::Render(const uint32 frameCount)
	{
	}

	void MainMenuState::PostRender()
	{
	}
}