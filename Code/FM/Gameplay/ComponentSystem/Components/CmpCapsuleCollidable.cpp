#include "CmpCapsuleCollidable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "Gameplay/Physics/PhysicsManager.h"

namespace fm
{
	const static eComponentType sType = eComponentType::CapsuleCollider;
	IComponent* CmpCapsuleCollidable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		ICmpCollidable* collidable = fmNew(eMemPool::Gameplay, "CylinderCollidable") CmpCapsuleCollidable(objectName);
		PhysicsManager::GetInstance().AddCollidable(collidable);
		return collidable;
	}

	void CmpCapsuleCollidable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpCapsuleCollidable::RegisterTypeInfo()
	{
		ICmpCollidable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpCapsuleCollidable::CmpCapsuleCollidable(hashid objectName)
		: ICmpCollidable(sType, objectName)
	{
	}

	eMessageResult CmpCapsuleCollidable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpCapsuleCollidable::Update(const float elaspedTime)
	{
		if (bMaster)
		{
			CheckAllConnectedCollision();
		}
	}

	void CmpCapsuleCollidable::CheckCollision(ICmpCollidable* collidable2)
	{

	}
}
