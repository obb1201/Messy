#pragma once

#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"

namespace fm
{
	class Animation;

	class AnimationGraph final
	{
	public:
		enum eActionType
		{
			Idle,
			Walk,
			Run,

			Count,
		};

		// --- methods
		AnimationGraph() = default;
		~AnimationGraph();

		FORCEINLINE void SwitchAction(const eActionType actionType);
		FORCEINLINE void Add(Animation* animation);

		void Update(const float elapsedTime);

	private:
		Vector<Animation*, eActionType::Count> mAnimations;

		eActionType mActionType = eActionType::Idle;
		Animation* mCurrentAnimation = nullptr;
	};

	void AnimationGraph::SwitchAction(const eActionType actionType)
	{
		mActionType = actionType;
		mCurrentAnimation = mAnimations[(uint32)actionType];
	}

	void AnimationGraph::Add(Animation* animation)
	{
		mAnimations.Add(animation);
	}
}
