#include "GameInfo.h"

namespace fm
{
	GameInfo::GameInfo(const uint32 framesPerSecond)
		: mFramesPerSecond(framesPerSecond)
	{
		mStartupTime = std::chrono::high_resolution_clock::now();
		mLastFrameRealTime = std::chrono::high_resolution_clock::now();
	}

	void GameInfo::tick()
	{
		std::chrono::high_resolution_clock::time_point nowTime = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - mLastFrameRealTime);
		mElapsedRealSeconds = float(miliseconds.count()) / 1000.0f;

		mLastFrameRealTime = nowTime;
	}
}