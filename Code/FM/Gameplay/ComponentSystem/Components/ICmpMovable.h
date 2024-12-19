#pragma once

#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"
#include "system/math/vector3.h"

namespace fm
{
	class ICmpMovable : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

		Vector3 Velocity = Vector3::Zero;

	protected:

		ICmpMovable() = delete;
		ICmpMovable(eComponentType type, hashid objectName) : IComponent(type, objectName) {};

	};

	void ICmpMovable::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Movable, compType);
	}
}