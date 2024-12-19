#include "CmpMassMovable.h"

namespace fm
{
	const static eComponentType sType = eComponentType::MassMovable;
	IComponent* CmpMassMovable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		return fmNew(eMemPool::Gameplay, "RigidbodyMovable") CmpMassMovable(objectName);
	}

	void CmpMassMovable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpMassMovable::RegisterTypeInfo()
	{
		ICmpMovable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
		gom.Subscribe(sType, eMessageType::Collide);
	}

	CmpMassMovable::CmpMassMovable(hashid objectName)
		: ICmpMovable(sType, objectName)
	{
	}

	eMessageResult CmpMassMovable::HandleMessage(eMessageType type, const void * const params)
	{
		switch (type)
		{
		case eMessageType::Collide:
		{
			mCollisionInfos.Add(*(CollisionInfo*)params);

			break;
		}
		}

		return eMessageResult::Ok;
	}

	void CmpMassMovable::Update(const float elaspedTime)
	{
		if (mCollisionInfos.GetSize() > 0)
		{
			Vector3 temp = Vector3::Zero;
			for (auto i = 0u; i < mCollisionInfos.GetSize(); ++i)
			{
				float otherMass = mCollisionInfos[i].OtherMass;

				if (otherMass <= 0.f)
				{
					temp += Vector3::Reflect(Velocity, mCollisionInfos[i].Normal) * mCollisionInfos[i].Elasticity;
				}
				else
				{
					Vector3 otherVelocity = mCollisionInfos[i].OtherVelocity;
					float elasticity = mCollisionInfos[i].Elasticity;
					temp += (Velocity * Mass + otherVelocity * otherMass - Velocity * otherMass * elasticity + otherVelocity * otherMass * elasticity) / (Mass + otherMass);
				}
			}
			Velocity = temp;
		}

		GameObjectManager& gom = GameObjectManager::GetInstance();
		// HACK
		float deltaTime = 1.0f / 60.f;
		float yVelocity = Velocity.GetY();
		Vector3 prevVelocity = Velocity;

		yVelocity -= Gravity * deltaTime;
		Velocity.SetY(yVelocity);

		Vector3 deltaPosition = (prevVelocity + Velocity) / 2.f * deltaTime;

		gom.BroadcastMessage(mObjectName, eMessageType::UpdatePosition, &deltaPosition);

		mCollisionInfos.RemoveAll();
	}
}