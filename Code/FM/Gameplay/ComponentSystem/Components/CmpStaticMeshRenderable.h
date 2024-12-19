#pragma once

#include "ICmpRenderable.h"

namespace fm
{
	class CmpStaticMeshRenderable : public ICmpRenderable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;
		virtual void CullAndSubmit(const ICmpCamera* currentCamera) override;

	private:
		CmpStaticMeshRenderable(hashid objectName);
		CmpStaticMeshRenderable() = delete;
	};
}