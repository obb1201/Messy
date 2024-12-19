#pragma once

#include "System/GlobalInclude.h"

namespace fm
{
	class GameState
	{
	public:
		bool bDrawPrevious = false;
		bool bUpdatePrevious = false;

	public:
		virtual void OnStart() = 0;
		virtual void OnRemove() = 0;
		virtual void OnPause() = 0;
		virtual void OnResume() = 0;

		virtual void PreUpdate() = 0;
		virtual void Update(const uint32 frameCount) = 0;
		virtual void PostUpdate() = 0;
		
		virtual void PreRender() = 0;
		virtual void Render(const uint32 frameCount) = 0;
		virtual void PostRender() = 0;
	};
}