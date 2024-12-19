#pragma once

#include "System/GlobalInclude.h"
#include "System/Container/Stack.h"
#include "System/UniquePtr.h"
#include "GameState.h"

namespace fm
{
	class GameStateManager final
	{
	public:
		// TODO: delete constructor and assignment 
		static void CreateInstance();
		static void DeleteInstance();
		static GameStateManager& GetInstance() { Assert(mInstance.get() != nullptr, "create game state manager instance before getting it"); return *mInstance; }

		void PreUpdate();
		void Update(const uint32 frameCount);
		void PostUpdate();
		void PreRender();
		void Render(const uint32 frameCount);
		void PostRender();

		void Pop();
		void RequestPush(GameState* const state);
		void RequestPop();
		GameState* Peek();
		uint32 GetStateCount() const { return mGameStates.GetSize(); }

	private:
		struct PushPopRequest
		{
			bool bPush;
			GameState* State;
		};

		static UniquePtr<GameStateManager> mInstance;

		static constexpr int MAX_STATES = 16;
		Stack<GameState*, MAX_STATES> mGameStates;
		
		// TODO: can we use container?
		PushPopRequest mPushPopRequests[MAX_STATES];
		uint32 mNextRequestIndex = 0;
	};
}