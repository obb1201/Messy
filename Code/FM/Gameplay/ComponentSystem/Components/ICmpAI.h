#pragma once

#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"

namespace fm
{
	class ICmpAI : public IComponent
	{
	protected:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

	protected:
		ICmpAI() = delete;
		ICmpAI(eComponentType type, hashid objectName) : IComponent(type, objectName) {};

	};

	void ICmpAI::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::AI, compType);
	}
}
