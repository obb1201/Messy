#pragma once

#include "Gameplay/Gamestate/GameState.h"

using namespace fm;

namespace messy
{
	class SplashScreenState final : public GameState
	{
	public:
		void OnStart() override;
		void OnRemove() override;
		void OnPause() override;
		void OnResume() override;

		void PreUpdate() override;
		void Update(const uint32 frameCount) override;
		void PostUpdate() override;
		
		void PreRender() override;
		void Render(const uint32 frameCount) override;
		void PostRender() override;

	private:
		//TODO: replace with time instead of frame
		const uint32 mTimeOutFrameNumber = 100;
		int32 mFramePass;
		bool mFinishLoading = true;//TODO: change this to false when we know what we are loading
		bool mKeyPressed = false;
	};
}