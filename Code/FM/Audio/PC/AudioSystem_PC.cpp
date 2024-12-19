#include "audio/audiosystem.h"
#include <system/Assert.h>
#include <system/Hash.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>  

namespace fm
{
	void AudioSystemPC::Init()
	{
		Assert(mWindowHandle, "call AudioSystem::GetInstance().setWindowHandle() before initializing it");

		auto hr = DirectSoundCreate8(nullptr, &mDirectSoundDevice, nullptr);
		Assert(SUCCEEDED(hr), "couldn't create device blarg");

		hr = mDirectSoundDevice->SetCooperativeLevel(mWindowHandle, DSSCL_PRIORITY);
		Assert(SUCCEEDED(hr), "set cooperative levels failed");
	}

	AudioSystemPC::~AudioSystemPC()
	{
		SafeRelease(mDirectSoundDevice);
	}

	//loads ogg and id the sound with idstring's hash
	bool AudioSystem::LoadFile(const char *filename, const hashid idhash)
	{
		LPDIRECTSOUNDBUFFER getlengthbuffer;
		LPDIRECTSOUNDBUFFER soundbuffer;
		OggVorbis_File vf;
		HRESULT hr;

		// check if the id had been used before
		for (uint32 i = 0; i < mSounds.GetLength(); ++i)
		{
			if (mSounds[i].id == idhash)
			{
				Assert(false, "audio with same id already exist, or id is INVALID_HASH");
				return false;
			}
		}

		if (ov_fopen(filename, &vf) > 0)
		{
			// TODO: fm file system 
			Assert(false, "failed file loading");
			return false;
		}
		vorbis_info *vi = ov_info(&vf, -1);

		// setting up formats for creating the buffer
		WAVEFORMATEX	    wfm;
		memset(&wfm, 0, sizeof(wfm));
		wfm.cbSize = sizeof(wfm);
		wfm.nChannels = vi->channels;
		wfm.wBitsPerSample = 16;                    // ogg vorbis is always 16 bit
		wfm.nSamplesPerSec = vi->rate;
		wfm.nAvgBytesPerSec = wfm.nSamplesPerSec*wfm.nChannels * 2;
		wfm.nBlockAlign = 2 * wfm.nChannels;
		wfm.wFormatTag = 1;

		constexpr int32 DEFAULT_BUFFER_SIZE = 65536;

		DSBUFFERDESC bufferDescription;
		memset(&bufferDescription, 0, sizeof(DSBUFFERDESC));
		bufferDescription.dwSize = sizeof(DSBUFFERDESC);
		bufferDescription.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
		bufferDescription.lpwfxFormat = &wfm;
		bufferDescription.dwReserved = 0;
		bufferDescription.dwBufferBytes = DEFAULT_BUFFER_SIZE;

		//create dummy buffer
		Verify(SUCCEEDED(mDirectSoundDevice->CreateSoundBuffer(&bufferDescription, &getlengthbuffer, nullptr)));
		DWORD pos = 0;
		int32 sec = 0;
		int32 ret = 1;
		DWORD size = DEFAULT_BUFFER_SIZE;
		char *tbuf;

		//read forever till we reach eof to find the file size, you would think it would just be a field somewhere but I coudnl't find it
		Verify(SUCCEEDED(getlengthbuffer->Lock(0, size, (LPVOID*)&tbuf, &size, nullptr, nullptr, DSBLOCK_ENTIREBUFFER)));
		while (ret)
		{
			ret = ov_read(&vf, tbuf, size, 0, 2, 1, &sec);
			pos += ret;
		}
		Verify(SUCCEEDED(getlengthbuffer->Unlock(tbuf, size, nullptr, NULL)));
		// get rid of the dummy buffer
		SafeRelease(getlengthbuffer);

		bufferDescription.dwBufferBytes = pos;
		ov_pcm_seek(&vf, 0);

		//create the real buffer and load it with the data
		hr = mDirectSoundDevice->CreateSoundBuffer(&bufferDescription, &soundbuffer, nullptr);
		Assert(SUCCEEDED(hr), "buffer create failed");
		pos = 0;
		sec = 0;
		ret = 1;
		size = bufferDescription.dwBufferBytes;
		char *buf;
		Verify(SUCCEEDED(soundbuffer->Lock(0, size, (LPVOID*)&buf, &size, nullptr, nullptr, DSBLOCK_ENTIREBUFFER)));

		while (ret && pos < size)
		{
			ret = ov_read(&vf, buf + pos, size - pos, 0, 2, 1, &sec);
			pos += ret;
		}
		Verify(SUCCEEDED(soundbuffer->Unlock(buf, size, nullptr, NULL)));

		// done with reading closing the vorbis file
		ov_clear(&vf);

		for (uint32 i = 0; i < mSounds.GetLength(); ++i)
		{
			auto& sound = mSounds[i];
			if (sound.id == INVALID_HASH)
			{
				sound.id = idhash;
				sound.buffer = soundbuffer;

				return true;
			}
		}
		Assert(false, "exceeded max sound limit");
		return false;
	}

