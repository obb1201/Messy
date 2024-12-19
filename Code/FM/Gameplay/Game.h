#pragma once

#include "Game_platform.h"
#include "GameInfo.h"

namespace fm
{
	class Game : public GamePlatform
	{
	public:
		static bool	bExit;

	public:
		Game(const int framesPerSecond);
		virtual ~Game();

		void Init(const uint32 width, const uint32 height);

		void Run();

	protected:	// basically callbacks
		// TODO: what other callbacks do we need more? init? shutdown?
		virtual void Update(const uint32 frameCount);
		virtual void BeginRender(const uint32 frameCount);
		virtual void Render(const uint32 frameCount);
		virtual void EndRender(const uint32 frameCount);

	private:	// variables
		int	mUpdateIntervalMS;
		float mUpdateIntervalSec;

		GameInfo mGameInfo;
	};
}
