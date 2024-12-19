#include "MessyGame.h"

#include "Gameplay/gamestate/GamestateManager.h"
#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "Gameplay/ComponentSystem/Components/CmpEntity.h"
#include "Gameplay/ComponentSystem/Components/CmpCamera.h"

#include "Gamestate/SplashScreenState.h"
#include "Gamestate/MainMenuState.h" 
#include "GameState/LoadingScreenState.h"
#include "gamestate/GamePlayState.h"
#include "Gamestate/PauseMenuState.h"
#include "Controller/ControllerManager.h"
#include "System/Math/MathUtility.h"

#include "PlayerManager.h"
#include "StadiumManager.h"

// Hack : player test
#include "Gameplay/GameDefine.h"
//#include "Animation/Animation.h"
//#include "Animation/Skeleton.h"
//
//static Animation* mAnimation = nullptr;
//static Skeleton* mSkeleton = nullptr;

namespace messy
{
	// TOOD: set proper FPS
	MessyGame::MessyGame()
		: Game(60)
	{
		StadiumManager::CreateInstance();
		PlayerManager::CreateInstance();
	}

	MessyGame::~MessyGame()
	{
		auto& gameStateManager = GameStateManager::GetInstance();
		for (auto i = 0U; i < gameStateManager.GetStateCount(); ++i)
		{
			gameStateManager.Pop();
		}

		PlayerManager::DeleteInstance();
		StadiumManager::DeleteInstance();
	}

	void MessyGame::SetUpGameStates()
	{
		mPauseMenuState = MakeUnique<PauseMenuState>(eMemPool::Gameplay, "pause menu state");
		mSplashScreenState = MakeUnique<SplashScreenState>(eMemPool::Gameplay, "splash screen state");
		mMainMenuState = MakeUnique<MainMenuState>(eMemPool::Gameplay, "main menu state");
		mLoadingScreenState = MakeUnique<LoadingScreenState>(eMemPool::Gameplay, "loading screen state");
		mGamePlayState = MakeUnique<GamePlayState>(eMemPool::Gameplay, "gameplay state");
		
		mMainMenuState->SetLoadingScrennState(mLoadingScreenState.get());
		mLoadingScreenState->SetGamePlayState(mGamePlayState.get());
		mGamePlayState->SetPauseState(mPauseMenuState.get());
		
		GameStateManager::GetInstance().RequestPush(mMainMenuState.get());
		GameStateManager::GetInstance().RequestPush(mSplashScreenState.get());
	}

	void MessyGame::SetUpInputs()
	{
		//TODO filthy dirty test code replace with real deal
		ControllerManager& controlmanager = ControllerManager::GetInstance();

		KeyboardBinding binding[6];
		binding[0].ControllerKey = fm::GamePad::A;
		binding[0].Key = fm::Keyboard::LeftShift;
		binding[0].Repeat = false;

		binding[1].ControllerKey = fm::GamePad::B;
		binding[1].Key = fm::Keyboard::LeftControl;
		binding[1].Repeat = false;

		binding[2].ControllerKey = fm::GamePad::DPadLeft;
		binding[2].Key = fm::Keyboard::ArrowLeft;
		binding[2].Repeat = true;

		binding[3].ControllerKey = fm::GamePad::DPadRight;
		binding[3].Key = fm::Keyboard::ArrowRight;
		binding[3].Repeat = true;

		binding[4].ControllerKey = fm::GamePad::DPadUp;
		binding[4].Key = fm::Keyboard::ArrowUp;
		binding[4].Repeat = true;

		binding[5].ControllerKey = fm::GamePad::DPadDown;
		binding[5].Key = fm::Keyboard::ArrowDown;
		binding[5].Repeat = true;

		ControllerBinding cb[6];
		cb[0].ActionID = Game_Action::OK;
		cb[0].Key = fm::GamePad::A;
		cb[0].Repeat =false;
		cb[0].ControllerID = 0;
		cb[0].Deadzone = 0.5f;

		cb[1].ActionID = Game_Action::Back;
		cb[1].Key = fm::GamePad::B;
		cb[1].Repeat = false;
		cb[1].ControllerID = 0;
		cb[1].Deadzone = 0.5f;

		cb[2].ActionID = Game_Action::Left;
		cb[2].Key = fm::GamePad::DPadLeft;
		cb[2].Repeat = true;
		cb[2].ControllerID = 0;
		cb[2].Deadzone = 0.f;

		cb[3].ActionID = Game_Action::Right;
		cb[3].Key = fm::GamePad::DPadRight;
		cb[3].Repeat = true;
		cb[3].ControllerID = 0;
		cb[3].Deadzone = 0.f;

		cb[4].ActionID = Game_Action::Up;
		cb[4].Key = fm::GamePad::DPadUp;
		cb[4].Repeat = true;
		cb[4].ControllerID = 0;
		cb[4].Deadzone = 0.f;

		cb[5].ActionID = Game_Action::Down;
		cb[5].Key = fm::GamePad::DPadDown;
		cb[5].Repeat = true;
		cb[5].ControllerID = 0;
		cb[5].Deadzone = 0.f;

		MouseBinding mb[2];
		mb[0].ActionID = Game_Action::OK;
		mb[0].Key = fm::MouseKey::Left;
		mb[0].Repeat = false;

		mb[1].ActionID = Game_Action::Back;
		mb[1].Key = fm::MouseKey::Right;
		mb[1].Repeat = false;

		controlmanager.SetLocalPlayerIndex(0);
		//controlmanager.SetInput(0, 0, 0, 0, 0, mb, 2);
		//controlmanager->SetInput(0, 0, 0, cb, 2, 0, 0);
		//controlmanager->SetInput(0, kbb, 2, 0, 0, 0 ,0); //uncomment to swap to test keyboard
		controlmanager.SetInput(0, binding, 6, cb, 6, mb, 2);
	}

