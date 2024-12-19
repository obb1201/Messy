#pragma once

#include "ICmpRenderable.h"

namespace fm
{
	class AnimationGraph;

	class CmpSkinnedMeshRenderable : public ICmpRenderable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;
		virtual void CullAndSubmit(const ICmpCamera* currentCamera) override;

		void SetAnimationGraph(AnimationGraph* animationGraph) { mAnimationGraph = animationGraph; }

	private:
		AnimationGraph* mAnimationGraph;

		// --- methods
		CmpSkinnedMeshRenderable(hashid objectName);
		CmpSkinnedMeshRenderable() = delete;
	};
}