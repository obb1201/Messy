#include <audio/AudioSystem.h>
#include "System/UniquePtr.h"

namespace fm
{
	UniquePtr<AudioSystem> AudioSystem::mInstance;

	void AudioSystem::Init()
	{
		AudioPlatform::Init();
	}

	void AudioSystem::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<AudioSystem>(fmNew(eMemPool::Audio, "audio instance") AudioSystem());
	}

	void AudioSystem::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated audiosystem");
		mInstance = nullptr;
	}

	AudioSystem::~AudioSystem()
	{
		StopAllSound();
		ReleaseAssets();
	}
}
