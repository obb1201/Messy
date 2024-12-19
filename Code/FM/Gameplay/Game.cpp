#include "Game.h"

#include <ctime>
#include <direct.h>

#include "Audio/AudioSystem.h"

#include "Controller/ControllerManager.h"

#include "Gameplay/ComponentSystem/GameObjectManager.h"
#include "Gameplay/Gamestate/GamestateManager.h"
#include "Gameplay/Physics/PhysicsManager.h"
#include "Graphics/Renderer.h"

#include "System/Assert.h"
#include "System/Hash.h"
#include "System/Logger.h"
#include "System/Threading/ThreadManager.h"
#include "System/Threading/WorkerThreadManager.h"
#include "System/File/PackageManager.h"

#include "Resource/ModelManager.h"
#include "Resource/TextureManager.h"

#include "Graphics/FrontEndManager.h"
#include "Graphics/Renderer2D.h"

namespace fm
{
	bool Game::bExit = false;

	Game::Game(int framesPerSecond)
		: mGameInfo(framesPerSecond)
	{
		_chdir("./..");

		Assert(framesPerSecond > 0, "cannot init 0 frame game");
		mUpdateIntervalMS = 1000 / framesPerSecond;
		mUpdateIntervalSec = 1.0f / framesPerSecond;

		Logger::CreateInstance();
		ThreadManager::CreateInstance();
		WorkerThreadManager::CreateInstance();
		GameObjectManager::CreateInstance();
		//LOG_VERBOSE( "Game", "Constructor - begin" );
		//TextureManager::createInstance();
		//LOG_VERBOSE( "Game", "Constructor - end" );

		// Resource Related
		ModelManager::CreateInstance();
		TextureManager::CreateInstance();
		PackageManager::CreateInstance();

		Renderer::CreateInstance();

		AudioSystem::CreateInstance();
		GameStateManager::CreateInstance();
		ControllerManager::CreateInstance();
		PhysicsManager::CreateInstance();

		FrontEndManager::CreateInstance();
		Renderer2D::CreateInstance();
	}

	Game::~Game(void)
	{
		PhysicsManager::DeleteInstance();
		ControllerManager::DeleteInstance();
		GameStateManager::DeleteInstance();
		AudioSystem::DeleteInstance();

		// resource related
		PackageManager::DeleteInstance();
		TextureManager::DeleteInstance();
		ModelManager::DeleteInstance();

		Renderer::DeleteInstance();

		GameObjectManager::DeleteInstance();
		WorkerThreadManager::DeleteInstance();
		ThreadManager::DeleteInstance();
		Logger::DeleteInstance();

		FrontEndManager::DeleteInstance();
		Renderer2D::DeleteInstance();
	}

	void Game::Init(const uint32 width, const uint32 height)
	{
		GamePlatform::Init(width, height);
		GameObjectManager::GetInstance().Init();
		Renderer::GetInstance().Init();
		AudioSystem::GetInstance().Init();

		FrontEndManager::GetInstance().Init();

		ThreadManager::GetInstance().RegisterThread(std::this_thread::get_id(), eThreadID::Main);
		const uint32 numThreads = std::thread::hardware_concurrency();
		WorkerThreadManager::GetInstance().Initialize(numThreads);
	}

	void Game::Update(const uint32 frameCount)
	{
		AudioSystem::GetInstance().Update();
		GamePlatform::Update(frameCount);

		GameStateManager& gameStateManager = GameStateManager::GetInstance();
		gameStateManager.PreUpdate();
		gameStateManager.Update(frameCount);
		gameStateManager.PostUpdate();

		ControllerManager::GetInstance().Update();
		GameObjectManager::GetInstance().Update(mGameInfo.GetElapsedGameSeconds());
		PhysicsManager::GetInstance().Update();
		WorkerThreadManager::GetInstance().RunAndWait();
	}

	void Game::Render(const uint32 frameCount)
	{
		GameStateManager& gameStateManager = GameStateManager::GetInstance();
		gameStateManager.PreRender();
		gameStateManager.Render(frameCount);
		gameStateManager.PostRender();

		GameObjectManager::GetInstance().CullAndSummit();
		Renderer::GetInstance().Render();

		Renderer2D::GetInstance().Render();
	}

	void Game::Run()
	{
		uint32 frameCount = 0;
		//AudioSystem test code TODO: remove
#if 0
		auto * audioSys = AudioSystem::getInstance();
		audioSys->loadFile("..\\..\\Asset\\Audio\\Epoq-Lepidoptera.ogg", 1);//can't use 0 anymore
		audioSys->play(1, false);
		audioSys->stop(1); //check if stop generate any error
		audioSys->play(1, true);
#endif
		clock_t prevTimer = std::clock();

		while (!bExit)
		{
			mGameInfo.tick();

			Update(frameCount);
			BeginRender(frameCount);
			{
				Render(frameCount);
			}
			EndRender(frameCount);

			frameCount = mGameInfo.incrementFrameCount();

			clock_t diff = std::clock() - prevTimer;
			if (diff < 16)
			{
				Sleep(1000 / 60 - diff);
			}
			prevTimer = std::clock();
		};
	}

	void Game::EndRender(const uint32 frameCount)
	{
		Renderer::GetInstance().EndRender();
	}
}