#pragma once

#include "Gameplay/ComponentSystem/IComponent.h"
#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "System/Math/Matrix.h"

namespace fm
{
	class ICmpCamera : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

		virtual const Matrix& GetViewProjectionMatrix() const { return Matrix::Identity; }

	protected:
		bool mViewProjectionDirty = true;

		ICmpCamera() = delete;
		ICmpCamera(eComponentType type, hashid objectName) : IComponent(type, objectName) {};
	};

	void ICmpCamera::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Camera, compType);
	}
}
