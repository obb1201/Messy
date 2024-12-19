#pragma once

#include <Dsound.h>

typedef IDirectSoundBuffer SoundBufferPlatform;

namespace fm
{
	class AudioSystemPC
	{
	public:
		void SetWindowHandle(const HWND hwnd) { mWindowHandle = hwnd; }

	protected:
		AudioSystemPC() = default;
		virtual ~AudioSystemPC();
		void Init();

	protected:
		LPDIRECTSOUND8 mDirectSoundDevice = nullptr;
		HWND mWindowHandle = 0;
	};
}
