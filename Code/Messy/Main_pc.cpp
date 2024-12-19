#include "windows.h"
#include "Gameplay/MessyGame.h"
#include "System/Memory/MemoryTracker.h"

using namespace fm;
using namespace messy;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	// Start the message loop.
	MessyGame game;
	game.SetWindowInstance(hInstance);
	
	// TODO: set proper width and height
	game.Init(1024, 768);
	game.SetUpGameStates();
	game.SetUpInputs();
	game.SetUpEntities();
	game.Run();

	return 0;
}
