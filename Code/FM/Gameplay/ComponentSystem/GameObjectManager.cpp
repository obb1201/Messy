#include "GameObjectManager.h"
#include "Gameplay/ComponentSystem/IComponent.h"
#include "Graphics/Renderer.h"

#include "Components/CmpEntity.h"
#include "Components/CmpCamera.h"
#include "Components/CmpStaticMeshRenderable.h"
#include "Components/CmpSkinnedMeshRenderable.h"
#include "Components/CmpMassMovable.h"
#include "Components/CmpCapsuleCollidable.h"
#include "Components/CmpCylinderCollidable.h"
#include "Components/CmpPlaneCollidable.h"
#include "Components/CmpPlayerCollidable.h"
#include "Components/CmpSphereCollidable.h"

#include "System/Threading/WorkerThreadManager.h"

namespace fm
{
	UniquePtr<GameObjectManager> GameObjectManager::mInstance;

	void GameObjectManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<GameObjectManager>(fmNew(eMemPool::Gameplay, "GameState Manager instance") GameObjectManager());
	}

	void GameObjectManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated GameObjectManager");
		mInstance = nullptr;
	}

	GameObjectManager::~GameObjectManager()
	{
		ComponentList list;
		for (uint32 i = 0; i < StaticArraySize(mDatabase.mComponents); ++i)
		{
			list.RemoveAll();

			const uint32 size = mDatabase.mComponents[i].GetSize();
			if (size > 0)
			{
				mDatabase.mComponents[i].GetValues(list);
			}

			for (uint32 j = 0; j < list.GetSize(); ++j)
			{
				mDatabase.mComponentTypeInfos[i].cleaner(list[j]);
			}

			mDatabase.mComponents[i].RemoveAll();
		}

		Reset();
	}

	void GameObjectManager::Init()
	{
		RegisterAllComponentTypes();
	}

	void GameObjectManager::RemoveComponent(hashid objectName, IComponent* component)
	{
		// TODO(pope) : make this happen!
		// remove then call clean method!
		const auto type = component->GetType();
		mDatabase.mComponentTypeInfos[(uint32)type].cleaner(component);
		mDatabase.mComponents[(uint32)type].Remove(objectName);
		InvalidateEnumerator(type);
	}

	void GameObjectManager::Reset()
	{
		mDatabase.Reset();

		//Invalidate all enumerators
		for (auto& e : mEnumerators)
		{
			e.RemoveAll();
		}
	}	

	void GameObjectManager::RegisterAllComponentTypes()
	{
		CmpEntity::RegisterTypeInfo();
		CmpCamera::RegisterTypeInfo();
		CmpStaticMeshRenderable::RegisterTypeInfo();
		CmpSkinnedMeshRenderable::RegisterTypeInfo();
		CmpMassMovable::RegisterTypeInfo();
		CmpCapsuleCollidable::RegisterTypeInfo();
		CmpCylinderCollidable::RegisterTypeInfo();
		CmpPlaneCollidable::RegisterTypeInfo();
		CmpSphereCollidable::RegisterTypeInfo();
		CmpPlayerCollidable::RegisterTypeInfo();

#if 0
		// Lights
		CmpLightPoint.resgisterTypeInfo();
		CmpLightDirectional.resgisterTypeInfo();

		CmpRenderable.resgisterTypeInfo();
		CmpRenderableAvatar.resgisterTypeInfo();
		CmpMovable.resgisterTypeInfo();
		CmpAIChase.resgisterTypeInfo();
		CmpAIRunaway.resgisterTypeInfo();
		CmpCollidablePushback.resgisterTypeInfo();
		CmpCollidableDisappear.resgisterTypeInfo();
		CmpCollidableBounce.resgisterTypeInfo();
		CmpControllable.resgisterTypeInfo();
		CmpPlayerDescription.resgisterTypeInfo();
#endif
	}


#if 0
	IComponent* GameObjectManager::createComponent(hashid objectName, XmlReader node)
	{
		string componentClassString = node.GetAttribute("Type");
		componentClassString = componentClassString.Substring(componentClassString.LastIndexOf(":Cmp") + 4);
		ComponentType type_id = (ComponentType)Enum.Parse(typeof(ComponentType), componentClassString, true);

		Debug.Assert(mDatabase.ComponentTypeInfos[(int)type_id].mCreator != null, "Creation method is not there");


		IComponent comp = mDatabase.ComponentTypeInfos[(int)type_id].mCreator(object_name_hash);

		if (comp != null)
		{
			comp.Init(node);
		}

		return comp;
	}
