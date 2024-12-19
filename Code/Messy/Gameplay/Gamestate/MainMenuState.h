#pragma once

#include "Gameplay/Gamestate/GameState.h"

using namespace fm;

namespace messy
{
	class MainMenuState final : public GameState
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

		void SetLoadingScrennState(GameState* state){ mLoadingScreenState = state; }

	private:
		GameState* mLoadingScreenState = nullptr;
	};
}