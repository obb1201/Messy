#pragma once

#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"
#include "Gameplay/Physics/Collidable.h"
#include "System/Container/Vector.h"

namespace fm
{
	class ICmpCollidable : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);
		
		float Elasticity = 0.5f;	// from 0 to 1
		SphereCollidable RoughData;
		bool bMaster = false;
		bool bConnected = false;

		FORCEINLINE void AddConnectedList(ICmpCollidable* collidable);
		FORCEINLINE void ResetConnectedList();

	protected:
		ICmpCollidable() = delete;
		ICmpCollidable(eComponentType type, hashid objectName) : IComponent(type, objectName) {};

		static constexpr int MAX_INPUT = 32;
		Vector<ICmpCollidable*, MAX_INPUT> mConnectedList;

		void CheckAllConnectedCollision();
		virtual void CheckCollision(ICmpCollidable* collidable2) = 0;
	};

	void ICmpCollidable::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Collidable, compType);
	}

	void ICmpCollidable::AddConnectedList(ICmpCollidable* collidable)
	{
		mConnectedList.Add(collidable);
	}

	void ICmpCollidable::ResetConnectedList()
	{
		bConnected = false;
		bMaster = false;
		mConnectedList.RemoveAll();
	}
}
