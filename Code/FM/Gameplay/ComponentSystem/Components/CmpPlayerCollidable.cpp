#include "CmpPlayerCollidable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "Gameplay/Physics/PhysicsManager.h"

namespace fm
{
	const static eComponentType sType = eComponentType::PlayerCollider;
	IComponent* CmpPlayerCollidable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		ICmpCollidable* collidable = fmNew(eMemPool::Gameplay, "CmpPlayerCollidable") CmpPlayerCollidable(objectName);
		PhysicsManager::GetInstance().AddCollidable(collidable);
		return collidable;
	}

	void CmpPlayerCollidable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpPlayerCollidable::RegisterTypeInfo()
	{
		ICmpCollidable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpPlayerCollidable::CmpPlayerCollidable(hashid objectName)
		: ICmpCollidable(sType, objectName)
	{
	}

	eMessageResult CmpPlayerCollidable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpPlayerCollidable::Update(const float elaspedTime)
	{
		if (bMaster)
		{
			CheckAllConnectedCollision();
		}
	}

	void CmpPlayerCollidable::CheckCollision(ICmpCollidable* collidable2)
	{

	}
}