#pragma once

#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"

namespace fm
{
	class Model;
	class ICmpCamera;

	class ICmpRenderable : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

		FORCEINLINE void SetModel(Model* model) { mModel = model; }
		FORCEINLINE const Model* GetModel() const { return mModel; }

		virtual void CullAndSubmit(const ICmpCamera* currentCamera) {}

	protected:
		Model* mModel;

		ICmpRenderable() = delete;
		ICmpRenderable(eComponentType type, hashid objectName) : IComponent(type, objectName) {};
	};

	void ICmpRenderable::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Renderable, compType);
	}
}
