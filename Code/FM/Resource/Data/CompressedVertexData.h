#pragma once

#include "System/GlobalInclude.h"

namespace fm
{
#pragma pack(push)
#pragma pack(1)
	struct CompressedVertexData
	{
		typedef uint16 float16;
		typedef uint8 BoneIndex;
		typedef uint8 BoneWeight;

		struct Position
		{
			float16 X;
			float16 Y;
			float16 Z;
			float16 W;
		};
		static_assert(sizeof(Position) == 8, "misaligned Position struct");

		struct TexCoord
		{
			float16 U;
			float16 V;
		};
		static_assert(sizeof(TexCoord) == 4, "misaligned TexCoord struct");

		struct Normal 
		{
			int16 X;
			int16 Y;
		};
		static_assert(sizeof(Normal) == 4, "misaligned Normal struct");

		struct Tangent
		{
			int16 X;
			int16 Y;
		};
		static_assert(sizeof(Tangent) == 4, "misaligned Tangent struct");

		struct Skinning
		{
			BoneIndex Indices[4];
			BoneWeight Weights[4];
		};
		static_assert(sizeof(Skinning) == 8, "misaligned Bone struct");

		struct Stream0
		{
			Position Position;
			TexCoord TexCoord;
		};
		static_assert(sizeof(Stream0) == 12, "misaligned stream0");

		struct Stream1
		{
			Normal Normal;
			Tangent Tangent;
			Skinning Skinning;
		};
		static_assert(sizeof(Stream1) == 16, "misaligned stream1");
	};
#pragma pack(pop)
}