	void MessyGame::SetUpEntities()
	{
		GameObjectManager& gom = GameObjectManager::GetInstance();

		hashid objectid = HashString("BasicCamera");

		CmpEntity* newEntity = (CmpEntity*)gom.CreateObject(objectid);
		newEntity->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

		CmpCamera* camera = (CmpCamera*)gom.CreateComponent(objectid, eComponentType::Camera);
		camera->SetPosition(50.0f, 50.0f, -100.0f);
		camera->Pitch(ToRadians(10.0f));
		camera->SetLens(0.25f * PI, 4.0f / 3.0f, 1.0f, 1000.0f);

		//mSkeleton = fmNew(eMemPool::Resource, "Skeleton instance") Skeleton();

		//Skeleton::BoneNode node1;
		//node1.ID = 0;
		//node1.ParentID = INVALID_INDEX;
		//mSkeleton->mBones.Add(node1);

		//Skeleton::BoneNode node2;
		//node2.ID = 1;
		//node2.ParentID = 0;
		//mSkeleton->mBones.Add(node2);

		//Skeleton::BoneNode node3;
		//node3.ID = 2;
		//node3.ParentID = 1;
		//mSkeleton->mBones.Add(node3);

		//mAnimation = fmNew(eMemPool::Resource, "animation instance") Animation();

		//Animation::BoneAnimation* boneAnim1 = fmNew(eMemPool::Resource, "animation instance") Animation::BoneAnimation();
		//boneAnim1->BoneID = 0;
		//boneAnim1->KeyFrameCount = 3;
		//boneAnim1->KeyFrames[0].TimeStamp = 0.1f;
		//boneAnim1->KeyFrames[0].Translation = Vector3(0.0f, 0.0f, 0.0f);
		//boneAnim1->KeyFrames[1].TimeStamp = 0.5f;
		//boneAnim1->KeyFrames[1].Translation = Vector3(0.0f, 20.0f, 0.0f);
		//boneAnim1->KeyFrames[2].TimeStamp = 1.0f;
		//boneAnim1->KeyFrames[2].Translation = Vector3(0.0f, 0.0f, 0.0f);

		//Animation::BoneAnimation* boneAnim2 = fmNew(eMemPool::Resource, "animation instance") Animation::BoneAnimation();
		//boneAnim2->BoneID = 1;
		//boneAnim2->KeyFrameCount = 3;
		//boneAnim2->KeyFrames[0].TimeStamp = 0.1f;
		//boneAnim2->KeyFrames[0].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 0.0f);
		//boneAnim2->KeyFrames[0].Translation = Vector3(25.0f, 0.0f, 0.0f);
		//boneAnim2->KeyFrames[1].TimeStamp = 0.5f;
		//boneAnim2->KeyFrames[1].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 90.0f);
		//boneAnim2->KeyFrames[1].Translation = Vector3(25.0f, 0.0f, 0.0f);
		//boneAnim2->KeyFrames[2].TimeStamp = 1.0f;
		//boneAnim2->KeyFrames[2].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 0.0f);
		//boneAnim2->KeyFrames[2].Translation = Vector3(25.0f, 0.0f, 0.0f);


		//Animation::BoneAnimation* boneAnim3 = fmNew(eMemPool::Resource, "animation instance") Animation::BoneAnimation();
		//boneAnim3->BoneID = 2;
		//boneAnim3->KeyFrameCount = 3;
		//boneAnim3->KeyFrames[0].TimeStamp = 0.1f;
		//boneAnim3->KeyFrames[0].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 0.0f);
		//boneAnim3->KeyFrames[0].Translation = Vector3(30.0f, 20.0f, 0.0f);
		//boneAnim3->KeyFrames[1].TimeStamp = 0.5f;
		//boneAnim3->KeyFrames[1].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 0.0f);
		//boneAnim3->KeyFrames[1].Translation = Vector3(30.0f, 20.0f, 0.0f);
		//boneAnim3->KeyFrames[2].TimeStamp = 1.0f;
		//boneAnim3->KeyFrames[2].Rotation.FromAxisAndAngle(Vector3(0.0f, 0.0f, 1.0f), 0.0f);
		//boneAnim3->KeyFrames[2].Translation = Vector3(30.0f, 20.0f, 0.0f);

		//mAnimation->mTotalTime = 1.0f;
		//mAnimation->mBoneAnimations.Add(boneAnim1);
		//mAnimation->mBoneAnimations.Add(boneAnim2);
		//mAnimation->mBoneAnimations.Add(boneAnim3);
		//mAnimation->mSkeleton = mSkeleton;

		// HACK : ball test
		char ballname[] = "Ball1";
		for (int i = 0; i < msBallNum; ++i)
		{
			ballname[4] = '0' + i;
			mTestBalls[i].Initialize(Vector3(-4.0f + i * 3.f, 10.0f + i * 1.5f, 30.0f), ballname);
			mTestBalls[i].SetVelocity(Vector3(3.f - 0.5f * i, 0.f, 0.f));
		}
	}

	void MessyGame::Update(const uint32 frameCount)
	{
		//mAnimation->Update(0.016f);

		// TODO remove test
		for (int i = 0; i < msBallNum; ++i)
		{
			mTestBalls[i].Update();

			//Quaternion rotation;
			//rotation.FromRotation(mSkeleton->mBones[i].LocalMatrix);
			//mTestBalls[i].SetPosition(mSkeleton->mBones[i].LocalMatrix.GetTranslation(), rotation);
		}

		Game::Update(frameCount);
	}
}