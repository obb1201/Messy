
#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "Gameplay/ComponentSystem/Components/ICmpCollidable.h"
#include "System/Memory.h"
#include "PhysicsManager.h"
#include "System/Math/Vector3.h"

namespace fm
{
	PhysicsManager* PhysicsManager::mInstance = nullptr;
	static const float ROUGH_DISTANCE_SQUARE = 2.f * 2.f;

	void PhysicsManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = fmNew(eMemPool::Gameplay, "Physics Manager instance") PhysicsManager();
	}

	void PhysicsManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated PhysicsManager");
		SafeDelete(mInstance);
	}

	PhysicsManager::PhysicsManager()
	{
	}

	PhysicsManager::~PhysicsManager()
	{
		mCollidables.RemoveAll();
	}

	void PhysicsManager::AddCollidable(ICmpCollidable* collidable)
	{
		mCollidables.Add(collidable);
	}

	void PhysicsManager::Update()
	{
		for (auto i = 0u; i < mCollidables.GetSize(); ++i)
		{
			mCollidables[i]->ResetConnectedList();
		}

		for (auto i = 0u; i < mCollidables.GetSize(); ++i)
		{
			if (!mCollidables[i]->bConnected)
			{
				MakeCollidableGroup(mCollidables[i], i);
			}
		}
		
		// MakeCollidableGroup() doesn't add master, so add master
		for (auto i = 0u; i < mCollidables.GetSize(); ++i)
		{
			if (mCollidables[i]->bMaster)
			{
				mCollidables[i]->AddConnectedList(mCollidables[i]);
			}
		}
	}

	void PhysicsManager::MakeCollidableGroup(ICmpCollidable* const master, uint32 index)
	{
		ICmpCollidable* connectedCollidable = mCollidables[index];

		for (auto i = 0u; i < mCollidables.GetSize(); ++i)
		{
			if (index == i || mCollidables[i]->bConnected)
			{
				continue;
			}

			ICmpCollidable* collidable = mCollidables[i];

			float distanceSquare = Vector3::DistanceSquared(connectedCollidable->RoughData.Position, collidable->RoughData.Position);
			if (distanceSquare <= ROUGH_DISTANCE_SQUARE)
			{
				connectedCollidable->bConnected = true;
				collidable->bConnected = true;
				master->bMaster = true;
				master->AddConnectedList(collidable);
				MakeCollidableGroup(master, i);
			}
		}
	}
}
