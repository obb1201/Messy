#pragma once

#include "System/Hash.h"
#include "System/GlobalInclude.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

#pragma pack(push)
#pragma pack(1)

namespace fm
{
	struct AnimationData
	{
		enum eLoopMode : uint8;
		enum eSkeletonType : uint8;

		struct Header
		{
			uint8 Version;
			eLoopMode LoopMode;
			eSkeletonType SkeletonType;
			uint8 NumBones;
			float TotalTime;
			hashid Name;
		};
		static_assert(sizeof(Header) == 16, "misaligned Header");

		struct Transformation
		{
			float Rotation[4];
			float Transition[3];
			uint8 Reserved[4];
		};
		static_assert(sizeof(Transformation) == 32, "misaligned Transformation");

		struct KeyFrame
		{
			float TimeStamp;
			Transformation Transformation;
			uint8 Reserved[4];
		};
		static_assert(sizeof(KeyFrame) == 40, "misaligned KeyFrame");

		struct KeyFrameInfo
		{
			uint8 BoneID;
			uint8 NumKeyFrames;
			uintptr KeyFrameOffset;
			uint8 Reserved[6];
		};
		static_assert(sizeof(KeyFrameInfo) == 16, "misaligned KeyFrameInfo");

		Header Header;

		// --- example of Animation Data File Structure ---
		// Header
		// KeyFrameInfo[NumBones]
		// KeyFrame - bone1
		// ...
		// KeyFrame - bone2
		// ...
	};
}
#pragma pack(pop)
