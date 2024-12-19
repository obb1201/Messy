#pragma once

#include "system/globalinclude.h"
#include "system/assert.h"
#include "system/hash.h"
#include "GameObjectManager.h"

namespace fm
{
	class ICmpEntity;

	class IComponent
	{
	public:
		FORCEINLINE hashid GetObjectName() const { return mObjectName; }
		FORCEINLINE ICmpEntity* GetEntity() const;
		FORCEINLINE eComponentType GetType() const;

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) { return eMessageResult::Ok; }
		virtual void Update(const float elaspedTime) {}

	protected:
		hashid mObjectName;
		eComponentType mType;

		IComponent() = delete;
		IComponent(eComponentType type, hashid objectName) : mType(type), mObjectName(objectName) {};
	};

	eComponentType IComponent::GetType() const
	{
		Assert(mType < eComponentType::Count, "set type in your component's init function");
		return mType;
	}

	ICmpEntity* IComponent::GetEntity() const
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		return (ICmpEntity*)gom.QueryInterface(mObjectName, eComponentInterface::Entity);
	}
}