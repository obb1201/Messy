#include "CmpPlaneCollidable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "Gameplay/Physics/PhysicsManager.h"

namespace fm
{
	const static eComponentType sType = eComponentType::PlaneCollider;
	IComponent* CmpPlaneCollidable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		ICmpCollidable* collidable = fmNew(eMemPool::Gameplay, "CmpPlaneCollidable") CmpPlaneCollidable(objectName);
		PhysicsManager::GetInstance().AddCollidable(collidable);
		return collidable;
	}

	void CmpPlaneCollidable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpPlaneCollidable::RegisterTypeInfo()
	{
		ICmpCollidable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpPlaneCollidable::CmpPlaneCollidable(hashid objectName)
		: ICmpCollidable(sType, objectName)
	{
	}

	eMessageResult CmpPlaneCollidable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpPlaneCollidable::Update(const float elaspedTime)
	{
		if (bMaster)
		{
			CheckAllConnectedCollision();
		}
	}

	void CmpPlaneCollidable::CheckCollision(ICmpCollidable* collidable2)
	{

	}
}