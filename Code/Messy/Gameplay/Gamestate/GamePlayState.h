#pragma once

#include "Gameplay/Gamestate/GameState.h"

using namespace fm;

namespace messy
{
	class GamePlayState final : public GameState
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

		void SetPauseState(GameState* state){ mPauseState = state; }
	
	private:
		GameState* mPauseState = nullptr;
		char mText[64];
		int32 mNumText = 0;
	};
}