#include "Player.h"

#include "Animation/AnimationGraph.h"

#include "Controller/ControllerManager.h"

#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "Gameplay/ComponentSystem/Components/CmpEntity.h"
#include "Gameplay/ComponentSystem/Components/CmpCamera.h"
#include "Gameplay/ComponentSystem/Components/CmpSkinnedMeshRenderable.h"

#include "Gameplay/GameDefine.h"

// HACK
#include <random>

namespace messy
{
	Player::Player(const hashid objectid, Model* model)
		: mObjectID(objectid)
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();

		CmpEntity* newEntity = (CmpEntity*)gom.CreateObject(mObjectID);
		// hack : that should be changed.
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<> distrubte(0, 30);
		float x = float(distrubte(generator));
		float y = float(distrubte(generator));
		newEntity->SetPosition(Vector3(x, y, 0.0f)); 

		CmpSkinnedMeshRenderable* skinnedMesh = (CmpSkinnedMeshRenderable*)gom.CreateComponent(mObjectID, eComponentType::SkinnedMesh);
		skinnedMesh->SetModel(model);
	}

	Player::~Player()
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();
		gom.DeleteObject(mObjectID);
	}

	void Player::Init()
	{
		// Hack : test
		// TODO initial real player using player data
		GameObjectManager& gom = GameObjectManager::GetInstance();
		mObjectID = HashString("player1");
		CmpEntity* newEntity = (CmpEntity*)gom.CreateObject(mObjectID);
		newEntity->SetPosition(Vector3(30.0f, 30.0f, 0.0f));
	}

	void Player::Update()
	{
		// Hack : test
		// arrows -> move to each directions, lshift -> move to init position
		//TODO filthy dirty test code replace with real deal
		ControllerManager& controlmanager = ControllerManager::GetInstance();
		GameObjectManager& gom = GameObjectManager::GetInstance();
		ICmpEntity* entity = (ICmpEntity*)gom.QueryInterface(mObjectID, eComponentInterface::Entity);
		Vector3 pos = entity->GetPosition();

		// move to each directions
		if (controlmanager.GetAction(0, Game_Action::Left))
		{
			pos += Vector3(-0.1f, 0.f, 0.f);
		}
		if (controlmanager.GetAction(0, Game_Action::Right))
		{
			pos += Vector3(0.1f, 0.f, 0.f);
		}
		if (controlmanager.GetAction(0, Game_Action::Up))
		{
			pos += Vector3(0.f, 0.1f, 0.f);
		}
		if (controlmanager.GetAction(0, Game_Action::Down))
		{
			pos += Vector3(0.f, -0.1f, 0.f);
		}

		// move to init position
		if (controlmanager.GetAction(0, Game_Action::OK))
		{
			pos = Vector3(30.0f, 30.0f, 0.0f);
		}

		entity->SetPosition(pos);
	}
}

