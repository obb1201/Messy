#pragma once
#include <chrono>
#include "System\GlobalInclude.h"

namespace fm
{
	class GameInfo final
	{
		friend class Game;
	public:
		GameInfo(const uint32 framesPerSecond);

		FORCEINLINE const float GetGameSeconds();
		FORCEINLINE const float GetElapsedRealSeconds() const;
		FORCEINLINE const float GetElapsedGameSeconds();
		FORCEINLINE const float GetTimeSinceStartUp();
		FORCEINLINE void ResetGameTime();

	private:
		std::chrono::high_resolution_clock::time_point mStartupTime;
		std::chrono::high_resolution_clock::time_point mLastFrameRealTime;

		uint32 mFramesPerSecond = 0;	// can be changed (60 <-> 30)
		uint32 mFrameCount = 0;
		float mElapsedRealSeconds = 0.0f;

		// --- methods
		void tick();

		FORCEINLINE const uint32 incrementFrameCount();
	};

	FORCEINLINE const uint32 GameInfo::incrementFrameCount()
	{
		return ++mFrameCount;
	}

	FORCEINLINE const float GameInfo::GetGameSeconds()
	{
		return float(mFrameCount) / float(mFramesPerSecond);
	}

	FORCEINLINE const float GameInfo::GetElapsedRealSeconds() const
	{
		return mElapsedRealSeconds;
	}

	FORCEINLINE const float GameInfo::GetElapsedGameSeconds()
	{
		return 1.0f / float(mFramesPerSecond);
	}

	FORCEINLINE const float GameInfo::GetTimeSinceStartUp()
	{
		std::chrono::high_resolution_clock::time_point nowTime = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - mStartupTime);

		return float(miliseconds.count()) / 1000.0f;
	}

	FORCEINLINE void GameInfo::ResetGameTime()
	{
		mFrameCount = 0;
	}
}
