#include "SplashScreenState.h"

#include "Controller/ControllerManager.h"
#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/MessyGame.h"
#include "System/Logger.h"

namespace messy
{
	void SplashScreenState::OnStart()
	{
		mFramePass = mTimeOutFrameNumber;
		mKeyPressed = false;
		//mFinishLoading = true // TODO: when loading is working, uncomment
	
		LOG(LogLevel::Debug, "===This is Splash Screen, Press Space to Skip===");
	}

	void SplashScreenState::OnRemove()
	{
	}

	void SplashScreenState::OnPause()
	{
	}

	void SplashScreenState::OnResume()
	{
	}

	void SplashScreenState::PreUpdate()
	{
	}

	void SplashScreenState::Update(const uint32 frameCount)
	{
		if (mFinishLoading && (mKeyPressed || (mFramePass <= 0)))
		{
			GameStateManager::GetInstance().RequestPop();
		}
		else if (mFramePass > 0)
		{
			mFramePass -= frameCount;
		}

#if 0
		if (ControllerManager::GetInstance().getAction(0, Action::OK))
		{
			mKeyPressed = true;
		}
#endif
	}

	void SplashScreenState::PostUpdate()
	{
	}

	void SplashScreenState::PreRender()
	{
	}

	void SplashScreenState::Render(const uint32 frameCount)
	{
	}

	void SplashScreenState::PostRender()
	{
	}
}