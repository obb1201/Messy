#pragma once

#include "Graphics/Texture_platform.h"
#include "System/GlobalInclude.h"
#include "ShaderReourceView.h"

namespace fm
{
	class Texture final
	{
		friend class TextureManager;
	public:
		enum class eFormat : uint8
		{
			DXT1,
			DXT3,
			DXT5,
		};

	private:
		eFormat mFormat;
		uint16 mWidth = 0;
		uint16 mHeight = 0;
		uint8 mDepth = 0;
		uint8 mArraySize = 0;
		uint8 mNumMips = 0;

		Texture2DPlatform* mTexture = nullptr;
		ShaderResourceViewPlatform* mTextureView = nullptr;

		Texture(const uint8* surfacedata, const eFormat format, const uint16 width, const uint16 height, 
			const uint8 depth, const uint8 arraysize, const uint8 numMips);
		~Texture();

		const uint32 getBlockByte(const eFormat format) const;
	};
}
