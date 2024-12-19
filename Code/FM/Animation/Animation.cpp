#include "Animation.h"

namespace fm
{
	Animation::~Animation()
	{
	}

	void Animation::Update(const float elapsedTime)
	{
		mCurrentTime += elapsedTime;

		if (mCurrentTime > mTotalTime)
		{
			mCurrentTime = 0.0f;
		}

		Vector3 scale = Vector3::One;

		for (uint32 i = 0; i < mBoneAnimations.GetSize(); ++i)
		{
			const uint8 boneIndex = mBoneAnimations[i]->BoneID;

			Animation::KeyFrame newkeyFrame = interpolate(mBoneAnimations[i], mCurrentTime);

			Matrix& matrix = mSkeleton->mFinalBoneTransforms[boneIndex];
			matrix.Build(scale, newkeyFrame.Rotation, newkeyFrame.Translation);

			const uint8 parentIndex = mSkeleton->mBones[boneIndex].ParentID;
			if (mSkeleton->mBones[boneIndex].ParentID != uint8(INVALID_INDEX))
			{
				mSkeleton->mFinalBoneTransforms[boneIndex] = matrix * mSkeleton->mFinalBoneTransforms[parentIndex];
			}
		}

		for (uint32 i = 0; i < mBoneAnimations.GetSize(); ++i)
		{
			const uint8 boneIndex = mBoneAnimations[i]->BoneID;
			mSkeleton->mFinalBoneTransforms[boneIndex] *= mSkeleton->mBones[boneIndex].OffsetTransform;
		}
	}

	Animation::KeyFrame Animation::interpolate(const BoneAnimation* boneAnimation, const float time)
	{
		Animation::KeyFrame newKeyFrame;

		const Array<KeyFrame, MAX_KEYFRAMES>& keyFrames = boneAnimation->KeyFrames;
		const uint32 lastIndex = boneAnimation->KeyFrameCount - 1;

		if (time <= keyFrames[0].TimeStamp)
		{
			newKeyFrame.Rotation = keyFrames[0].Rotation;
			newKeyFrame.Translation = keyFrames[0].Translation;
		}
		else if (time >= keyFrames[lastIndex].TimeStamp)
		{
			newKeyFrame.Rotation = keyFrames[lastIndex].Rotation;
			newKeyFrame.Translation = keyFrames[lastIndex].Translation;
		}
		else
		{
			for (uint32 keyIndex = 0; keyIndex < lastIndex; ++keyIndex)
			{
				if (time >= keyFrames[keyIndex].TimeStamp && time <= keyFrames[keyIndex + 1].TimeStamp)
				{
					const float weight = (time - keyFrames[keyIndex].TimeStamp) / (keyFrames[keyIndex + 1].TimeStamp - keyFrames[keyIndex].TimeStamp);

					const Animation::KeyFrame& currentKey = keyFrames[keyIndex];
					const Animation::KeyFrame& nextKey = keyFrames[keyIndex + 1];

					newKeyFrame.Rotation = Quaternion::SphericalLerp(currentKey.Rotation, nextKey.Rotation, weight);
					newKeyFrame.Translation = Vector3::Lerp(currentKey.Translation, nextKey.Translation, weight);

					break;
				}
			}
		}

		return newKeyFrame;
	}
}
