#include "AnimationGraph.h"
#include "Animation.h"

namespace fm
{
	AnimationGraph::~AnimationGraph()
	{
		mAnimations.RemoveAll();
	}

	void AnimationGraph::Update(const float elapsedTime)
	{
		if (mCurrentAnimation)
		{
			mCurrentAnimation->Update(elapsedTime);
		}
	}
}
