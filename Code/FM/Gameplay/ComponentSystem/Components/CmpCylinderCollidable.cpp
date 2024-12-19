#include "CmpCylinderCollidable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "Gameplay/Physics/PhysicsManager.h"

namespace fm
{
	const static eComponentType sType = eComponentType::CylinderCollider;
	IComponent* CmpCylinderCollidable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		ICmpCollidable* collidable = fmNew(eMemPool::Gameplay, "CylinderCollidable") CmpCylinderCollidable(objectName);
		PhysicsManager::GetInstance().AddCollidable(collidable);
		return collidable;
	}

	void CmpCylinderCollidable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpCylinderCollidable::RegisterTypeInfo()
	{
		ICmpCollidable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpCylinderCollidable::CmpCylinderCollidable(hashid objectName)
		: ICmpCollidable(sType, objectName)
	{
	}

	eMessageResult CmpCylinderCollidable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpCylinderCollidable::Update(const float elaspedTime)
	{
		if (bMaster)
		{
			CheckAllConnectedCollision();
		}
	}

	void CmpCylinderCollidable::CheckCollision(ICmpCollidable* collidable2)
	{

	}
}