#endif

	void GameObjectManager::RegisterComponentTypeInfo(eComponentType compType, ComponentCreator createFunc, ComponentCleaner cleaningFunc)
	{
		auto& info = mDatabase.mComponentTypeInfos[(int)compType];
		info.creator = createFunc;
		info.cleaner = cleaningFunc;
	}

	void GameObjectManager::RegisterComponent(eComponentInterface interfaceID, eComponentType compType)
	{
		int i;
		for (i = 0; i < ObjectManagerDB::MaxCompTypesPerInterface; ++i)
		{
			auto& type = mDatabase.mComponentTypes[(uint8)interfaceID][i];
			Assert(type != compType, "don't put same components twice");
			if (type == eComponentType::Invalid)
			{
				type = compType;
				break;
			}
		}
		Assert(i < ObjectManagerDB::MaxCompTypesPerInterface, "increase MaxCompTypesPerInterfaceId");
	}

	void GameObjectManager::Subscribe(eComponentType compType, eMessageType msgType)
	{
		mDatabase.mSubscribedComponentTypes[(uint8)msgType].SetBit((uint8)compType);
	}

	void GameObjectManager::AddComponent(hashid objectName, IComponent* component)
	{
		const auto type = component->GetType();
		mDatabase.mComponents[(uint8)type].Put(objectName, component);
		InvalidateEnumerator(type);
	}
	
	IComponent* GameObjectManager::QueryInterface(hashid objectName, eComponentInterface interfaceID)
	{
		IComponent* ret = nullptr;

		for (auto& compType : mDatabase.mComponentTypes[(uint8)interfaceID])
		{
			if (compType == eComponentType::Invalid)
			{
				break;
			}

			auto& compMap = mDatabase.mComponents[(uint8)compType];

			if (compMap.Contains(objectName))
			{
				if (ret = compMap.Get(objectName))
				{
					break;
				}
			}
		}
		return ret;
	}

	GameObjectManager::ComponentList* GameObjectManager::GetComponentList(eComponentType compType)
	{
		Assert(compType < eComponentType::Count);
		
		auto& list = mEnumerators[(uint32)compType];
		if (list.GetSize() == 0)
		{
			auto& hashmap = mDatabase.mComponents[(uint32)compType];
			const auto size = hashmap.GetSize();
			if (size > 0)
			{
				list.RemoveAll();
				hashmap.GetValues(list);
			}
		}
		return &list;
	}

#if 0
	List<IDictionaryEnumerator> GameObjectManager::getComponents(InterfaceID interface_id)
	{
		List<IDictionaryEnumerator> enumerator_list = new List<IDictionaryEnumerator>();

		// Create a list of all component types associated with given InterfaceID
		List<ComponentType> cmp_type_list = new List<ComponentType>();
		for (int i = 0; i < mDatabase.MaxComponentTypesPerInterfaceID; ++i)
		{
			if (ComponentType.Invalid != mDatabase.InterfaceIDToComponentTypes[(int)interface_id, i])
				cmp_type_list.Add(mDatabase.InterfaceIDToComponentTypes[(int)interface_id, i]);
		}

		//iterate through each component type
		for (int i = 0; i < cmp_type_list.Count; ++i)
		{
			int index = (int)cmp_type_list[i];

			if (mComponentsByComponentTypeEnumerators[index] == null)
			{
				mComponentsByComponentTypeEnumerators[index] = mDatabase.ComponentTypeToComponents[index].GetEnumerator();
			}
			mComponentsByComponentTypeEnumerators[(int)cmp_type_list[i]].Reset();

			enumerator_list.Add(mComponentsByComponentTypeEnumerators[(int)cmp_type_list[i]]);
		}
		return enumerator_list;
	}
