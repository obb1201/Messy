#pragma once

#include <Windows.h>
#include <System/GlobalInclude.h>

namespace fm
{
	class GamePC
	{
	public:
		GamePC() = default;
		virtual ~GamePC();

		void SetWindowInstance(HINSTANCE windowInstance);
		void Update(const uint32 frameCount);
	
	protected:
		HWND mWindowHandle = nullptr;

	protected:
		void Init(const uint32 width, const uint32 height);

	private:
		HINSTANCE mWindowInstance = nullptr;
	};
}
