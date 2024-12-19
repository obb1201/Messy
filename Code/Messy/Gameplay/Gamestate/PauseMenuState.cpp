#include "PauseMenuState.h"

#include "Controller/ControllerManager.h"
#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/MessyGame.h"
#include "System/Logger.h"

namespace messy
{
	PauseMenuState::PauseMenuState()
	{
		bDrawPrevious = true;
	}

	void PauseMenuState::OnStart()
	{
		LOG(LogLevel::Debug, "===You are Paused,you should still see GamePlayState Message===");
		LOG(LogLevel::Debug, "===Press CTRL to go back to game===");
	}

	void PauseMenuState::OnRemove()
	{
	}

	void PauseMenuState::OnPause()
	{
	}

	void PauseMenuState::OnResume()
	{
	}

	void PauseMenuState::PreUpdate()
	{
	}

	void PauseMenuState::Update(const uint32 frameCount)
	{
		if (ControllerManager::GetInstance().GetAction(0, Action::Back))
		{
			GameStateManager::GetInstance().RequestPop();
		}
	}

	void PauseMenuState::PostUpdate()
	{
	}

	void PauseMenuState::PreRender()
	{
	}

	void PauseMenuState::Render(const uint32 frameCount)
	{
	}

	void PauseMenuState::PostRender()
	{
	}
}