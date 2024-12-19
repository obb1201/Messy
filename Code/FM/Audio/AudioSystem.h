#pragma once

#include "audio/AudioPlatform.h"
#include "system/hash.h"
#include "system/assert.h"
#include "system/container/Vector.h"
#include "System/UniquePtr.h"

namespace fm
{
	class AudioSystem final : public AudioPlatform
	{
		FRIEND_UNIQUEPTR(AudioSystem);
	public:
		static void CreateInstance();
		static void DeleteInstance();
		static AudioSystem& GetInstance() { Assert(mInstance.get(), "create instance before using it"); return *mInstance; }

		COPY_CONSTRUCTOR(AudioSystem) = delete;
		ASSIGN_OPERATOR(AudioSystem) = delete;

		void Init();
		bool LoadFile(const char* const filename, const hashid id);
		void ReleaseAssets();

		hashid Play(const hashid id, const bool bLoop);
		void StopAllSound();
		void Stop(const hashid id);

		void Update();

	protected:
		struct SoundBuffer
		{
			hashid id = INVALID_HASH;
			SoundBufferPlatform* buffer = nullptr;
		};

	private:
		static constexpr int MAX_BUFFER = 32;

		static UniquePtr<AudioSystem> mInstance;
		
		Array<SoundBuffer, MAX_BUFFER> mSounds;
		Array<SoundBuffer, MAX_BUFFER> mSoundsPlaying;

		AudioSystem() = default;
		~AudioSystem();
	};
}
