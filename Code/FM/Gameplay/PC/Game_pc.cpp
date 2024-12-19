#include "Game_pc.h"

#include "audio/audiosystem.h"
#include "Controller/ControllerManager.h"
#include "gameplay/game.h"
#include "system/assert.h"
#include "system/memory.h"

#include "graphics/renderer.h"

#include "system/hash.h"



namespace fm
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			fm::Game::bExit = true;
			break;

		case WM_SIZE:
		{
			//Renderer* renderer = Renderer::getInstance();
			//if ( renderer )
			//{
			//	renderer->setSize(LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
			//}
		}
			break;
		case WM_MOUSEMOVE:
			ControllerManager::GetInstance().HandleMouse(wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Left, true);
			break;
		case WM_MBUTTONDOWN:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Middle, true);
			break;
		case WM_RBUTTONDOWN:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Right, true);
			break;
		case WM_LBUTTONUP:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Left, false);
			break;
		case WM_MBUTTONUP:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Middle, false);
			break;
		case WM_RBUTTONUP:
			ControllerManager::GetInstance().HandleMouseDown(fm::MouseKey::Right, false);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	static const wchar_t * const windowClass = L"GamePC";

	void GamePC::SetWindowInstance(HINSTANCE windowInstance)
	{
		mWindowInstance = windowInstance;
	}

	void GamePC::Init(const uint32 width, const uint32 height)
	{
		Assert(mWindowInstance != nullptr, "setWindowInstance() first on PC");

		// Register class
		WNDCLASSEX wcex;
		MemSet(&wcex, 0, sizeof(WNDCLASSEX));
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.hInstance = mWindowInstance;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)BLACK_BRUSH;
		wcex.lpszClassName = windowClass;
		wcex.lpfnWndProc = WndProc;

		Verify(RegisterClassEx(&wcex), "failed at register class");

		// Create window
		RECT rc = { 0, 0, ToSigned(width), ToSigned(height) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		mWindowHandle = CreateWindowEx(NULL, windowClass, L"GamePC", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, mWindowInstance, nullptr);

		Assert(mWindowHandle, "failed to create window handle");
		Renderer::GetInstance().SetWindowHandle(mWindowHandle);
		AudioSystem::GetInstance().SetWindowHandle(mWindowHandle);
		ControllerManager::GetInstance().SetWindowHandle(mWindowHandle);

		ShowWindow(mWindowHandle, SW_SHOW);



		//Game::createInstance();
		//Game* game = Game::getInstance();
		//
		//Keyboard::createInstance();
		//Keyboard* keyboard = Keyboard::getInstance();



		//renderer->shutdown();

		//game->deleteInstance();
		//renderer->deleteInstance();
		//Keyboard::getInstance()->deleteInstance();
	}

	GamePC::~GamePC(void)
	{
		DestroyWindow(mWindowHandle);
		// TODO(pope): unregister window class
		//if ( !UnregisterClass( windowClass, windowInstance ) )
		//{
		//	// TODO: log
		//}
	}

	void Game::BeginRender(const uint32 frameCount)
	{
	}

	void GamePC::Update(const uint32 frameCount)
	{
		//
		// Main message loop
		//
		MSG msg;
		//MemSet(&msg, 0, sizeof(MSG));

		ControllerManager::GetInstance().HandleInput();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}