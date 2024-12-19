#include "GamePlayState.h"

#include "Controller/ControllerManager.h"

#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/MessyGame.h"
#include "Gameplay/StadiumManager.h"
#include "Gameplay/PlayerManager.h"

#include "System/Logger.h"
#include "System/Memory/MemoryTracker.h"

namespace messy
{
	void GamePlayState::OnStart()
	{
		MemoryTracker::CreateCheckPoint();

		// TODO: remove this hack
		LOG(LogLevel::Debug, "===You are now playing a game, SHIFT to Pause===");
		LOG(LogLevel::Debug, "===CTRL to go back to main menu===");
		//LOG(LogLevel::Debug, mText);
	}

	void GamePlayState::OnRemove()
	{
		mText[0] = 0;
		mNumText = 0;

		StadiumManager::GetInstance().CleanUp();
		PlayerManager::GetInstance().CleanUp();

		MemoryTracker::RevertAndValidateCheckPoint();
	}

	void GamePlayState::OnPause()
	{
	}

	void GamePlayState::OnResume()
	{
	}

	void GamePlayState::PreUpdate()
	{
	}

	void GamePlayState::Update(const uint32 frameCount)
	{
		ControllerManager& cm = ControllerManager::GetInstance();
		if (cm.GetAction(0, Action::OK))
		{
			GameStateManager::GetInstance().RequestPush(mPauseState);
		}
		if (cm.GetAction(0, Action::Back))
		{
			GameStateManager::GetInstance().RequestPop();
		}

		PlayerManager::GetInstance().Update();

		//simple text output test code, no error checking, don't type more than 64 char or else
		char buffer[ControllerPlatform::MAX_CHARS_PER_FRAME];
		uint32 outNumTextToDelete;
		cm.GetTextBuffer(buffer, ControllerPlatform::MAX_CHARS_PER_FRAME, outNumTextToDelete);
		if (outNumTextToDelete > 0)
		{
			mNumText -= outNumTextToDelete;
			if (mNumText < 0)
			{
				mNumText = 0;
			}
		}
		uint32 index = 0;
		char c = buffer[index];
		while (c != 0)
		{
			mText[mNumText] = c;
			mNumText++;
			index++;
			c = buffer[index];
		}
		mText[mNumText] = 0;
	}

	void GamePlayState::PostUpdate()
	{
	}

	void GamePlayState::PreRender()
	{
	}

	void GamePlayState::Render(const uint32 frameCount)
	{
	}

	void GamePlayState::PostRender()
	{
	}
}