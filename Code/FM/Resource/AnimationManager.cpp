#include "AnimationManager.h"
#include "Animation\Animation.h"

namespace fm
{
	UniquePtr<AnimationManager> AnimationManager::mInstance;

	void AnimationManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<AnimationManager>(fmNew(eMemPool::Resource, "model Manager instance") AnimationManager());
	}

	void AnimationManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated AnimationManager");
		mInstance.reset();
	}

	AnimationManager::~AnimationManager()
	{
	}

	Handle AnimationManager::Create(const AnimationData& data)
	{
		const uint8* cur = reinterpret_cast<const uint8*>(&data);
		cur += sizeof(AnimationData::Header);

		const auto& Header = data.Header;

		uintptr currentPosition = reinterpret_cast<uintptr>(cur);
		fixupKeyFramesOffset(currentPosition, Header.NumBones);

		Animation* animation = fmNew(eMemPool::Resource, "animation instance") Animation();
		animation->mLoopMode = (Animation::eLoopMode)Header.LoopMode;
		animation->mSkeletonType = (Animation::eSkeletonType)Header.SkeletonType;
		animation->mTotalTime = Header.TotalTime;
		mAnimations.Add(animation);

		const auto& boneTracks = reinterpret_cast<const AnimationData::KeyFrameInfo*>(cur);
		cur += sizeof(AnimationData::KeyFrameInfo) * Header.NumBones;

		for (uint32 trackIndex = 0; trackIndex < Header.NumBones; ++trackIndex)
		{
			const uint8 boneID = boneTracks[trackIndex].BoneID;
			const uint8 numKeyFrames = boneTracks[trackIndex].NumKeyFrames;
			const AnimationData::KeyFrame* keyFrames = reinterpret_cast<const AnimationData::KeyFrame*>(boneTracks[trackIndex].KeyFrameOffset);

			Animation::BoneAnimation* boneAnimation = fmNew(eMemPool::Resource, "AnimationTrack instance") Animation::BoneAnimation();
			boneAnimation->BoneID = boneID;
			boneAnimation->KeyFrameCount = 0;

			for (uint32 keyIndex = 0; keyIndex < numKeyFrames; ++keyIndex)
			{
				const AnimationData::Transformation& transformation = keyFrames[keyIndex].Transformation;

				boneAnimation->KeyFrames[keyIndex].TimeStamp = keyFrames[keyIndex].TimeStamp;
				boneAnimation->KeyFrames[keyIndex].Rotation = Quaternion(transformation.Rotation[0], transformation.Rotation[1], transformation.Rotation[2], transformation.Rotation[3]);
				boneAnimation->KeyFrames[keyIndex].Translation = Vector3(transformation.Transition[0], transformation.Transition[1], transformation.Transition[2]);
				boneAnimation->KeyFrameCount++;
			}

			animation->mBoneAnimations.Add(boneAnimation);
		}

		return mAnimations.GetSize() - 1;
	}

	void AnimationManager::Delete(const Handle handle)
	{
		Assert(handle != INVALID_HANDLE);
	}

	void AnimationManager::fixupKeyFramesOffset(const uintptr currentPosition, const uint32 numBoneTracks)
	{
		auto* boneTracks = reinterpret_cast<AnimationData::KeyFrameInfo*>(currentPosition);

		const uint8* cur = reinterpret_cast<const uint8*>(currentPosition);
		cur += sizeof(AnimationData::KeyFrameInfo) * numBoneTracks;

		const uintptr offset = reinterpret_cast<uintptr>(cur);

		for (uint32 trackIndex = 0; trackIndex < numBoneTracks; ++trackIndex)
		{
			boneTracks[trackIndex].KeyFrameOffset += offset;
		}
	}
}