	// find the id audio, create a copy and plays it.
	// returns the playing audio instance id
	hashid AudioSystem::Play(const hashid id, const bool loop)
	{
		for (uint32 i = 0; i < mSounds.GetLength(); ++i)
		{
			auto& sound = mSounds[i];

			if (sound.id != id)
			{
				continue;
			}

			for (uint32 j = 0; j < mSoundsPlaying.GetLength(); ++j)
			{
				auto& targetBuffer = mSoundsPlaying[j];
				if (targetBuffer.id != INVALID_HASH)
				{
					continue;
				}

				targetBuffer.id = i + 1;

				LPDIRECTSOUNDBUFFER CopyBuffer;
				Verify(SUCCEEDED(mDirectSoundDevice->DuplicateSoundBuffer(sound.buffer, &CopyBuffer)));
				targetBuffer.buffer = CopyBuffer;
				targetBuffer.buffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);

				return targetBuffer.id;
			}

			Assert(false, "no free buffer found?");
		}
		Assert(false, "no audio found");
		
		return INVALID_HASH;
	}

	void AudioSystem::StopAllSound()
	{
		for (int32 i = ToSigned(mSoundsPlaying.GetLength()) - 1; i >= 0; --i)
		{
			SoundBuffer& playingSound = mSoundsPlaying[i];
			if (playingSound.buffer != nullptr)
			{
				playingSound.buffer->Stop();
				SafeRelease(playingSound.buffer);
			}

			playingSound.id = INVALID_HASH;
		}
	}

	// the id is the sound instance id 
	void AudioSystem::Stop(const hashid id)
	{
		Assert(id != INVALID_HASH, "hash id invalid");
		Assert(id <= mSoundsPlaying.GetLength(), "index out of bound, or id is invalid some how");

		SoundBuffer& playingSound = mSoundsPlaying[(int32)(id - 1)];
		Assert(playingSound.buffer, "a ided instance held a nullpointer buffer");

		playingSound.buffer->Stop();
		SafeRelease(playingSound.buffer);
		playingSound.id = INVALID_HASH;
	}

	// check for finished sound and remove them from the list
	void AudioSystem::Update()
	{
		for (int32 i = ToSigned(mSoundsPlaying.GetLength()) - 1; i >= 0; --i)
		{
			SoundBuffer& playingSound = mSoundsPlaying[i];
			if (playingSound.id == INVALID_HASH)
			{
				continue;
			}

			DWORD status;

			Verify(!FAILED(playingSound.buffer->GetStatus(&status)));
			if ((status & DSBSTATUS_PLAYING) == 0)
			{
				SafeRelease(playingSound.buffer);
				playingSound.id = INVALID_HASH;
			}
		}
	}

	void AudioSystem::ReleaseAssets()
	{
		for (auto i = 0u; i < mSounds.GetLength(); ++i)
		{
			SoundBuffer& sound = mSounds[i];
			if (sound.id != INVALID_HASH)
			{
				// mSounds should never be playing, so we shouldn't have to stop it first
				SafeRelease(sound.buffer);
				sound.id = INVALID_HASH;
			}
		}
	}
}