#pragma once

#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"

namespace fm
{
	class ICmpControllable : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

		virtual void SetPlayerID(uint32 id) { mPlayerID = id; };
		virtual uint32 GetPlayerID() { return mPlayerID; };
		virtual void SetBeingControlled(bool control) { mBeingControlled = control; };
		virtual bool GetBeingControlled() { return mBeingControlled; };

	protected:
		uint32 mPlayerID;
		bool mBeingControlled;

		ICmpControllable() = delete;
		ICmpControllable(eComponentType type, hashid objectName) : IComponent(type, objectName) {};

	};

	void ICmpControllable::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Controllable, compType);
	}
}
