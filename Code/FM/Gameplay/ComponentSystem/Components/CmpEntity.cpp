#include "CmpEntity.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"

namespace fm
{
	const static eComponentType sType = eComponentType::Entity;

	IComponent* CmpEntity::Create(hashid objectName)
	{
		// TODO(pope) mempoo
		return fmNew(eMemPool::Gameplay, "CmpEntity") CmpEntity(objectName);
	}

	void CmpEntity::Clean(IComponent* component)
	{
		fmDelete(component);
	}
	
	void CmpEntity::RegisterTypeInfo()
	{
		ICmpEntity::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
		gom.Subscribe(sType, eMessageType::UpdatePosition);
		gom.Subscribe(sType, eMessageType::Collide);
	}

	CmpEntity::CmpEntity(hashid objectName)
		: ICmpEntity(sType, objectName)
	{
	}

	eMessageResult CmpEntity::HandleMessage(eMessageType type, const void * const params)
	{
		switch (type)
		{
		case eMessageType::UpdatePosition:
		{
			const Vector3& Position =*(Vector3*)params;
			SetPosition(mPosition + Position);
			RebuildTransform();
			break;
		}

		case eMessageType::Collide:
		{
			const CollisionInfo& info = *(CollisionInfo*)params;
			SetPosition(mPosition + info.Normal * info.DistanceToSurface);
			RebuildTransform();
			break;
		}
		}
		return eMessageResult::Ok;
	}

	void CmpEntity::Update(const float elaspedTime)
	{
		RebuildTransform();
	}

	void ICmpEntity::RebuildTransform()
	{
		if (mMatrixDirty)
		{
			mTransform.Build(mScale, mRotation, mPosition);
			mMatrixDirty = false;
		}
	}
}