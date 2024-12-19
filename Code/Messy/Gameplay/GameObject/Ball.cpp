
#include "Ball.h"

#include "Gameplay/ComponentSystem//Components/CmpEntity.h"
#include "Gameplay/ComponentSystem/Components/CmpMassMovable.h"
#include "Gameplay/ComponentSystem/Components/CmpSphereCollidable.h"
#include "Gameplay/ComponentSystem/Components/ICmpRenderable.h"

#include "Controller/ControllerManager.h"

#include "Gameplay/GameDefine.h"

// HACK
#include "Resource/ModelManager.h"
#include "System/File/PackageManager.h"
#include "System/File/FileType.h"

namespace fm
{
	void Ball::Initialize(const Vector3& initPosition, const char* ballName)
	{
		mInitPosition = initPosition;
		mObjectID = HashString(ballName);

		GameObjectManager& gom = GameObjectManager::GetInstance();
		CmpEntity* entity = (CmpEntity*)gom.CreateObject(mObjectID);
		entity->SetPosition(initPosition);
		entity->SetScale(Vector3(0.2f, 0.2f, 0.2f));

		// HACK
		// TODO initial ball using ball data
		CmpSphereCollidable* sphereCollidale = (CmpSphereCollidable*)gom.CreateComponent(mObjectID, eComponentType::SphereCollider);
		sphereCollidale->Data.Radius = 1.f;
		sphereCollidale->Data.Position = initPosition;
		sphereCollidale->Elasticity = 0.9f;
		sphereCollidale->RoughData.Radius = 1.f;

		CmpMassMovable* massMovable = (CmpMassMovable*)gom.CreateComponent(mObjectID, eComponentType::MassMovable);
		massMovable->Velocity = Vector3::Zero;
		massMovable->Gravity = 9.8f;
		massMovable->Mass = 1.f;

		// HACK : sorry, kill me
		mPackageHandle = PackageManager::GetInstance().RequestLoad("debugdata/ball.pak");
	}

	// HACK - test
	void Ball::Update()
	{
		ControllerManager& controlmanager = ControllerManager::GetInstance();
		GameObjectManager& gom = GameObjectManager::GetInstance();
		CmpEntity* entity = (CmpEntity*)gom.QueryInterface(mObjectID, eComponentInterface::Entity);

		// move to init position
		if (controlmanager.GetAction(0, Game_Action::OK))
		{
			entity->SetPosition(mInitPosition);
		}

// 		Quaternion deltaQ(Vector3::UnitY, ToRadians(1.0f));
// 		entity->SetRotation(entity->GetRotation()*deltaQ);

		// HACK : sorry, kill me.
		if (!mbLoadHasCompleted)
		{
			const PackageDesc& packageDesc = PackageManager::GetInstance().GetPackageDesc(mPackageHandle);
			mbLoadHasCompleted = (packageDesc.Status == ePackageStatus::Loaded);

			if (mbLoadHasCompleted)
			{
				for (uint32 i = 0; i < packageDesc.Resources.GetSize(); ++i)
				{
					if (packageDesc.Resources[i].Type == eFileType::Model)
					{
						Model* model = ModelManager::GetInstance().Get(packageDesc.Resources[i].Handle);

						ICmpRenderable* mesh = (ICmpRenderable*)gom.CreateComponent(mObjectID, eComponentType::StaticMesh);
						mesh->SetModel(model);
					}
				}
			}
		}
	}

	// HACK
	void Ball::SetVelocity(const Vector3& velocity)
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		CmpMassMovable* massMovable = (CmpMassMovable*)gom.QueryInterface(mObjectID, eComponentInterface::Movable);
		massMovable->Velocity = velocity;
	}

	void Ball::SetPosition(const Vector3& position, const Quaternion& rotation)
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		CmpEntity* entity = (CmpEntity*)gom.QueryInterface(mObjectID, eComponentInterface::Entity);
		entity->SetPosition(position);
		entity->SetRotation(rotation);
	}
}