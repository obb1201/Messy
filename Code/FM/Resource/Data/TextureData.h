#pragma once

#include "System/GlobalInclude.h"

#pragma pack(push)
#pragma pack(1)

namespace fm
{
	struct TextureData
	{
		enum class eFormat : uint8;

		struct Header
		{
			uint8 Version;
			eFormat Format;
			uint16 Width;
			uint16 Height;
			uint8 Depth;
			uint8 ArraySize;
			uint8 NumMips;
			uint32 Size;
			uint8 Reserved[3];
		};
		static_assert(sizeof(Header) == 16, "misaligned texture header");

		Header Header;
		uintptr Data;
	};
}
#pragma pack(pop)