#endif

	void GameObjectManager::DeleteObject(hashid objectName)
	{
		for (auto i = 0; i < (uint32)eComponentType::Count; ++i)
		{
			DeleteComponent(objectName, (eComponentType)i);
		}

#if 0
		List<IDictionaryEnumerator> iter = GetComponents(InterfaceID.AI);
		for (int b = 0; b < iter.Count; b++)
		{
			while (iter[b].MoveNext())
			{
				if (((ICmpAI)iter[b].Value).TargetName == ObjectName)
				{
					((ICmpAI)iter[b].Value).TargetName = "";
				}
			}
		}
#endif
	}

	void GameObjectManager::DeleteComponent(hashid objectName, eComponentType compType)
	{
		if (!mDatabase.mComponents[(uint32)compType].Contains(objectName))
		{
			return;
		}

		IComponent* component = mDatabase.mComponents[(uint32)compType].Get(objectName);
		mDatabase.mComponentTypeInfos[(uint32)compType].cleaner(component);
		mDatabase.mComponents[(uint32)compType].Remove(objectName);
		InvalidateEnumerator(compType);
	}

	ICmpEntity* GameObjectManager::CreateObject(hashid objectName)
	{
		Assert(!Exists(objectName), "object with same name already exists");

		return (ICmpEntity*)CreateComponent(objectName, eComponentType::Entity);
	}

	IComponent* GameObjectManager::CreateComponent(hashid objectName, eComponentType compType)
	{
		const ComponentList* cmpList = GetComponentList(compType);
		const uint32 count = cmpList->GetSize();
		for (uint32 i = 0u; i < count; ++i)
		{
			const auto* component = (*cmpList)[i];
			Assert(component->GetObjectName() != objectName);
		}

		IComponent* component = mDatabase.mComponentTypeInfos[(uint32)compType].creator(objectName);

		AddComponent(objectName, component);

		return component;
	}

	void GameObjectManager::BroadcastMessage(hashid objectName, eMessageType msg, const void * const params)
	{
		const auto& subscription = mDatabase.mSubscribedComponentTypes[(uint32)msg];
		// find the component types that subscribe to this message
		for (auto i = 0u; i < (uint32)eComponentType::Count; ++i)
		{
			const auto subscribed = subscription.IsSetBit(i);

			if (subscribed)
			{
				auto * iter = GetComponentList((eComponentType)i);
				const auto count = iter->GetSize();
				for (auto i = 0u; i < count; ++i)
				{
					auto* comp = (*iter)[i];
					if (comp->GetObjectName() == objectName)
					{
						comp->HandleMessage(msg, params);
					}
				}
			}
		}
	}

	void GameObjectManager::BroadcastMessage(eMessageType msg, const void * const params)
	{
		const auto& subscription = mDatabase.mSubscribedComponentTypes[(uint32)msg];
		// find the component types that subscribe to this message
		
		for (auto i = 0u; i < (uint32)eComponentType::Count; ++i)
		{
			// if a component subscribed to this msg
			if (subscription.IsSetBit(i))
			{
				auto* iter = GetComponentList((eComponentType)i);
				const auto count = iter->GetSize();
				for (auto i = 0u; i < count; ++i)
				{
					auto* comp = (*iter)[i];
					comp->HandleMessage(msg, params);
				}
			}
		}
	}

	void GameObjectManager::Update(const float elapsedTime)
	{
		WorkerThreadManager& workerManager = WorkerThreadManager::GetInstance();

		for (auto i = 0u; i < (uint32)eComponentType::Count; ++i)
		{
			//HACK : this code will be changed and make it fast for getting component list. 
			auto* iter = GetComponentList((eComponentType)i);
			const auto count = iter->GetSize();
			for (auto i = 0u; i < count; ++i)
			{
				IComponent* comp = (*iter)[i];

				Job job;
				job.Object = comp;
				job.ElapsedTime = elapsedTime;
				job.Type = comp->GetType();
				job.Update = JobUpdate;

				//It would fine to allow to copy jobs like this.
				workerManager.AddJob(job);
			}
		}
	}

	void GameObjectManager::CullAndSummit()
	{
		// TODO : get skeletalmesh from component list;
		const ComponentList* renderableList = GetComponentList(eComponentType::StaticMesh);
		const ComponentList* cameraList = GetComponentList(eComponentType::Camera);

		// HACK : camera would have more than two. 
		const ICmpCamera* currentCamera = (const ICmpCamera*)(*cameraList)[0];

		Renderer::GetInstance().SetViewProjectMatrix(currentCamera->GetViewProjectionMatrix());

		const uint32 count = renderableList->GetSize();
		for (uint32 i = 0; i < count; ++i)
		{
			ICmpRenderable* comp = (ICmpRenderable*)(*renderableList)[i];

			comp->CullAndSubmit(currentCamera);
		}
	}
}

