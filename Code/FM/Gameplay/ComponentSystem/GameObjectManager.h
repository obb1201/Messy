#pragma once

#include "ObjectManagerDB.h"
#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"
#include "System/UniquePtr.h"

namespace fm
{
	class ICmpEntity;
	class IComponent;

	class GameObjectManager final
	{
		FRIEND_UNIQUEPTR(GameObjectManager);
	public:  
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();
		static GameObjectManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(GameObjectManager) = delete;
		ASSIGN_OPERATOR(GameObjectManager) = delete;

		void Init();

		void RegisterComponentTypeInfo(eComponentType compType, ComponentCreator createFunc, ComponentCleaner cleaningFunc);
		void RegisterComponent(eComponentInterface interfaceID, eComponentType compType);
		
		ICmpEntity* CreateObject(hashid objectName);
		IComponent* CreateComponent(hashid objectName, eComponentType compType);

		void DeleteObject(hashid objectName);
		void DeleteComponent(hashid objectName, eComponentType compType);

		void Subscribe(eComponentType compType, eMessageType msgType);

		IComponent* QueryInterface(hashid objectName, eComponentInterface iterfaceID);

		FORCEINLINE bool Exists(hashid objectName) const;

		void BroadcastMessage(hashid objectName, eMessageType msg, const void * const params);
		void BroadcastMessage(eMessageType msg, const void * const params);

		void Update(const float elapsedTime);
		void CullAndSummit();

	private:
		static UniquePtr<GameObjectManager> mInstance;

		// members
		typedef Vector<IComponent*, ObjectManagerDB::MaxObjects> ComponentList;

		ObjectManagerDB mDatabase;
		ComponentList mEnumerators[(uint32)eComponentType::Count];

		// --- methods
		GameObjectManager() = default;
		~GameObjectManager();

		void RegisterAllComponentTypes();

		ComponentList* GetComponentList(eComponentType compType);

		// TODO(pope) this is adding to db. maybe hide it or not make it as another func?
		void AddComponent(hashid objectName, IComponent* cmp);
		void RemoveComponent(hashid objectName, IComponent* cmp);
		void Reset();

		FORCEINLINE void InvalidateEnumerator(eComponentType compType);
	};

	bool GameObjectManager::Exists(hashid objectName) const
	{
		// we only need to check Entity. Because all objects must have entity
		return mDatabase.mComponents[(uint8)eComponentType::Entity].Contains(objectName);
	}

	void GameObjectManager::InvalidateEnumerator(eComponentType compType)
	{
		Assert(compType < eComponentType::Invalid);
		mEnumerators[(uint32)compType].RemoveAll();
	}
}