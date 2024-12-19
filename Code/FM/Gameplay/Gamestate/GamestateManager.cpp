#include "GamestateManager.h"

namespace fm
{
	UniquePtr<GameStateManager> GameStateManager::mInstance;

	void GameStateManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<GameStateManager>(fmNew(eMemPool::Gameplay, "GameState Manager instance") GameStateManager());
	}

	void GameStateManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated GameStateManager");
		mInstance = nullptr;
	}

	void GameStateManager::RequestPush(GameState* const state)
	{
		Assert(mNextRequestIndex < MAX_STATES, "request exceed allow size");

		PushPopRequest &request = mPushPopRequests[mNextRequestIndex++];
		{
			request.bPush = true;
			request.State = state;
		}
	}

	void GameStateManager::RequestPop()
	{
		Assert(mNextRequestIndex < MAX_STATES, "request exceed allow size");

		PushPopRequest &request = mPushPopRequests[mNextRequestIndex++];
		request.bPush = false;
		request.State = nullptr;
	}

	GameState* GameStateManager::Peek()
	{
		Assert(!mGameStates.IsEmpty(), "peeked an empty stack");
		return (GameState*)mGameStates.Peek();
	}

	void GameStateManager::PreUpdate()
	{
		for (auto i = 0U; i < mNextRequestIndex; ++i)
		{
			PushPopRequest& request = mInstance->mPushPopRequests[i];
			if (request.bPush)
			{
				request.State->OnStart();
				mGameStates.Push(request.State);
			}
			else
			{
				request.State = (GameState*)mGameStates.Pop();
				request.State->OnRemove();
			}
			request.bPush = true;
			request.State = nullptr;
		}
		mNextRequestIndex = 0;
		
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->PreUpdate();
			if (!state->bUpdatePrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::Update(const uint32 frameCount)
	{
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->Update(frameCount);
			if (!state->bUpdatePrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::PostUpdate()
	{
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->PostUpdate();
			if (!state->bUpdatePrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::PreRender()
	{
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->PreRender();
			if (!state->bDrawPrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::Render(const uint32 frameCount)
	{
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->Render(frameCount);
			if (!state->bDrawPrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::PostRender()
	{
		const uint32 stackdepth = mGameStates.GetSize();
		for (auto i = 0U; i < stackdepth; ++i)
		{
			GameState * state = (GameState*)mGameStates.Peek(i);
			state->PostRender();
			if (!state->bDrawPrevious)
			{
				break;
			}
		}
	}

	void GameStateManager::Pop()
	{
		Assert(mGameStates.GetSize() > 0, "nothing to pop");
		GameState* gs=  (GameState*) mGameStates.Pop();
		gs->OnRemove();
	}
}