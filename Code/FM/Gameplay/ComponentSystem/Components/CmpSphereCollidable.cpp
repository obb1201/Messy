#include "CmpSphereCollidable.h"
#include "CmpEntity.h"
#include "CmpMassMovable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "Gameplay/Physics/PhysicsManager.h"

namespace fm
{
	const static eComponentType sType = eComponentType::SphereCollider;
	IComponent* CmpSphereCollidable::Create(hashid objectName)
	{
		// TODO(pope) mempool
		ICmpCollidable* collidable = fmNew(eMemPool::Gameplay, "CmpSphereCollidable") CmpSphereCollidable(objectName);
		PhysicsManager::GetInstance().AddCollidable(collidable);
		return collidable;
	}

	void CmpSphereCollidable::Clean(IComponent* const component)
	{
		fmDelete(component);
	}

	void CmpSphereCollidable::RegisterTypeInfo()
	{
		ICmpCollidable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpSphereCollidable::CmpSphereCollidable(hashid objectName)
		: ICmpCollidable(sType, objectName)
	{
	}

	eMessageResult CmpSphereCollidable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpSphereCollidable::Update(const float elaspedTime)
	{
		if (bMaster)
		{
			CheckAllConnectedCollision();
		}

		GameObjectManager& gom = GameObjectManager::GetInstance();
		CmpEntity* entity = (CmpEntity*)gom.QueryInterface(mObjectName, eComponentInterface::Entity);
		Data.Position = entity->GetPosition();
		RoughData.Position = Data.Position;

		if (Data.Position.GetY() - Data.Radius <= 0.f)
		{
			CollisionInfo info;
			info.Normal.Set(0.f, 1.f, 0.f); // normal
			info.Position.Set(Data.Position.GetX(), 0.f, Data.Position.GetZ()); // position
			info.Elasticity = Elasticity;
			info.DistanceToSurface = -Data.Position.GetY() + Data.Radius;
			info.OtherMass = 0.f;

			gom.BroadcastMessage(mObjectName, eMessageType::Collide, &info);
		}
	}

	void CmpSphereCollidable::CheckCollision(ICmpCollidable* collidable2)
	{
		switch (collidable2->GetType())
		{
		case eComponentType::SphereCollider:
		{
			// TODO
			// make it common function for collidables
			CmpSphereCollidable* sphereCollidable = (CmpSphereCollidable*)collidable2;
			float distanceSquare = Vector3::DistanceSquared(Data.Position, sphereCollidable->Data.Position);
			float radius = Data.Radius + sphereCollidable->Data.Radius;
			if (distanceSquare <= radius * radius)
			{
				GameObjectManager& gom = GameObjectManager::GetInstance();
				CmpMassMovable* myMovable = (CmpMassMovable*)gom.QueryInterface(mObjectName, eComponentInterface::Movable);
				CmpMassMovable* otherMovable = (CmpMassMovable*)gom.QueryInterface(sphereCollidable->GetObjectName(), eComponentInterface::Movable);
				CollisionInfo myInfo;
				myInfo.Normal = Data.Position - sphereCollidable->Data.Position;
				myInfo.Normal = myInfo.Normal.Normalize();
				myInfo.DistanceToSurface = (radius - sqrtf(distanceSquare)) * 2.f;
				myInfo.Position = Data.Position + (myInfo.Normal * -(Data.Radius - myInfo.DistanceToSurface));
				myInfo.Elasticity = Elasticity * sphereCollidable->Elasticity;
				myInfo.OtherMass = otherMovable->Mass;
				myInfo.OtherVelocity = otherMovable->Velocity;
				gom.BroadcastMessage(mObjectName, eMessageType::Collide, &myInfo);

				CollisionInfo otherInfo;
				otherInfo.Normal = myInfo.Normal * -1.f;
				otherInfo.Normal = otherInfo.Normal.Normalize();
				otherInfo.DistanceToSurface = myInfo.DistanceToSurface;
				otherInfo.Position = myInfo.Position;
				otherInfo.Elasticity = myInfo.Elasticity;
				otherInfo.OtherMass = myMovable->Mass;
				otherInfo.OtherVelocity = myMovable->Velocity;
				gom.BroadcastMessage(sphereCollidable->GetObjectName(), eMessageType::Collide, &otherInfo);
			}
			break;
		}
		default:
			break;
		}
	}
}