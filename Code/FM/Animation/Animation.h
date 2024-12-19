#pragma once

#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"
#include "System/Math/Quaternion.h"

#include "Skeleton.h"
#include "AnimationDefine.h"

#include "Resource/Data/AnimationData.h"

namespace fm
{
	class Animation final
	{
		friend class AnimationManager;
	public:
		enum eLoopMode : uint8
		{
			Loop,
		};

		enum eSkeletonType : uint8
		{
		};

		void Update(const float elapsedTime);
		
		FORCEINLINE void SetSkeleton(Skeleton* skeleton);

	private:
		static const uint32 MAX_KEYFRAMES = 128;

		struct KeyFrame
		{
			float TimeStamp;
			Quaternion Rotation;
			Vector3 Translation;
		};

		struct BoneAnimation
		{
			uint8 BoneID;
			uint8 KeyFrameCount;
			Array<KeyFrame, MAX_KEYFRAMES> KeyFrames;
		};

		Vector<BoneAnimation*, MAXIMUM_BONES> mBoneAnimations;

		hashid mName;
		eLoopMode mLoopMode;
		eSkeletonType mSkeletonType;

		float mTotalTime = 0.0f;
		float mCurrentTime = 0.0f;

		Skeleton* mSkeleton = nullptr;

		// --- methods
		Animation() = default;
		~Animation();

		Animation::KeyFrame interpolate(const BoneAnimation* boneAnimation, const float time);
	};

	void Animation::SetSkeleton(Skeleton* skeleton)
	{
		Assert(mSkeleton->mBones.GetSize() == mBoneAnimations.GetSize(), "mismatcheed bone numbers");

		mSkeleton = skeleton;
	}
}
