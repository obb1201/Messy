#pragma once

#include "System/Hash.h"
#include "System/GlobalInclude.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

#pragma pack(push)
#pragma pack(1)

namespace fm
{
	struct SkeletonData
	{
		struct Header
		{
			uint8 Version;
			uint8 NumBones;
			uint8 Reserved[6];
		};
		static_assert(sizeof(Header) == 8, "misaligned Skeleton data");

		struct Bone
		{
			uint8 ID;
			uint8 ParentID;
		};
		static_assert(sizeof(Bone) == 2, "misaligned Bone");

		struct BoneTransform
		{
			Vector4 Rotation;
			Vector3 Translation;
		};
		static_assert(sizeof(BoneTransform) == 32, "misaligned BoneTransform"); // sizeof(Vector3) = 16

		struct BoneTable
		{
			hashid* Names;					// hashid[NumBones]
			Bone* Bones;					// Bone[NumBones]
			BoneTransform* BoneTransforms;	// BoneTransform[NumBones]
		};
		static_assert(sizeof(BoneTable) == 24, "misaligned Bone table");

		Header Header;
		BoneTable BoneTable;
	};
}
#pragma pack(pop)
