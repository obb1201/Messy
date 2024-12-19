#pragma once

#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"
#include "System/Container/Vector.h"

#include "Data/AnimationData.h"

namespace fm
{
	class Animation;

	class AnimationManager final
	{
		FRIEND_UNIQUEPTR(AnimationManager);
	public:
		Handle Create(const AnimationData& data);
		void Delete(const Handle handle);

		FORCEINLINE Animation* Get(const Handle handle);

		// -- singleton related
		static void CreateInstance();
		static void DeleteInstance();

		static AnimationManager& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }

		COPY_CONSTRUCTOR(AnimationManager) = delete;
		ASSIGN_OPERATOR(AnimationManager) = delete;

	private:
		static UniquePtr<AnimationManager> mInstance;

		static const uint32 MAX_ANIMATIONS = 1024;
		Vector<Animation*, MAX_ANIMATIONS> mAnimations;

		// --- methods
		AnimationManager() = default;
		~AnimationManager();

		void fixupKeyFramesOffset(const uintptr currentPosition, const uint32 numBoneTracks);
	};

	Animation* AnimationManager::Get(const Handle handle)
	{
		Assert(handle != INVALID_HANDLE);

		const uint32 index = handle;
		return mAnimations[index];
	}
}