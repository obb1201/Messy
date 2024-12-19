#pragma once

#include "Gameplay/Game.h"
#include "System/Container/Vector.h"
#include "System/Hash.h"
#include "System/UniquePtr.h"

#include "GameObject/Ball.h"

using namespace fm;

namespace messy
{
	enum Action {OK, Back, Left, Right, Up, Down};

	class SplashScreenState;
	class MainMenuState;
	class LoadingScreenState;
	class GamePlayState;
	class PauseMenuState;

	class MessyGame final : public Game
	{
	public:
		MessyGame();
		~MessyGame();
		void SetUpGameStates();
		void SetUpInputs();
		void SetUpEntities();

	protected:
		virtual void Update(const uint32 frameCount) override;

	private:
		UniquePtr<SplashScreenState> mSplashScreenState;
		UniquePtr<MainMenuState> mMainMenuState;
		UniquePtr<LoadingScreenState> mLoadingScreenState;
		UniquePtr<GamePlayState> mGamePlayState;
		UniquePtr<PauseMenuState> mPauseMenuState;

		// HACK : ball test
		static const int msBallNum = 10;
		Ball mTestBalls[msBallNum];
	};
}